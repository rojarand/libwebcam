
#include <string>
#include <sstream>
#include <algorithm>
#include "device_info_parser.h"
#include "../utils/text_utils.h"

namespace webcam
{

	bool device_info_parser::is_vendor_line(const std::string & line_)
	{
		std::string::size_type space_index = line_.find_first_of(" ");
		std::string::size_type tab_index = line_.find_first_of("\t");
		return (0 != space_index) && (0 != tab_index);
	}

	bool device_info_parser::is_product_line(const std::string & line_)
	{
		std::string::size_type space_index = line_.find_first_of(" ");
		std::string::size_type tab_index = line_.find_first_of("\t");
		return (0 == space_index) || (0 == tab_index);
	}

	bool device_info_parser::is_comment_line(const std::string & line_)
	{
		std::string::size_type hash_index = line_.find_first_of("#");
		return (0 == hash_index);
	}

	bool device_info_parser::is_empty_line(const std::string & line_)
	{
		return line_.empty();
	}

	basic_info device_info_parser::build_basic_info(const std::string & line_)
	{
		std::istringstream istringstream(line_);
		std::string hex_index;
		istringstream >> hex_index;

		int index = text_utils::hex_str_2_dec_int(hex_index);

		std::string name = "";
		while (istringstream.good()){
			std::string name_token = "";
			istringstream >> name_token;
			std::replace(name_token.begin(), name_token.end(), '\"', '\'');
			if (name.size())
			{
				name += " ";
			}
			name += name_token;
		}
		return basic_info(index, name);
	}

	model_info device_info_parser::provide_model_info(const vendor_model_indices & vendor_model_indices_,
		std::istream & vendor_product_stream_)
	{
		bool has_vendor = false;

		class model_info model_info;
		class vendor_info vendor_info;
		std::string line = "";
		while (std::getline(vendor_product_stream_, line)){

			if (is_empty_line(line))
			{
				continue;
			}
			if (is_comment_line(line))
			{
				continue;
			}

			if (has_vendor)
			{
				if (is_vendor_line(line))
				{
					break;//next vendor stop processing
				}
				else if (is_product_line(line))
				{
					class basic_info basic_info = build_basic_info(line);
					const indexable & model_id = vendor_model_indices_.get_model_id();
					if (model_id == basic_info.get_id())
					{
						model_info.set_id(basic_info.get_id());
						model_info.set_name(basic_info.get_name());
						break;
					}
				}
			}
			else
			{
				if (is_vendor_line(line))
				{
					class basic_info basic_info = build_basic_info(line);
					const indexable & vendor_id = vendor_model_indices_.get_vendor_id();
					if (vendor_id == basic_info.get_id())
					{
						vendor_info.set_id(basic_info.get_id());
						vendor_info.set_name(basic_info.get_name());
						has_vendor = true;
					}
				}
			}
		}

		model_info.set_vendor_info(vendor_info);
		return model_info;
	}
}