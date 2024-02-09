
#include <algorithm>
#include <audioclient.h>
#include <cmath>
#include <complex>
#include <endpointvolume.h>
#include <fftw3.h>
#include <iostream>
#include <mmdeviceapi.h>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

__CRT_UUID_DECL(IAudioMeterInformation, 0xC02216F6, 0x8C67, 0x4B5B, 0x9D, 0x00,
                0xD0, 0x08, 0xE7, 0x3E, 0x00, 0x64);

MIDL_INTERFACE("C02216F6-8C67-4B5B-9D00-D008E73E0064")
IAudioMeterInformation : public IUnknown {
public:
  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPeakValue(
      /* [out] */ float *pfPeak) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMeteringChannelCount(
      /* [out] */
      UINT * pnChannelCount) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetChannelsPeakValues(
      /* [in] */ UINT32 u32ChannelCount,
      /* [size_is][out] */ float *afPeakValues) = 0;

  virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryHardwareSupport(
      /* [out] */
      DWORD * pdwHardwareSupportMask) = 0;
};

class Listener {
private:
  IMMDeviceEnumerator *pDeviceEnumerator = nullptr;
  IMMDevice *pAudioRenderDevice = nullptr;
  IAudioMeterInformation *pMeterInformation = nullptr;

  IMMDevice *pAudioCaptureDevice = nullptr;
  IAudioClient *pAudioClient = nullptr;
  WAVEFORMATEX *pMixFormat;
  IAudioCaptureClient *pCaptureClient = nullptr;
  fftwf_plan plan;
  const int SIZE_ESTIMATE = 306;

  vector<float> capturedAudioData;
  vector<complex<float>> frequencyData;
  int sampleRate;
  int frequencyDataSize = 25;

  UINT32 bufferFrameCount;
  UINT32 numFramesAvailable;
  UINT32 packetLength = 0;
  BOOL bDone = FALSE;
  BYTE *pData;
  DWORD flags;

  void fft();

public:
  void printAudioMeterInformation();
  double getAudioLevel();
  vector<float> getFrequencyData();
  void initialize();
  Listener();
  Listener(int _frequencyDataSize);
  ~Listener();
};

void Listener::initialize() {
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

  hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole,
                                                  &pAudioCaptureDevice);
  if (FAILED(hr)) {
    cout << "Failed to get audio capture endpoint" << endl;
  }

  hr = pAudioCaptureDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
                                     (void **)&pAudioClient);
  if (FAILED(hr)) {
    cout << "Failed to activate audio client" << endl;
  }

  hr = pAudioClient->GetMixFormat(&pMixFormat);
  if (FAILED(hr)) {
    cout << "Failed to set audio format" << endl;
  }

  hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 7000000, 0,
                                pMixFormat, NULL);
  if (FAILED(hr)) {
    cout << "Failed to set audio format" << endl;
  }

  hr = pAudioClient->GetBufferSize(&bufferFrameCount);
  if (FAILED(hr)) {
    cout << "Failed to get buffer size" << endl;
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
  frequencyDataSize = _frequencyDataSize;
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

void Listener::printAudioMeterInformation() {
  float peakValue;
  float rmsValue;

  pMeterInformation->GetPeakValue(&peakValue);

  cout << "Peak Value: " << peakValue << endl;

  pMeterInformation->GetChannelsPeakValues(1, &rmsValue);
  cout << "RMS Value: " << rmsValue << endl;
}

double Listener::getAudioLevel() {
  float volume;
  pMeterInformation->GetPeakValue(&volume);
  return volume;
}

void Listener::fft() {
  frequencyData.resize(capturedAudioData.size());

  cout << capturedAudioData.size() << endl;

  fftwf_execute_dft_r2c(
      plan, capturedAudioData.data(),
      reinterpret_cast<fftwf_complex *>(frequencyData.data()));
}

vector<float> Listener::getFrequencyData() {
  pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
  pCaptureClient->ReleaseBuffer(numFramesAvailable);
  std::vector<float> audioSamples(framesAvailable);
  for (UINT32 i = 0; i < framesAvailable; ++i) {
    audioSamples[i] = static_cast<float>(
        *reinterpret_cast<INT16 *>(pData + i * sizeof(INT16)));
  }

  fft();

  // // Calculate frequency resolution
  // double frequencyResolution =
  //     static_cast<double>(sampleRate) / capturedAudioData.size();

  // // Calculate volume at each frequency band
  // vector<float> frequencyVolumes(frequencyDataSize, 0.0f);
  // for (int i = 0; i < frequencyData.size(); ++i) {
  //   // Calculate frequency corresponding to current FFT bin
  //   double frequency = i * frequencyResolution;

  //   // Determine which frequency band this frequency belongs to
  //   int bandIndex =
  //       static_cast<int>(floor((frequency / sampleRate) *
  //       frequencyDataSize));
  //   if (bandIndex >= 0 && bandIndex < frequencyDataSize) {
  //     // Accumulate amplitude for the corresponding band
  //     frequencyVolumes[bandIndex] += abs(frequencyData[i]);
  //   }
  // }

  // // Normalize volume values
  // float maxVolume =
  //     *max_element(frequencyVolumes.begin(), frequencyVolumes.end());
  // if (maxVolume > 0) {
  //   for (int i = 0; i < frequencyVolumes.size(); ++i) {
  //     frequencyVolumes[i] /= maxVolume;
  //   }
  // }

  // return frequencyVolumes;
  return vector<float>();
}