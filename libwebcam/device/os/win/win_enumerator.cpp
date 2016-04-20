#ifdef WIN32

#include <Strmif.h>
#include <Mmreg.h>
#include <Mtype.h>

#include "win_enumerator.h"

#include "qedit.h"
#include "CComPtr.h"
#include "win_webcam_utils.h"

namespace webcam
{

	enumerator::enumerator(IEnumMoniker * enum_moniker_, bool cleanUp_)
		:_enum_moniker(enum_moniker_)
		, cleanUp(cleanUp_)
	{
	}


	enumerator::~enumerator()
	{
		if (_enum_moniker)
		{
			_enum_moniker->Release();
		}
		if (cleanUp)
		{
			CoUninitialize();
		}
	}

	enumerator enumerator::create()
	{
		HRESULT res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		bool cleanUp = (S_OK == res);
		
		CComPtr<ICreateDevEnum> dev_enum = nullptr;
		res = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&dev_enum));
		if (S_OK != res)
		{
			throw system_device_enumerator_exception("can not create system device enumerator", res);
		}

		// Enumerate video capture devices
		IEnumMoniker * enum_moniker;
		res = dev_enum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enum_moniker, 0);
		if (S_OK != res)
		{
			throw video_device_enumerator_exception("can not create video device enumerator", res);
		}
		return enumerator(enum_moniker, cleanUp);
	}

	device_info_provider enumerator::next()
	{
		IMoniker * moniker = nullptr;
		ULONG fetched = 0;
		HRESULT res = _enum_moniker->Next(1, &moniker, &fetched);
		return device_info_provider((res == S_OK) ? moniker : nullptr);
	}

	device_info_enumeration enumerator::enumerate()
	{
		webcam::enumerator enumerator = enumerator::create();
		device_info_enumeration enumeration;
		while (true){

			webcam::device_info_provider provider = enumerator.next();
			if (provider.is_valid()){

				webcam::device_info device_info = provider.provide();
				enumeration.put(device_info);
			}
			else{
				break;
			}
		}
		return enumeration;
	}

}
#endif
