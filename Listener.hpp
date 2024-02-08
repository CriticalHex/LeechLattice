
#include <endpointvolume.h>
#include <iostream>
#include <mmdeviceapi.h>
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
  IMMDeviceEnumerator *pEnumerator = nullptr;
  IMMDevice *pDevice = nullptr;
  IAudioMeterInformation *pMeterInformation = nullptr;

public:
  void printAudioMeterInformation();
  double getAudioLevel();
  Listener();
  ~Listener();
};

Listener::Listener() {
  CoInitialize(NULL);

  CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                   __uuidof(IMMDeviceEnumerator), (void **)&pEnumerator);
  pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
  pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL,
                    (void **)&pMeterInformation);
}

Listener::~Listener() {
  pMeterInformation->Release();
  pDevice->Release();
  pEnumerator->Release();

  CoUninitialize();
}

void Listener::printAudioMeterInformation() {
  float peakValue;
  float rmsValue;

  pMeterInformation->GetPeakValue(&peakValue);

  std::cout << "Peak Value: " << peakValue << std::endl;

  pMeterInformation->GetChannelsPeakValues(1, &rmsValue);
  std::cout << "RMS Value: " << rmsValue << std::endl;
}

double Listener::getAudioLevel() {
  float volume;
  pMeterInformation->GetPeakValue(&volume);
  return volume;
}