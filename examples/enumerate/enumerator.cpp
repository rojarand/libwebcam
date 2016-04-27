#include <iostream>
#include <libwebcam/webcam.h>

int main(int argc, char **argv)
{
	const webcam::device_info_enumeration & enumeration = webcam::enumerator::enumerate();
	const size_t count = enumeration.count();
	for (size_t device_index = 0; device_index < count; device_index++){

		const webcam::device_info & device_info = enumeration.get(device_index);
		
		//print model name
		std::cout << device_info.get_model_info().get_name() << std::endl;
		
		//print image details
		const webcam::video_info_enumeration & video_info_enumeration = device_info.get_video_info_enumeration();
		size_t video_count = video_info_enumeration.count();
		for (size_t video_index = 0; video_index < video_count; video_index++){
			const webcam::video_info & video_info = video_info_enumeration.get(video_index);

			std::cout << "\tformat: " << video_info.get_format().get_name() << std::endl;
			const webcam::resolution & resolution = video_info.get_resolution();
			std::cout << "\twidth: " << resolution.get_width() << ", height: " << resolution.get_height() << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
	return 0;
}
