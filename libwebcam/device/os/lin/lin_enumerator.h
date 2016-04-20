/*
 * linenumerator.h
 *
 *  Created on: Oct 25, 2015
 *      Author: zebul
 */

#pragma once

#ifndef WIN32

#include "../../../info/device_info_enumeration.h"

namespace webcam {

enum
{
	ERROR = -1,
};

class enumerator {
public:
	enumerator();
	virtual ~enumerator();
	static device_info_enumeration enumerate();
private:
	static model_info get_model_info(int fd_);
	static video_info_enumeration get_video_info_enumeration(int fd_);
	static std::vector<resolution> get_resolutions(int fd_, unsigned int pixelformat_);
};

} /* namespace core */
#endif 
