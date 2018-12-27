/*
 * linenumerator.h
 *
 *  Created on: Oct 25, 2015
 *      Author: zebul
 */

#pragma once

#ifndef WIN32

#include "info/DeviceInfoEnumeration.h"

namespace webcam {

enum
{
	ERROR = -1,
};

class enumerator {
public:
	enumerator();
	virtual ~enumerator();
	static DeviceInfoEnumeration enumerate();
private:
	static ModelInfo get_model_info(int fd_);
	static VideoInfoEnumeration get_video_info_enumeration(int fd_);
	static Resolutions get_resolutions(int fd_, unsigned int pixelformat_);
	static int get_automatic_controls(int fd, __u32 control, ControlInfo &info);
	static int get_manual_controls(int fd, __u32 control, ControlInfo &info);
};

} /* namespace core */
#endif 
