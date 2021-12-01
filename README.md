# Quick Build Instructions

sudo apt install libjpeg-dev
export JAVA_HOME=/path/to/jdk
cd libwebcan-jni
../gradlew pTML
cd ..
./build_release.sh

# libwebcam
## Lightweigh C++ webcam library

### What it does
Provides access to your USB webcam using object oriented approach. 

### Main features:
* Image acquisition
* Webcam enumeration
* C++ 11 compliance
* Built on top of Video4linux
* Ease of use

## Requirements C++

* libjpeg-dev

## Requirements Java

* Java JDK (Only if you wish to use Java)

To use the Java code it's recommend that you install the Oracle JDK 1.8.
The open source JDK 1.8 is missing encryption keys that are required by Gradle.
This is fixed in future versions of the JDK. It should compile in JDK 11 but not tested.

https://websiteforstudents.com/how-to-install-oracle-java-jdk8-on-ubuntu-16-04-17-10-18-04-desktops/

If this is your first don't forget to set up your java environmental variables.
```commandline
echo "export JAVA_HOME=/usr/lib/jvm/java-8-oracle/" >> ~/.bashrc
echo "export JAVA_INCLUDE_PATH=/usr/lib/jvm/java-8-oracle/include/" >> ~/.bashrc
```
You should now log out and log back in to be safe.

## Building

```commandline
cd libwebcam
mkdir build;cd build
cmake ..
make -j8
```

## Example C++

Capture Images

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

Controls
```c++
#include <libwebcam/webcam.h>
...
int main()
{
	try{
		webcam::video_settings video_settings;
		video_settings.set_format(webcam::format_JPEG());
		video_settings.set_fps(6);
		video_settings.set_resolution(webcam::resolution(320, 240));

		const webcam::DeviceInfo & device_info = device.get_device_info();

		const webcam::ControlInfo& info = device_info.get_exposure_info();
		std::cout << "min="<<info.min<<" max="<<info.max<<" default="<<info.default_value<<endl;

		unsigned char device_number = 1;
		webcam::device device(device_number, video_settings);
		device.open();

        // manual exposure at smallest value
        device.set_exposure(false,info.min);
        
        .... read images same as above

    catch (const webcam::webcam_exception & exc_){
		std::cout<<exc_.what()<<std::endl;
	}
	return 0;
}

```


## Example Java 

```java
import libwebcam.WebcamDriver;

public class ExampleLibWebcam {
    public static void main(String[] args) throws IOException {
        WebcamDriver webcam = new WebcamDriver();
        
        if( !webcam.open(4000,2000) ) {
            throw new RuntimeException("Can't open webcam");
        }
        
        InterleavedU8 color = new InterleavedU8(1, 1, 3);
        
        for(int i = 0; i < 100; i++) {
            if (!webcam.capture(color)) {
                throw new RuntimeException("Capture Failed");
            }
            
            // do something with the image
        }
    }
}
```


### More info
How to use, install and examples may be found on [project site]

### License
MIT © [Robert Andrzejczyk]
forked by [Peter Abeles]
### Contribute
Contributions are always welcome!

[project site]: http://rojarand.github.io/libwebcam
[Robert Andrzejczyk]: https://github.com/rojarand
[Peter Abeles]: https://github.com/lessthanoptimal
