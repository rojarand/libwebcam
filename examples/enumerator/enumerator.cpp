#include <iostream>
#include <libwebcam/webcam.h>

int main(int argc, char **argv)
{
	try
	{
		const webcam::device_info_enumeration & enumeration = webcam::enumerator::enumerate();
		const size_t count = enumeration.count();
		if(count == 0){
		
			std::cout << "There is no webcam available on this computer"<< std::endl;
			return 0;
		}
		
		for (size_t device_index = 0; device_index < count; device_index++){

			const webcam::device_info & device_info = enumeration.get(device_index);
			
			//print name
			std::cout<<"Webcam no. "<<(device_index+1)<<":"<<std::endl;
			const webcam::model_info & model_info = device_info.get_model_info();
			std::cout<<"name: "<<model_info.get_name()<<std::endl;
			std::cout<<"friendly_name: "<<model_info.get_friendly_name()<<std::endl;
			
			//print vendor info
			const webcam::vendor_info & vendor_info = model_info.get_vendor_info();
			std::cout<<"vendor: "<<vendor_info.get_name()<<std::endl;
			
			//print image details
			const webcam::video_info_enumeration & video_info_enumeration = device_info.get_video_info_enumeration();
			size_t video_count = video_info_enumeration.count();
			for (size_t video_index = 0; video_index < video_count; video_index++){
				const webcam::video_info & video_info = video_info_enumeration.get(video_index);

				std::cout << "----------------------------" << std::endl;
				std::cout << "\tformat: " << video_info.get_format().get_name() << std::endl;
				const webcam::resolution & resolution = video_info.get_resolution();
				std::cout << "\twidth: " << resolution.get_width() << ", height: " << resolution.get_height() << std::endl;
			}
			std::cout << std::endl << std::endl;
		}
	}
	catch(const webcam::enumerator_exception & exc_)
	{
		std::cout<<exc_.what()<<std::endl;
	}
	return 0;
}
