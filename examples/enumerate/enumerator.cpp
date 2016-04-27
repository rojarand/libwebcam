#include <iostream>
#include <libwebcam/webcam.h>

int main()
{
	webcam::device_info_enumeration enumeration = webcam::enumerator::enumerate();
	size_t count = enumeration.count();
	std::cout<<"Count"<<count<<std::endl;
	return 0;
}