#pragma once

#ifdef WIN32

#include <tchar.h>
#include <dshow.h>
#include "CComPtr.h"
#include "../../../info/device_info.h"


struct IPropertyBag;
namespace webcam
{

	class device_info_provider
	{
	public:
		device_info_provider(IMoniker * moniker_);
		bool is_valid()const;
		device_info provide();
		IBaseFilter * create_base_filter();
		static model_info provide_model_info(const std::string & device_path_, std::istream & vendor_product_stream_);
	private:
		model_info get_model_info();
		video_info_enumeration get_video_info_enumeration();
		static std::string find_hex_id_of(const std::string & element_, const std::string & device_path_);

		static model_info get_model_info(IPropertyBag * property_bag_);
		static std::string read_property(IPropertyBag * property_bag_, const wchar_t * property_name_);
		static IEnumMoniker * create_enum_moniker();

		CComPtr<IMoniker> _moniker;
	};
}

#endif
