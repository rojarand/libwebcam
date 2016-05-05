#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <libwebcam/webcam.h>

#ifdef WIN32

#include <windows.h>

#else

#define BYTE  unsigned char
#define DWORD  unsigned int
#define LONG  int
#define UINT  unsigned int
#define WORD  unsigned short int

#define LPSTR  char*
typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	DWORD  biSize;
	LONG   biWidth;
	LONG   biHeight;
	WORD   biPlanes;
	WORD   biBitCount;
	DWORD  biCompression;
	DWORD  biSizeImage;
	LONG   biXPelsPerMeter;
	LONG   biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#endif

webcam::buffer create_bitmap_buffer(webcam::image * image_)
{
	const webcam::video_settings & video_settings = image_->get_video_settings();
	const webcam::resolution & resolution = video_settings.get_resolution();
	
	BITMAPINFOHEADER bih;
	memset(&(bih), 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biWidth = resolution.get_width();
	bih.biHeight = resolution.get_height();
	bih.biPlanes = 1;
	bih.biBitCount = 24;

	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(bfh));
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof(bih)+sizeof(bfh)+image_->get_data_lenght();
	bfh.bfOffBits = sizeof(bih)+sizeof(bfh);

	const int len = bfh.bfSize;
	unsigned char * buf = new unsigned char[len];
	int offset = 0;
	memcpy(buf + offset, (const char*)&bfh, sizeof(bfh));
	offset += sizeof(bfh);
	memcpy(buf + offset, (const char*)&bih, sizeof(bih));
	offset += sizeof(bih);
	memcpy(buf + offset, (const char*)image_->get_data(), image_->get_data_lenght());

	return webcam::buffer(buf, len);
}

void save_bitmap_to_file(const webcam::buffer & buffer_, unsigned int image_index_)
{
	std::stringstream file_name_stream;
	file_name_stream << "image" << std::setfill('0') << std::setw(5) << image_index_ << ".bmp";
	std::string file_name = file_name_stream.str();

	std::ofstream image_file_stream;
	image_file_stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

	image_file_stream.write((const char*)buffer_.get_data(), buffer_.get_length());
	image_file_stream.close();
}

void write_images(const int image_count_, webcam::device & device_){

	for (int image_index = 0; image_index < image_count_; ){

		webcam::image * image = device_.read();
		if (!image->is_empty()){

			webcam::buffer buffer = create_bitmap_buffer(image);
			std::cout << "writing bitmap no: " << image_index << std::endl;
			save_bitmap_to_file(buffer, image_index++);
		}
		delete image;
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
		//force bitmap as output format - it may throw exception under linux
		video_settings.set_format(webcam::format_BMP24());
		video_settings.set_resolution(video_info.get_resolution());
		video_settings.set_fps(10);

		webcam::device device(device_number, video_settings);
		std::cout << "Trying to open device with number: " << (int)device_number << ", fps:" << (int)video_settings.get_fps() << std::endl;
		device.open();
		write_images(100, device);
		device.close();
	}
	catch (const webcam::webcam_exception & exc_)
	{
		std::cout << "error occured: " << exc_.what() << std::endl;
	}
	return 0;
}
