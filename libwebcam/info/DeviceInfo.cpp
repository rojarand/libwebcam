#include "DeviceInfo.h"

namespace webcam
{
	ControlInfo::ControlInfo() {
		this->automatic = false;
		this->manual = false;
		this->min = this->max = this->step = this->default_value = 0;
	}

	DeviceInfo::DeviceInfo()
	{
	}

	DeviceInfo::~DeviceInfo()
	{
	}

	const VideoInfoEnumeration & DeviceInfo::get_video_info_enumeration()const
	{
		return _video_info_enumeration;
	}

	void DeviceInfo::set_video_info_enumeration(const VideoInfoEnumeration & video_info_enumeration_)
	{
		_video_info_enumeration = video_info_enumeration_;
	}

	const ModelInfo & DeviceInfo::get_model_info()const
	{
		return _model_info;
	}

	void DeviceInfo::set_model_info(ModelInfo & model_info_)
	{
		_model_info = model_info_;
	}

	bool DeviceInfo::supports_format(int format_)const
	{
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::VideoInfo & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				return true;
			}
		}
		return false;
	}

	Shape DeviceInfo::get_highest_resolution(int format_)const
	{
        Shape current_resolution(0, 0);
        int best_area = 0;
		size_t count = _video_info_enumeration.count();
		for (size_t index = 0; index < count; index++){
			const webcam::VideoInfo & video_info = _video_info_enumeration.get(index);
			if (video_info.get_format() == format_)
			{
				const Resolutions & r = video_info.get_resolutions();

				for( size_t i = 0; i < r._resolutions.size(); i++ ) {
					const Shape& shape = r._resolutions.at(i);
					int area = shape.width*shape.height;
					if( area > best_area ) {
						best_area = area;
						current_resolution.width = shape.width;
						current_resolution.height = shape.height;
					}
				}
				int area = r._height_max*r._width_max;
				if( area > best_area ) {
					best_area = area;
					current_resolution.width = r._width_max;
					current_resolution.height = r._height_max;
				}
			}
		}
		return current_resolution;
	}

	const std::string& DeviceInfo::get_port() const {
		return _port;
	}

	std::string& DeviceInfo::get_port() {
		return _port;
	}

	const std::string& DeviceInfo::get_serial() const {
		return _serial;
	}

	std::string& DeviceInfo::get_serial() {
		return _serial;
	}

	const ControlInfo& DeviceInfo::get_gain_info() const {
		return _control_gain;
	}

	const ControlInfo& DeviceInfo::get_exposure_info() const {
		return _control_exposure;
	}

	const ControlInfo& DeviceInfo::get_focus_info() const {
		return _control_focus;
	}

	ControlInfo& DeviceInfo::get_gain_info() {
		return _control_gain;
	}

	ControlInfo& DeviceInfo::get_exposure_info() {
		return _control_exposure;
	}

	ControlInfo& DeviceInfo::get_focus_info() {
		return _control_focus;
	}
}