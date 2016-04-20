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
#include "../image/video_settings.h"
#include "../image/image.h"
#include "../info/device_info.h"

namespace webcam
{

	class state;
	class webcam_impl;

	class device {
	public:
		friend class state;
		friend class webcam_impl;
		device(unsigned char number_, const video_settings & video_settings_);
		virtual ~device();
		
		virtual void open();
		virtual void close();
		virtual image * read();

		unsigned char get_number()const;
		const video_settings & get_video_settings()const;
		void set_video_settings(const video_settings & video_settings_);
		bool is_resolution_adjustable()const;
		bool is_quality_adjustable()const;
		bool is_open()const;
	protected:
		void set_state(state  * state_);
		void do_set_video_settings(const video_settings & video_settings_);
		state * _state;
		video_settings _video_settings;
		webcam_impl * _impl;
	};

}
#endif /* WEB_CAM_H_ */
