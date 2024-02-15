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

vector<float> Listener::getFrequencyData() {
  HRESULT hr;
  vector<BYTE> data;
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
    if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
      cout << "Got silent" << endl;
    }

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
    //data.size() is at least 480 and at most 1056
    size_t numFrames = data.size() / (pMixFormat->nChannels * sizeof(float));

    complexData.clear();
    complexData = vector<fftwf_complex>(numFrames);

    switch (pMixFormat->wBitsPerSample) {
    case 16: {
      short *pShortData = reinterpret_cast<short *>(data.data());
      for (int i = 0; i < numFramesRetrieved; ++i) {
        float sumReal = 0.0f;
        for (int channel = 0; channel < pMixFormat->nChannels; ++channel) {
          int index = i * pMixFormat->nChannels + channel;
          sumReal += static_cast<float>(pShortData[index]);
        }
        complexData[i][0] = sumReal / pMixFormat->nChannels;
        complexData[i][1] = 0;
      }
      break;
    }
    case 32: {
      float *pFloatData = reinterpret_cast<float *>(data.data());
      for (int i = 0; i < numFrames; i++) {
        float sumReal = 0.0f;
        for (int channel = 0; channel < pMixFormat->nChannels; channel++) {
          int index = i * pMixFormat->nChannels + channel;
          sumReal += pFloatData[index];
        }
        complexData[i][0] = sumReal / pMixFormat->nChannels;
        complexData[i][1] = 0;
      }
      break;
    }
    default: {
      return vector<float>(frequencyBins);
      break;
    }
    }
    // cout << complexData.size() << endl;

    // // windowing
    // for (int i = 0; i < numFrames; ++i) {
    //   float windowFactor = 0.5f * (1 - cos(2 * M_PI * i /
    //   (numFrames - 1))); complexData[i][0] *= windowFactor;
    // }

    fftwf_plan fftPlan =
        fftwf_plan_dft_1d(numFrames, complexData.data(), complexData.data(),
                          FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_execute(fftPlan);

    fftwf_destroy_plan(fftPlan);
  }

  // Calculate frequency resolution
  double frequencyResolution = double(sampleRate) / complexData.size();

  // Calculate volume at each frequency band
  vector<float> frequencyVolumes(frequencyBins, 0.0f);
  for (int i = 0; i < complexData.size(); i++) {
    // Calculate frequency corresponding to current FFT bin
    double frequency = i * frequencyResolution;

    // Determine which frequency band this frequency belongs to
    int bandIndex = int(floor((frequency / sampleRate) * frequencyBins));
    if (bandIndex >= 0 && bandIndex < frequencyBins) {
      // Accumulate amplitude for the corresponding band
      frequencyVolumes[bandIndex] += abs(complexData[i]);
    }
  }

  // Normalize volume values
  float maxVolume =
      *max_element(frequencyVolumes.begin(), frequencyVolumes.end());
  if (maxVolume > 0) {
    for (int i = 0; i < frequencyVolumes.size(); ++i) {
      frequencyVolumes[i] /= maxVolume;
    }
  }

  return frequencyVolumes;
  // return vector<float>();
}