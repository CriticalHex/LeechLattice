#include "Listener.h"

using namespace std;

void Listener::initialize() {
  REFERENCE_TIME NS_PER_SEC = 1000000000;
  HRESULT hr;
  plan = fftwf_plan_dft_r2c_1d(SIZE_ESTIMATE, nullptr, nullptr, FFTW_ESTIMATE);

  hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    cout << "Failed to initialize" << endl;
  }

  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                        __uuidof(IMMDeviceEnumerator),
                        (void **)&pDeviceEnumerator);
  if (FAILED(hr)) {
    cout << "Failed to create device enumerator" << endl;
  }

  hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
                                                  &pAudioRenderDevice);
  if (FAILED(hr)) {

    cout << "Failed to get the audio render endpoint" << endl;
  }

  hr =
      pAudioRenderDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL,
                                   NULL, (void **)&pMeterInformation);
  if (FAILED(hr)) {
    cout << "Failed to activate meterInformation device" << endl;
  }

  // hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole,
  //                                                 &pAudioCaptureDevice);
  // if (FAILED(hr)) {
  //   cout << "Failed to get audio capture endpoint" << endl;
  // }

  hr = pAudioRenderDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
                                    (void **)&pAudioClient);
  if (FAILED(hr)) {
    cout << "Failed to activate audio client" << endl;
  }

  hr = pAudioClient->GetMixFormat(&pMixFormat);
  if (FAILED(hr)) {
    cout << "Failed to set audio format" << endl;
  } else {
    cout << "Bits: " << pMixFormat->wBitsPerSample << endl;
    cout << "Sample Rate: " << pMixFormat->nSamplesPerSec << endl;
    cout << "Channels: " << pMixFormat->nChannels << endl;

    sampleRate = pMixFormat->nSamplesPerSec;
    // fftOutput.resize(sampleRate);
  }

  hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pMixFormat,
                                nullptr);
  if (FAILED(hr)) {
    cout << "Failed to set audio format" << endl;
  }

  hr = pAudioClient->GetBufferSize(&bufferFrameCount);
  if (FAILED(hr)) {
    cout << "Failed to get buffer size" << endl;
  } else {
    cout << "Buffer size: " << bufferFrameCount << endl;
  }

  hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient),
                                (void **)&pCaptureClient);
  if (FAILED(hr)) {
    switch (hr) {
    case E_POINTER:
      cout << "Parameter ppv is NULL." << endl;
      break;
    case E_NOINTERFACE:
      cout << "The requested interface is not available." << endl;
      break;
    case AUDCLNT_E_NOT_INITIALIZED:
      cout << "The audio stream has not been initialized." << endl;
      break;
    case AUDCLNT_E_WRONG_ENDPOINT_TYPE:
      cout << "The caller tried to access an IAudioCaptureClient interface on "
              "a rendering endpoint, or an IAudioRenderClient interface on a "
              "capture endpoint."
           << endl;
      break;
    case AUDCLNT_E_DEVICE_INVALIDATED:
      cout << "The audio endpoint device has been unplugged, or the audio "
              "hardware or associated hardware resources have been "
              "reconfigured, disabled, removed, or otherwise made unavailable "
              "for use."
           << endl;
      break;
    case AUDCLNT_E_SERVICE_NOT_RUNNING:
      cout << "The Windows audio service is not running." << endl;
      break;
    }
    cout << "Failed to get capture client" << endl;
  }

  hr = pAudioClient->Start();
  if (FAILED(hr)) {
    cout << "Failed to start audio client" << endl;
  }
}

Listener::Listener() { initialize(); }

Listener::Listener(int _frequencyDataSize) {
  initialize();
  frequencyBins = _frequencyDataSize;
}

Listener::~Listener() {
  pMeterInformation->Release();
  pAudioRenderDevice->Release();
  pAudioCaptureDevice->Release();
  pDeviceEnumerator->Release();
  pAudioClient->Stop();
  pAudioClient->Release();
  pCaptureClient->Release();

  CoUninitialize();
  fftwf_destroy_plan(plan);
}

void Listener::getAudioLevel(float *volume) {
  pMeterInformation->GetPeakValue(volume);
}

float abs(fftwf_complex v) { return sqrt(v[0] * v[0] + v[1] * v[1]); }

float lerp(float start, float end, float t) {
  return start + t * (end - start);
}

float perp(const std::vector<Point> &points, float x) {
  float result = 0.0;
  for (size_t i = 0; i < points.size(); i++) {
    float term = points[i].y;
    for (size_t j = 0; j < points.size(); j++) {
      if (i != j && points[i].x != points[j].x) {
        term *= (x - points[j].x) / (points[i].x - points[j].x);
      }
    }
    result += term;
  }
  return result;
}

vector<float> Listener::getFrequencyData() {
  HRESULT hr;
  vector<BYTE> data;
  vector<float> frequencyVolumes(frequencyBins, 0.0f);
  hr = pCaptureClient->GetNextPacketSize(&packetLength);
  if (FAILED(hr)) {
    cout << "Failed to get the next packet size" << endl;
  }
  while (packetLength != 0) {
    // cout << "L: " << packetLength << endl;
    hr = pCaptureClient->GetBuffer(&pBufferData, &numFramesRetrieved, &flags,
                                   NULL, NULL);
    // cout << "N: " << numFramesRetrieved << endl;
    if (FAILED(hr)) {
      cout << "Failed to get buffer" << endl;
    }
    // if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
    //   cout << "Got silent" << endl;
    // }

    data.insert(data.end(), pBufferData,
                pBufferData + numFramesRetrieved * sizeof(BYTE));

    hr = pCaptureClient->ReleaseBuffer(numFramesRetrieved);
    if (FAILED(hr)) {
      cout << "Failed to release the buffer" << endl;
    }

    hr = pCaptureClient->GetNextPacketSize(&packetLength);
    if (FAILED(hr)) {
      cout << "Failed to get the next packet size" << endl;
    }
  }

  if (data.size() != 0) {
    // cout << "D: " << data.size() << endl;
    // data.size() is at least sampleRate/100 and at most bufferFrameCount
    size_t numFrames = data.size() / (pMixFormat->nChannels * sizeof(float));

    vector<float> mergedChannelData(numFrames);

    switch (pMixFormat->wBitsPerSample) {
    case 16: {
      return vector<float>(frequencyBins);
      // i no wanna :(
      break;
    }
    case 32: {
      // creating new array compressing the BYTES in data to FLOATS, so size
      // shrinks by sizeof(float) (4)
      float *pFloatData = reinterpret_cast<float *>(data.data());

      // numFrames is theoretically half the size of the above array
      for (int i = 0; i < numFrames; i++) {
        float sum = 0.0f;
        for (int channel = 0; channel < pMixFormat->nChannels; channel++) {
          int index = i * pMixFormat->nChannels + channel;
          sum += pFloatData[index];
        }
        mergedChannelData[i] = sum / pMixFormat->nChannels;
      }
      break;
    }
    default: {
      return vector<float>(frequencyBins);
      break;
    }
    }

    // windowing
    for (int i = 0; i < numFrames; ++i) {
      float windowFactor = 0.5f * (1 - cos(2 * M_PI * i / (numFrames - 1)));
      mergedChannelData[i] *= windowFactor;
    }

    fftOutput = vector<fftwf_complex>(numFrames / 2 + 1);

    fftwf_plan fftPlan = fftwf_plan_dft_r2c_1d(
        numFrames, mergedChannelData.data(), fftOutput.data(), FFTW_ESTIMATE);
    fftwf_execute(fftPlan);

    fftwf_destroy_plan(fftPlan);

    float samplesPerIndex = float(sampleRate) / fftOutput.size();

    // for every combination this seems to be:
    // 13
    int maxIndex = ceil(20000.f / samplesPerIndex);
    // 0
    int minIndex = floor(20.f / samplesPerIndex);

    float renderSize = maxIndex - minIndex;

    float binsPerFFTBin = frequencyBins / renderSize;

    // linear interpolation
    float atIndex;
    float atNextIndex;
    for (int i = minIndex; i < maxIndex; i++) {
      atIndex = abs(fftOutput[i]);
      atNextIndex = abs(fftOutput[i + 1]);
      for (int j = 0; j < binsPerFFTBin; j++) {
        frequencyVolumes[j + (binsPerFFTBin * i)] =
            lerp(atIndex, atNextIndex, j / binsPerFFTBin);
      }
    }

    // polynomial interpolation
    // vector<Point> points;
    // // cout << "{";
    // for (int i = minIndex; i <= maxIndex; i++) {
    //   Point point(i / float(maxIndex), abs(fftOutput[i]));
    //   // cout << "Point(" << point.x << ", " << point.y << "), ";
    //   points.push_back(point);
    // }
    // // cout << "}" << endl;
    // for (int i = 0; i < frequencyBins; i++) {
    //   frequencyVolumes[i] = perp(points, i / float(frequencyBins));
    // }

    // Normalize volume values
    float maxVolume =
        *max_element(frequencyVolumes.begin(), frequencyVolumes.end());
    if (maxVolume > 0) {
      for (int i = 0; i < frequencyVolumes.size(); ++i) {
        frequencyVolumes[i] /= maxVolume;
      }
    }
  }

  return frequencyVolumes;
  // return vector<float>();
}