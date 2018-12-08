#pragma once

#include <string>
#include "ModelInfo.h"
#include "VideoInfoEnumeration.h"

namespace webcam
{
	struct ControlInfo
	{
		ControlInfo();

		// if true then this method of control is available
		bool automatic;
		bool manual;
		int min,max,default_value,step;
	};

	class DeviceInfo
	{
	public:
		DeviceInfo();
		~DeviceInfo();

		const VideoInfoEnumeration & get_video_info_enumeration()const;
		void set_video_info_enumeration(const VideoInfoEnumeration & video_info_enumeration_);

		const ModelInfo & get_model_info()const;
		void set_model_info(ModelInfo & model_info_);
		bool supports_format(int format_)const;
		Shape get_highest_resolution(int format_)const;

		const ControlInfo& get_gain_info() const;
		const ControlInfo& get_exposure_info() const;
		const ControlInfo& get_focus_info() const;

		ControlInfo& get_gain_info();
		ControlInfo& get_exposure_info();
		ControlInfo& get_focus_info();

	private:
		ModelInfo _model_info;
		VideoInfoEnumeration _video_info_enumeration;

		ControlInfo _control_gain;
		ControlInfo _control_exposure;
		ControlInfo _control_focus;
	};
}