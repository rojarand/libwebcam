/*
 * linenumerator.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: zebul
 */

#ifndef WIN32

#include <dirent.h>
#include <cstring>
#include <fcntl.h>//O_RDONLY
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <algorithm>
#include <unistd.h>//close

#include <iostream>


#include "lin_enumerator.h"

namespace webcam {

template <typename ENUMRABLE_PROPERTY>
class property_enumerator
{
public:
	property_enumerator(ENUMRABLE_PROPERTY & enumerable_property_, int command_)
	:_enumerable_property(enumerable_property_)
	,_command(command_)
	{
		memset(&_enumerable_property, 0, sizeof(_enumerable_property));
	}

	bool next(int fd_)
	{
		int result = ioctl(fd_, _command, &_enumerable_property );
		_enumerable_property.index++;
		return (0 <= result);
	}
private:
	ENUMRABLE_PROPERTY & _enumerable_property;
	int _command;
};

enumerator::enumerator() {

}

enumerator::~enumerator() {

}

static bool is_video_device(const char * name)
{
	std::string::size_type pos = std::string(name).find("video");
	return pos != std::string::npos;
}

DeviceInfoEnumeration enumerator::enumerate()
{
	DeviceInfoEnumeration device_enumeration;

	DIR * dp = opendir("/dev");
	if (dp == nullptr) {
		return device_enumeration;
	}

	std::vector<std::string> files;
	struct dirent * ep = nullptr;
	while( (ep = readdir(dp)) )
	{
		if (is_video_device(ep->d_name))
		{
			files.push_back(std::string("/dev/") + ep->d_name);
		}
	}
	closedir(dp);

	std::sort(files.begin(), files.end());

	for(const std::string & device_file: files){

		int fd = open(device_file.c_str(), O_RDONLY);
		if(ERROR != fd){

			webcam::DeviceInfo device_info;

			ModelInfo model_info = get_model_info(fd);
			device_info.set_model_info(model_info);

			VideoInfoEnumeration video_enumeration = get_video_info_enumeration(fd);
			device_info.set_video_info_enumeration(video_enumeration);

			device_enumeration.put(device_info);
			close(fd);
		}
	}
	return device_enumeration;

}

ModelInfo enumerator::get_model_info(int fd_)
{
	struct v4l2_capability video_cap;
	int res = ioctl(fd_, VIDIOC_QUERYCAP, &video_cap);
	if(ERROR != res)
	{
		return webcam::ModelInfo(1, (const char*)video_cap.card);
	}
	return webcam::ModelInfo();
}

VideoInfoEnumeration enumerator::get_video_info_enumeration(int fd_)
{
	VideoInfoEnumeration enumeration;

	struct v4l2_fmtdesc vid_fmtdesc;    // Enumerated video formats supported by the device
	property_enumerator<v4l2_fmtdesc> fmt_enum(vid_fmtdesc, VIDIOC_ENUM_FMT);
	vid_fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while( fmt_enum.next(fd_) )
	{
		const Format * format = create_format(vid_fmtdesc.pixelformat);
		std::vector<webcam::Resolution> resolutions = get_resolutions(fd_, vid_fmtdesc.pixelformat);
		for(const webcam::Resolution & resolution: resolutions)
		{
			webcam::VideoInfo video_info(resolution, *format, 0);
			enumeration.put(video_info);
		}
		delete format;
	}
	return enumeration;
}

std::vector<webcam::Resolution> enumerator::get_resolutions(int fd_, unsigned int pixelformat_)
{
	std::vector<webcam::Resolution> resolutions;
	struct v4l2_frmsizeenum frmsize;
	struct v4l2_frmivalenum frmival;
	memset(&frmival, 0, sizeof(frmival));

	property_enumerator<v4l2_frmsizeenum> frmsize_enum(frmsize, VIDIOC_ENUM_FRAMESIZES);
	frmsize.pixel_format = pixelformat_;
	frmsize.index = 0;
	while(frmsize_enum.next(fd_)) {

		if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {

			unsigned int width = frmsize.discrete.width;
			unsigned int height = frmsize.discrete.height;

			property_enumerator<v4l2_frmivalenum> frmival_enum(frmival, VIDIOC_ENUM_FRAMEINTERVALS);
			frmival.pixel_format = pixelformat_;
			frmival.width = width;
			frmival.height = height;

			webcam::Resolution resolution(width, height);
			resolutions.push_back(resolution);

			while( frmival_enum.next(fd_) ) {
				//frmival contains frame rating
			}
		}
	}
	return resolutions;
}

} /* namespace core */

#endif
