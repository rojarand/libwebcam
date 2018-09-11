/*
 * lin_web_cam.h
 *
 *  Created on: Feb 11, 2013
 *      Author: zebul
 */

#ifndef LIN_WEB_CAM_IMPL_H_
#define LIN_WEB_CAM_IMPL_H_

#ifndef WIN32

#include <vector>
#include "device/WebcamImpl.h"
#include "../../../error/webcam_exception.h"
#include "info/DeviceInfo.h"
#include "utils/Buffer.h"

namespace webcam
{

class lin_webcam_impl: public WebcamImpl
{
public:
	lin_webcam_impl(unsigned char camera_number_, Device & webcam_);
	virtual ~lin_webcam_impl();

	virtual void open();
	virtual void close();
	virtual Image * read();
	virtual void release_safe();
protected:
	void do_open();
	void set_video_settings();
	void map_memory();
	void unmap_memory();
	void do_close();

	void wait_for_image();
	Image * create_image();

	void enum_formats(std::vector<int> & pix_fmt_);
	int xioctl(int request_, void * argp_);
	int						_fd;
	std::vector<Buffer*>	_buffers;
};

}

#endif /* WIN32 */

#endif /* LIN_WEB_CAM_H_ */
