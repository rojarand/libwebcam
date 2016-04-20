
#include "device_info_provider.h"

#ifdef WIN32

#include <fstream>
#include <sstream>
#include <dshow.h>
#include <comutil.h>
#include <Strmif.h>
#include <Mmreg.h>
#include <Mtype.h>
#include "qedit.h"
#include "win_webcam_utils.h"
#include "../../../info/device_info_parser.h"
#include "../../../error/webcam_exception.h"
#include "../../../utils/text_utils.h"

namespace webcam
{
	device_info_provider::device_info_provider(IMoniker * moniker_)
		:_moniker(moniker_)
	{

	}

	bool device_info_provider::is_valid()const
	{
		return _moniker != nullptr;
	}

	device_info device_info_provider::provide(){
		
		webcam::device_info device_info;
		model_info model_info = get_model_info();
		device_info.set_model_info(model_info);
		webcam::video_info_enumeration video_info_enumeration = get_video_info_enumeration();
		device_info.set_video_info_enumeration(video_info_enumeration);
		return device_info;
	}

	model_info device_info_provider::get_model_info()
	{
		CComPtr<IPropertyBag> property_bag = nullptr;
		HRESULT res = _moniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&property_bag);//bind the properties of the moniker
		if (S_OK == res)
		{
			model_info model_info = get_model_info(property_bag);
			return model_info;
		}
		return model_info();//default
	}

	model_info device_info_provider::get_model_info(IPropertyBag * property_bag_)
	{
		std::string path = read_property(property_bag_, L"DevicePath");
		//std::istringstream stream(VENDORS_AND_DEVICES);
		std::ifstream stream("usb.ids");
		model_info model_info = provide_model_info(path, stream);

		std::string friendly_name = read_property(property_bag_, L"FriendlyName");
		model_info.set_friendly_name(friendly_name);

		std::string desc = read_property(property_bag_, L"Description");
		model_info.set_desc(desc);

		return model_info;
	}

	std::string device_info_provider::read_property(IPropertyBag * property_bag_, const wchar_t * property_name_)
	{
		std::string property_value = "";
		VARIANT var;
		VariantInit(&var);// Initialise the variant data type
		HRESULT res = property_bag_->Read(property_name_, &var, 0);//extract the FriendlyName
		if (SUCCEEDED(res))
		{
			BSTR bStr = SysAllocString(var.bstrVal);
			property_value = _com_util::ConvertBSTRToString(bStr);
			SysFreeString(bStr);
		}
		VariantClear(&var);//clear the variant data type
		return property_value;
	}

	IBaseFilter * device_info_provider::create_base_filter()
	{
		IBaseFilter * base_filter_source = nullptr;
		HRESULT res = _moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&base_filter_source);
		if (S_OK != res)
		{
			throw webcam_exception("can not create base filter", res);
		}
		return base_filter_source;
	}

	video_info_enumeration device_info_provider::get_video_info_enumeration()
	{
		webcam::video_info_enumeration enumeration;
		CComPtr<IBaseFilter> base_filter_source = nullptr;
		HRESULT res = _moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&base_filter_source);
		if (S_OK != res)
		{
			return enumeration;
		}
		CComPtr<IPin> pin = win_webcam_utils::get_out_pin(base_filter_source, 0);
		CComPtr<IEnumMediaTypes> mediaTypesEnumerator = nullptr;
		res = pin->EnumMediaTypes(&mediaTypesEnumerator);
		if (S_OK != res)
		{
			return enumeration;
		}

		AM_MEDIA_TYPE * mediaType = nullptr;
		while (S_OK == mediaTypesEnumerator->Next(1, &mediaType, nullptr))
		{
			if ((mediaType->formattype == FORMAT_VideoInfo) &&
				(mediaType->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
				(mediaType->pbFormat != nullptr))
			{
				const format * format = create_format(&mediaType->subtype);
				VIDEOINFOHEADER * videoInfoHeader = (VIDEOINFOHEADER*)mediaType->pbFormat;
				const BITMAPINFOHEADER & bitmapInfoHeader = videoInfoHeader->bmiHeader;
				int width = bitmapInfoHeader.biWidth;  // Supported width
				int height = bitmapInfoHeader.biHeight; // Supported height
				int image_byte_count = bitmapInfoHeader.biSizeImage;
				webcam::resolution resolution(width, height);
				enumeration.put(video_info(resolution, *format, image_byte_count));
				delete format;
			}
			FreeMediaType(*mediaType);
		}
		return enumeration;
	}

	model_info device_info_provider::provide_model_info(const std::string & device_path_, std::istream & vendor_product_stream_)
	{
		const std::string vendor_hex_id = find_hex_id_of("vendor", device_path_);
		const std::string model_hex_id = find_hex_id_of("product", device_path_);
		
		indexable vendor_indexable(text_utils::hex_str_2_dec_int(vendor_hex_id));
		indexable modex_indexable(text_utils::hex_str_2_dec_int(model_hex_id));
		vendor_model_indices indices(vendor_indexable, modex_indexable);
		return device_info_parser::provide_model_info(indices, vendor_product_stream_);
	}

	std::string device_info_provider::find_hex_id_of(const std::string & element_, const std::string & device_path_)
	{
		const std::string token = element_.substr(0, 1) + "id_";
		const std::string::size_type token_pos = device_path_.find(token);
		if (token_pos != std::string::npos)
		{
			const std::string::size_type id_pos = token_pos + token.size();
			const std::string::size_type id_len = 4;
			std::string id = device_path_.substr(id_pos, id_len);
			return id;
		}
		return "";
	}
}

#endif
