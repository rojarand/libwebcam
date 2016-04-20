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
#include "../../webcam_impl.h"
#include "../../../error/webcam_exception.h"
#include "../../../info/device_info.h"
#include "../../../utils/buffer.h"

namespace webcam
{

class lin_webcam_impl: public webcam_impl
{
public:
	lin_webcam_impl(unsigned char camera_number_, device & webcam_);
	virtual ~lin_webcam_impl();

	virtual void open();
	virtual void close();
	virtual image * read();
	virtual void release_safe();
protected:
	void do_open();
	void set_video_settings();
	void map_memory();
	void unmap_memory();
	void do_close();

	void wait_for_image();
	image * create_image();

	void enum_formats(std::vector<int> & pix_fmt_);
	int xioctl(int request_, void * argp_);
	int						_fd;
	std::vector<buffer*>	_buffers;
};

}

#endif /* WIN32 */

#endif /* LIN_WEB_CAM_H_ */
