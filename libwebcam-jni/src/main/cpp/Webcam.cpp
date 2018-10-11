#include "Webcam.h"
#include <libwebcam/webcam.h>
#include <libwebcam/info/DeviceInfo.h>
#include <libwebcam/info/VideoInfo.h>
#include <stdexcept>
#include <iostream>

using namespace std;

extern "C" {

bool camera_open = false;
webcam::DeviceInfo device_info;
webcam::VideoSettings video_settings;
webcam::Resolution video_resolution;
std::string video_format = "";
size_t selected_device = 99999;
std::string error_message = "";
webcam::Device *device = NULL;

jclass safe_FindClass( JNIEnv *env, const char* name )
{
    jclass theclass = env->FindClass(name);
    if( env->ExceptionCheck() || theclass == nullptr ) {
        env->ExceptionDescribe();
        throw "failed FindClass";
    }
    return theclass;
}

jmethodID safe_GetMethodID( JNIEnv *env, jclass& objClass, const char* name , const char* type)
{
    jmethodID fid = env->GetMethodID(objClass, name, type);
    if( env->ExceptionCheck() ) {
        env->ExceptionDescribe();
        throw "failed GetMethodID";
    }
    return fid;
}

//JNIEXPORT void JNICALL Java_libwebcam_Webcam_lookupDevices(JNIEnv *env, jclass obj, jobject devices)
//{
//    try
//    {
//        jclass cls_dinfo = safe_FindClass( env,  "libwebcam/DeviceInfo");
//        jmethodID constructor_vinfo = safe_GetMethodID(env, cls_dinfo, "<init>", "()V");
//
//        jclass cls_list = safe_FindClass( env,  "java/util/List");
//        jmethodID list_add = safe_GetMethodID(env, cls_list, "add", "(Ljava/lang/Object;)Z");
//
//        const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();
//
//        const size_t count = enumeration.count();
//
//        for (size_t device_index = 0; device_index < count; device_index++) {
//            const webcam::DeviceInfo & device_info = enumeration.get(device_index);
//
//            jobject jinfo = env->NewObject(cls_dinfo,constructor_vinfo);
//
//            env->CallObjectMethod(devices,list_add,jinfo);
//        }
//
//    }
//    catch(const webcam::enumerator_exception & exc_)
//    {
//        std::cout<<exc_.what()<<std::endl;
//    }
//}

bool select_resolution( const webcam::DeviceInfo & device_info , int width , int height ,
                        webcam::Resolution& selected , std::string &format )
{
    const webcam::VideoInfoEnumeration & video_info_enumeration =
                        device_info.get_video_info_enumeration();
    size_t video_count = video_info_enumeration.count();

    int best_score = -1;

    for (size_t video_index = 0; video_index < video_count; video_index++) {
        const webcam::VideoInfo &video_info = video_info_enumeration.get(video_index);
        std::string format_name = video_info.get_format().get_name();
        if( format_name != "MJPG")
            continue;
        const webcam::Resolution &resolution = video_info.get_resolution();
        int dw = width-resolution.get_width();
        int dh = height-resolution.get_height();
        int score = dw*dw + dh*dh;

        if( best_score == -1 || best_score > score ) {
            best_score = score;
            selected = resolution;
            format = format_name;
        }
    }

    return best_score != -1;
}

JNIEXPORT jboolean JNICALL Java_libwebcam_WebcamDriver_open
  (JNIEnv *env, jobject obj, jint width, jint height) {

  cout << "ENTER Java_libwebcam_WebcamDriver_open()" << endl;
    if( !camera_open ) {
        cout << " camera not open already" << endl;

      // Pick a webcam to open. The first one is a great choice!
      try {
          const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();
          const size_t count = enumeration.count();

        cout << " searching devices " << count << endl;

          if( count > 1 ) {
            std::cout << "Multiple cameras to choose from. First first." << endl;
          } else if( count == 0 ) {
            error_message = "No cameras found";
            return false;
          }
          device_info = enumeration.get(0);
          selected_device = 1; // 0 + 1
      } catch(const webcam::enumerator_exception & exc_) {
          error_message = exc_.what();
          std::cout<<exc_.what()<<std::endl;
          return false;
      }

        cout << " selecting resolution" << endl;
      if( !select_resolution( device_info,width,height,video_resolution,video_format) ) {
        error_message = "failed to select resolution";
        return false;
      }
    } else {
       // see if the resolution has changed
       webcam::Resolution selected;
       std::string format;
       if( !select_resolution( device_info,width,height,selected,format) ) {
          error_message = "failed to select resolution";
          return false;
       }
       if( selected.get_width() == video_resolution.get_width() &&
           selected.get_height() == video_resolution.get_height() )
       {
         return true;
       }

       // change camera resolution
       video_resolution = selected;
       video_format = format;

        device->close();
        delete device;
        device = NULL;
    }

    cout << " configuring settings" << endl;
    webcam::Format *format = webcam::create_format(video_format);
    webcam::VideoSettings video_settings;
    video_settings.set_format(*format);
    video_settings.set_fps(500);
    video_settings.set_resolution(video_resolution);

    delete format;
    cout << " opening" << endl;

    try {
        device = new webcam::Device(selected_device,video_settings,device_info);
        device->open();
        camera_open = true;
        return true;
    } catch( webcam::webcam_exception &e ) {
        delete device;
        device = NULL;
        error_message = e.what();
        std::cout << "FAILURE: " << e.get_details() << std::endl;
        return false;
    }
  }

JNIEXPORT jboolean JNICALL Java_libwebcam_WebcamDriver_close
  (JNIEnv *env, jobject obj) {
  if( !camera_open )
      return true;

  device->close();
  delete device;
  device = NULL;
  return true;
}


JNIEXPORT jboolean JNICALL Java_libwebcam_WebcamDriver_isOpen
  (JNIEnv *env, jobject obj) {
    return camera_open;
  }

JNIEXPORT jboolean JNICALL Java_libwebcam_WebcamDriver_capture
  (JNIEnv *env, jobject obj, jbyteArray jdata, jint jdata_length) {
    if( !camera_open ) {
        error_message = "camera not open";
        return false;
    }
    webcam::Image *image = device->read();
    const unsigned char * data = image->get_data();
    unsigned int length = image->get_data_length();
    cout << "  image data length " << length << endl;

    delete image;

    return true;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_imageWidth
  (JNIEnv *env, jobject obj) {
    return video_resolution.get_width();
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_imageHeight
  (JNIEnv *env, jobject obj) {
    return video_resolution.get_height();
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_imageBands
  (JNIEnv *env, jobject obj) {
    return 3;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_readExposure
  (JNIEnv *env, jobject obj, jobject) {
    return 0;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_readGain
  (JNIEnv *env, jobject obj, jobject) {
    return 0;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_readFocus
  (JNIEnv *env, jobject obj, jobject) {
    return 0;
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setExposure
  (JNIEnv *env, jobject obj, jboolean manual, jint value) {
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setGain
  (JNIEnv *env, jobject, jboolean manual, jint value) {
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setFocus
  (JNIEnv *env, jobject, jboolean manual, jint value) {
  }

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_errorMessage
  (JNIEnv *env, jobject) {
    return env->NewStringUTF(error_message.c_str());
  }

}