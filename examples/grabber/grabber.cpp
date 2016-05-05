#include <iostream>
#include <chrono>
#include <libwebcam/webcam.h>

void grab(int image_number_, webcam::device & device_)
{
	//read image
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	webcam::image * image = device_.read();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	//print image info
	const webcam::video_settings & video_settings = image->get_video_settings();
	std::string fmt_name = video_settings.get_format().get_name();
	long long duration_in_millisec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "img [" << image_number_ << "], fmt: " << fmt_name <<
		", size: "<<image->get_data_lenght()<<
		", read took: " << duration_in_millisec << " milliseconds." << std::endl;
	delete image;
}

void grab_for(const std::chrono::milliseconds & total_duration_, webcam::device & device_)
{
	
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int image_number = 0;
	while (true)
	{
		grab(image_number++, device_);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::chrono::milliseconds current_duration = 
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		if (total_duration_ < current_duration)
		{
			break;
		}
	}
}

int main(int argc, char **argv)
{
	try
	{
		const webcam::device_info_enumeration & enumeration = webcam::enumerator::enumerate();
		const size_t count = enumeration.count();
		if (count == 0){

			std::cout << "There is no webcam available on this computer" << std::endl;
			return 0;
		}

		const webcam::device_info & device_info = enumeration.get(0);
		const webcam::video_info_enumeration & video_info_enumeration = device_info.get_video_info_enumeration();
		const webcam::video_info & video_info = video_info_enumeration.get(0);
		
		const unsigned char device_number = 1;

		webcam::video_settings video_settings;
		video_settings.set_format(video_info.get_format());
		video_settings.set_resolution(video_info.get_resolution());
		video_settings.set_fps(10);
		
		webcam::device device(device_number, video_settings);
		std::cout<<"Trying to open device with number: "<<(int)device_number<<", fps:"<<(int)video_settings.get_fps()<<std::endl;
		device.open();
		grab_for(std::chrono::milliseconds(5000), device);
		device.close();
	}
	catch(const webcam::webcam_exception & exc_)
	{
		std::cout<<"error occured: "<<exc_.what()<<std::endl;
	}
	return 0;
}

