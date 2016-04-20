#include "win_webcam_utils.h"

#ifdef WIN32
#include "CComPtr.h"

namespace webcam
{

	win_webcam_utils::win_webcam_utils()
	{
	}


	win_webcam_utils::~win_webcam_utils()
	{
	}

	HRESULT win_webcam_utils::get_pin(IBaseFilter * filter, PIN_DIRECTION dirrequired, int num, IPin **pin)
	{
		CComPtr<IEnumPins> enum_pin;
		*pin = nullptr;

		HRESULT res = filter->EnumPins(&enum_pin);
		if (S_OK != res)
		{
			return res;
		}

		ULONG found;
		IPin *pPin;
		res = E_FAIL;

		while (S_OK == enum_pin->Next(1, &pPin, &found))
		{
			PIN_DIRECTION pindir = (PIN_DIRECTION)3;

			pPin->QueryDirection(&pindir);
			if (pindir == dirrequired)
			{
				if (num == 0)
				{
					*pin = pPin;  // Return the pin's interface
					res = S_OK;      // Found requested pin, so clear error
					break;
				}
				num--;
			}
			pPin->Release();
		}
		return res;
	}

	IPin * win_webcam_utils::get_in_pin(IBaseFilter * filter, int nPin)
	{
		IPin * pin = nullptr;
		get_pin(filter, PINDIR_INPUT, nPin, &pin);
		return pin;
	}

	IPin * win_webcam_utils::get_out_pin(IBaseFilter * filter, int nPin)
	{
		IPin * pin = nullptr;
		get_pin(filter, PINDIR_OUTPUT, nPin, &pin);
		return pin;
	}
}

#endif
