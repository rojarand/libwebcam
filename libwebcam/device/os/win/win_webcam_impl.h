/*
 * win_web_cam.h
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#ifndef WIN_WEB_CAM_IMPL_H_
#define WIN_WEB_CAM_IMPL_H_

#ifdef WIN32

#include <dshow.h>
#include <Mtype.h>
#include "qedit.h"
#include "grabber.h"
#include "../../webcam_impl.h"

namespace webcam
{

	class win_webcam_impl : public webcam_impl
	{
	public:
		win_webcam_impl(unsigned char camera_number_, device & webcam_);
		virtual ~win_webcam_impl();

		virtual void open();
		virtual void close();
		virtual image * read();
		virtual void apply_traits();
		virtual void release_safe();
	private:
		void initialize();
		IGraphBuilder * create_graph_builder();
		ICaptureGraphBuilder2 * create_capture_graph_builder2();
		IMediaControl * create_media_control();
		IBaseFilter * create_base_filter_source();
		IBaseFilter * create_base_filter_grabber();
		ISampleGrabber * create_sample_grabber();
		void add_filter(IBaseFilter * base_filter_);

		void apply_video_settings();
		void connect_pins();
		void render();
		void set_up_sample_grabber();
		void run();

		void throw_webcam_exception(const char * what_, int err_no_ = S_OK);

		static void setup_media_type(AM_MEDIA_TYPE * pmt, const video_settings & video_settings_);

		grabber _grabber;
		IGraphBuilder * _graph_builder;
		ICaptureGraphBuilder2 * _capture_graph_builder2;
		IMediaControl * _media_control;

		IBaseFilter * _base_filter_source;
		IBaseFilter * _base_filter_grabber;
		ISampleGrabber * _sample_grabber;
	};
}
#endif /* WIN32 */

#endif /* WIN_WEB_CAM_H_ */
