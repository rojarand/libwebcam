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
		Device(unsigned char number_, const VideoSettings & video_settings_);
		virtual ~Device();
		
		virtual void open();
		virtual void close();
		virtual Image * read();

		unsigned char get_number()const;
		const VideoSettings & get_video_settings()const;
		void set_video_settings(const VideoSettings & video_settings_);
		bool is_resolution_adjustable()const;
		bool is_quality_adjustable()const;
		bool is_open()const;
	protected:
		void set_state(state  * state_);
		void do_set_video_settings(const VideoSettings & video_settings_);
		state * _state;
		VideoSettings _video_settings;
		WebcamImpl * _impl;
	};

}
#endif /* WEB_CAM_H_ */
