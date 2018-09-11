/*
 * device.h
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#ifndef WEB_CAM_H_
#define WEB_CAM_H_

#include <map>
#include "../error/webcam_exception.h"
#include "image/VideoSettings.h"
#include "image/Image.h"
#include "info/DeviceInfo.h"

namespace webcam
{
	class state;
	class WebcamImpl;

	class Device {
	public:
		friend class state;
		friend class WebcamImpl;

		Device(unsigned char number_, const VideoSettings & video_settings_,
		        const DeviceInfo &device_info );
		virtual ~Device();
		
		virtual void open();
		virtual void close();
		virtual Image * read();

		unsigned char get_number()const;
		const VideoSettings & get_video_settings()const;
		const DeviceInfo& get_device_info() const;

		void set_video_settings(const VideoSettings & video_settings_);
		bool is_resolution_adjustable()const;
		bool is_quality_adjustable()const;
		bool is_open()const;

		void set_focus( bool automatic , int value );
        void set_exposure( bool automatic , int value );
        void set_gain(  bool automatic , int value );

	protected:
		void set_state(state  * state_);
		void do_set_video_settings(const VideoSettings & video_settings_);
		state * _state;
		VideoSettings _video_settings;
		WebcamImpl * _impl;
		const DeviceInfo& _device_info;
	};

}
#endif /* WEB_CAM_H_ */
