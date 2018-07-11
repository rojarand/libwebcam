/*
 * lin_webcam_impl.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: zebul
 */

#ifndef WIN32

#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <algorithm>

#include "lin_webcam_impl.h"

namespace webcam
{

	lin_webcam_impl::lin_webcam_impl(unsigned char camera_number_, device & webcam_)
		:webcam_impl(camera_number_, webcam_)
	{
		_fd					= -1;
	}

	lin_webcam_impl::~lin_webcam_impl() {
	}

	void lin_webcam_impl::open()
	{
		do_open();
		set_video_settings();
		map_memory();
	}

	image * lin_webcam_impl::read()
	{
		wait_for_image();
		return create_image();
	}

	void lin_webcam_impl::close()
	{
		unmap_memory();
		do_close();
	}

	void lin_webcam_impl::do_open()
	{
		std::stringstream str;
		str<<"/dev/video"<<int(_camera_number-1);
		std::string device_file = str.str();

		//open----------------------------
		struct stat st;
		// stat file
		if (FAILURE == ::stat(device_file.c_str(), &st))
		{
			throw webcam_exception("can not stats device file", errno, _camera_number);
		}

		// check if its device
		if (!S_ISCHR (st.st_mode))
		{
			throw webcam_exception("not device file", errno, _camera_number);
		}
		// open device
		_fd = ::open(device_file.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);

		// check if opening was successfull
		if (FAILURE == _fd)
		{
			throw webcam_exception("can not open", errno, _camera_number);
		}

		//init-------------------------------------------
		struct v4l2_capability cap;
		if (-1 == xioctl(VIDIOC_QUERYCAP, &cap))
		{
			if (EINVAL == errno)
			{
				throw webcam_exception("no V4L2 device", errno, _camera_number);
			}
			else
			{
				throw webcam_exception( "can not query device capabilities", errno, _camera_number);
			}
		}

		if ( !(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) )
		{
			throw webcam_exception("device has no video capture capabilities", SUCCESS, _camera_number);
		}

		if ( !(cap.capabilities & V4L2_CAP_STREAMING) )
		{
			throw webcam_exception("device does not support i/o streaming", SUCCESS, _camera_number);
		}

		struct v4l2_cropcap cropcap;
		struct v4l2_crop crop;

		// Select video input, video standard and tune here.
		memset(&cropcap, 0, sizeof(cropcap));
		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (0 == xioctl(VIDIOC_CROPCAP, &cropcap))
		{
			crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			crop.c = cropcap.defrect; // reset to default
			if (-1 == xioctl(VIDIOC_S_CROP, &crop))
			{
				switch (errno)
				{
				case EINVAL:
					// Cropping not supported.
					break;
				default:
					// Errors ignored.
					break;
				}
			}
		}
	}

	void lin_webcam_impl::set_video_settings()
	{
		v4l2_format			fmt;
		memset(&fmt, 0, sizeof(fmt));

		// v4l2_format
		fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		const video_settings & video_settings = _device.get_video_settings();
		const Format & format = video_settings.get_format();
		fmt.fmt.pix.width       = video_settings.get_width();
		fmt.fmt.pix.height      = video_settings.get_height();
		fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
		fmt.fmt.pix.pixelformat = format.get_native_format();

		if (-1 == xioctl(VIDIOC_S_FMT, &fmt))
		{
			throw webcam_exception("can not set video format", errno, _camera_number);
		}

		bool is_jpeg = (format==Format_JPEG()) || (format==Format_MJPEG());
		if(is_jpeg)
		{
			struct v4l2_jpegcompression jpeg_compression;
			memset(&jpeg_compression, 0, sizeof(jpeg_compression));
			/*int res = */xioctl(VIDIOC_G_JPEGCOMP, &jpeg_compression);
			jpeg_compression.quality = video_settings.get_quality();//20;
			/*res = */xioctl(VIDIOC_S_JPEGCOMP, &jpeg_compression);
			memset(&jpeg_compression, 0, sizeof(jpeg_compression));
			/*res = */xioctl(VIDIOC_G_JPEGCOMP, &jpeg_compression);
		}

		v4l2_streamparm streamparm;
		memset (&streamparm, 0, sizeof (streamparm));
		streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_fract * fract = &streamparm.parm.capture.timeperframe;
		fract->numerator = 1;
		fract->denominator = video_settings.get_fps();
		/*int res = */xioctl(VIDIOC_S_PARM, &streamparm);
	}

	void lin_webcam_impl::map_memory()
	{
		struct v4l2_requestbuffers req;
		memset(&req, 0, sizeof(req));

		req.count               = 4;
		req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory              = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(VIDIOC_REQBUFS, &req))
		{
			if (EINVAL == errno)
			{
				throw webcam_exception( "device does not support memory mapping", errno, _camera_number);
			}
			else
			{
				throw webcam_exception( "can not initiate memory mapping", errno, _camera_number);
			}
		}

		if (req.count < 2)
		{
			throw webcam_exception( "insufficient buffer memory", errno, _camera_number);
		}

		size_t buffer_index = 0;
		for (buffer_index = 0; buffer_index<req.count; ++buffer_index)
		{
			struct v4l2_buffer v4l2_buf;
			memset(&v4l2_buf, 0, sizeof(v4l2_buf));

			v4l2_buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			v4l2_buf.memory      = V4L2_MEMORY_MMAP;
			v4l2_buf.index       = buffer_index;

			if (-1 == xioctl(VIDIOC_QUERYBUF, &v4l2_buf))
			{
				throw webcam_exception( "can not query status of the buffer", errno, _camera_number);
			}

			void * mem_ptr = mmap (NULL , v4l2_buf.length, PROT_READ | PROT_WRITE , MAP_SHARED , _fd, v4l2_buf.m.offset);
			if (MAP_FAILED == mem_ptr)
			{
				throw webcam_exception( "can not map memory", errno, _camera_number);
			}
			unsigned char * mem = reinterpret_cast<unsigned char*>(mem_ptr);
			unsigned int mem_len = v4l2_buf.length;
			buffer * buf = new buffer;
			buf->attach(buffer_data(mem, mem_len));
			_buffers.push_back(buf);
		}

		enum v4l2_buf_type type;
		for (buffer_index = 0; buffer_index<_buffers.size(); ++buffer_index)
		{
			struct v4l2_buffer v4l2_buf;
			memset(&v4l2_buf, 0, sizeof(v4l2_buf));
			v4l2_buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			v4l2_buf.memory      = V4L2_MEMORY_MMAP;
			v4l2_buf.index       = buffer_index;
			if (-1 == xioctl(VIDIOC_QBUF, &v4l2_buf))
			{
				throw webcam_exception( "can not exchange buffer with the driver", errno, _camera_number);
			}
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(VIDIOC_STREAMON, &type))
		{
			throw webcam_exception( "can not start i/o streaming", errno, _camera_number);
		}
	}

	void lin_webcam_impl::release_safe(){

		unmap_memory();
		try
		{
			do_close();
		}
		catch(const webcam_exception & exc)
		{
		}
	}

	void lin_webcam_impl::unmap_memory()
	{
		//delete buffers if are allocated
		for (webcam::buffer * buffer: _buffers)
		{
			buffer_data buffer_data = buffer->detach();
			void * mem_ptr = (void *)buffer_data.get_data();
			int length = (int)buffer_data.get_lenght();
			const int res = ::munmap (mem_ptr, length);
			if (-1 == res)
			{
				//errno_exit("munmap");
			}
			delete buffer;
		}
		_buffers.clear();
	}

	void lin_webcam_impl::do_close()
	{
		if(-1 < _fd)
		{
			const int res = ::close(_fd);
			_fd = -1;
			if ( -1 == res )
			{
				throw webcam_exception("can not close file descriptor", errno, _camera_number);
			}
		}
	}

	void lin_webcam_impl::wait_for_image()
	{
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(_fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		int r = select(_fd + 1, &fds, NULL, NULL, &tv);
		//r = select(1, &fds, NULL, NULL, &tv);
		if (-1 == r)
		{
			if (EINTR == errno)
			{

			}
			else
			{
				throw webcam_exception("can not read data", errno, _camera_number);
			}
		}

		if (0 == r)
		{
			throw webcam_exception( "select timeout", errno, _camera_number);
		}
	}

	image * lin_webcam_impl::create_image()
	{
		struct v4l2_buffer v4l2_buf;

		memset (&v4l2_buf, 0, sizeof(v4l2_buf));

		v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_buf.memory = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(VIDIOC_DQBUF, &v4l2_buf))
		{
			switch (errno)
			{
			case EAGAIN:
				return 0;

			case EIO:
				// Could ignore EIO, see spec

				// fall through
			default:
				throw webcam_exception( "can not exchange buffer with the driver - VIDIOC_DQBUF fail", errno, _camera_number);
			}
		}

		int len = v4l2_buf.bytesused;//copy bytesused before "VIDIOC_QBUF" - it may be reset after call

		if (-1 == xioctl(VIDIOC_QBUF, &v4l2_buf))
		{
			throw webcam_exception( "can not exchange buffer with the driver - VIDIOC_QBUF fail", _camera_number);
		}

		const buffer * buf = _buffers[v4l2_buf.index];

		if(0 < len)
		{
			webcam::image * image = new webcam::image();
			image->copy_deep(buf->get_data(), len, _device.get_video_settings());
			return image;
		}

		return image::create_empty();
	}

	int lin_webcam_impl::xioctl(int request_, void * argp_)
	{
		int r;

		do r = ioctl(_fd, request_, argp_);
		while (-1 == r && EINTR == errno);

		return r;
	}
}

#endif
