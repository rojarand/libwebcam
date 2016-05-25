# libwebcam
## Lightweigh C++ webcam library

### What it does
Provides access to your USB webcam using object oriented aproach. 

### Main features:
* Image acquisition
* Webcam enumeration
* C++ 11 compliance
* Portablity (Windows, Linux)
* Built on top of Video4linux (Linux) and DirectShow (Windows)
* Ease of use

Usage example:
```c++
...
#include <libwebcam/webcam.h>
...
int main()
{
    try{
        webcam::video_settings video_settings;
		video_settings.set_format(webcam::format_JPEG());
		video_settings.set_fps(6);
		video_settings.set_resolution(webcam::resolution(320, 240));
		
		unsigned char device_number = 1;
		webcam::device device(device_number, video_settings);
		device.open();
		for(int i=0; i<100; i++){
		    webcam::image * image = device.read();
		    //do somethig with image
		    delete image;
		}
		device.close();
    }
    catch (const webcam::webcam_exception & exc_){
		std::cout<<exc_.what()<<std::endl;
	}
	return 0;
}
```
### How to use libwebcam
* Install source using [installation guide]
* Include header `#include <libwebcam/webcam.h>`
* Enumerate for available webcams
* Setup webcam::video_settings using enumerated video parameters
* Create instance of webcam::device passing number of device and video_settings (number starts from 1)
* Open device, read images, close device

You can also find usage examples in libwebcam/examples directory 

### License
MIT © [Robert Andrzejczyk]
# TODO
### Contribute
Contributions are always welcome!
### Contact

[installation guide]: http://rojarand.github.io/libwebcam
[Robert Andrzejczyk]: https://github.com/rojarand


```c++
#include <libwebcam/webcam.h>
...
int main(){
  //check if there is at least one webcam connected to the computer
  const webcam::device_info_enumeration enumeration = webcam::enumerator::enumerate();
  const size_t count = enumeration.count();
  if (count == 0){
    std::cout << "There is no webcam available on this computer" << std::endl;
    return 0;
  }

  const webcam::device_info & device_info = enumeration.get(0);
  const webcam::video_info_enumeration & video_info_enumeration =
    device_info.get_video_info_enumeration();
  const webcam::video_info & video_info = video_info_enumeration.get(0);

  //setup video_settings
  webcam::video_settings video_settings;
  video_settings.set_format(video_info.get_format());
  video_settings.set_resolution(video_info.get_resolution());
  video_settings.set_fps(1);

  const unsigned char device_number = 1;
  webcam::device device(device_number, video_settings);
  device.open();
  while(...){
    webcam::image * image = device.read();
    ...
    delete image;
  }
  device.close();
}
```
