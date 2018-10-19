#include "Webcam.h"
#include <libwebcam/webcam.h>
#include <libwebcam/info/DeviceInfo.h>
#include <libwebcam/info/VideoInfo.h>
#include <stdexcept>
#include <iostream>
#include <jpeglib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <regex>
#include <sstream>

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

struct my_jpeg_error {
    struct jpeg_error_mgr   base;
    jmp_buf                 env;
};

static void my_output_message(struct jpeg_common_struct *com)
{
    struct my_jpeg_error *err = (struct my_jpeg_error *)com->err;
    char buf[JMSG_LENGTH_MAX];

    err->base.format_message(com, buf);
    fprintf(stderr, "JPEG error: %s", buf);
}

static void my_error_exit(struct jpeg_common_struct *com)
{
    struct my_jpeg_error *err = (struct my_jpeg_error *)com->err;

    my_output_message(com);
    longjmp(err->env, 0);
}

static struct jpeg_error_mgr *my_error_mgr(struct my_jpeg_error *err)
{
    jpeg_std_error(&err->base);

    err->base.error_exit = my_error_exit;
    err->base.output_message = my_output_message;

    return &err->base;
}

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
  (JNIEnv *env, jobject obj, jint width, jint height, jstring jdevice_name ) {

    bool success = true;
    regex device_regex;
    std::string str_regex = ".*";
    if( jdevice_name != NULL ) {
        const char* device_name = env->GetStringUTFChars(jdevice_name,0);
        str_regex = device_name;
        env->ReleaseStringUTFChars(jdevice_name,device_name);
    }
    device_regex.assign(str_regex);

    // if a specific device is requested, the camera is open, and the request doesn't match
    // the current camera, close the open camera
    if( jdevice_name != NULL && camera_open && !regex_match(device_info.get_model_info().get_name(), device_regex) ) {
        cout << "device name changed" << endl;
        device->close();
        delete device;
        device = NULL;
        camera_open = false;
    }

//  cout << "ENTER Java_libwebcam_WebcamDriver_open()" << endl;
    if( !camera_open ) {
//        cout << " camera not open already" << endl;

      // Pick a webcam to open. The first one is a great choice!
      try {
          const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();
          const size_t count = enumeration.count();

//        cout << " searching devices " << count << endl;

          if( count > 1 ) {
            std::cout << "Multiple cameras to choose from. Selecting first matching. regex='" << str_regex << "`" << endl;
          } else if( count == 0 ) {
            error_message = "No cameras found";
            return false;
          }

          bool matched = false;
          for( int deviceIdx = 0; deviceIdx < count; deviceIdx++ ) {
              device_info = enumeration.get(deviceIdx);

              cout << "device found '" << device_info.get_model_info().get_name() << "'" << endl;
              if( regex_match(device_info.get_model_info().get_name(), device_regex) ) {
                  cout << "  matched!" << endl;
                  selected_device = deviceIdx+1;
                  matched = true;
                  break;
              }
          }

          if( !matched ) {
              stringstream ss;
              ss << "No matching device found for "<<str_regex;
              error_message = ss.str();
              return false;
          }
          
      } catch(const webcam::enumerator_exception & exc_) {
          error_message = exc_.what();
          std::cout<<exc_.what()<<std::endl;
          return false;
      }

//        cout << " selecting resolution" << endl;
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

//    cout << " configuring settings" << endl;
    webcam::Format *format = webcam::create_format(video_format);
    webcam::VideoSettings video_settings;
    video_settings.set_format(*format);
    video_settings.set_fps(500);
    video_settings.set_resolution(video_resolution);

    delete format;
//    cout << " opening" << endl;

    try {
        device = new webcam::Device(selected_device,video_settings,device_info);
        device->open();
        camera_open = true;
        return true;
    } catch( webcam::webcam_exception &e ) {
        camera_open = false;
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

    bool success = true;

    char* img_data = (char *)env->GetPrimitiveArrayCritical(jdata, 0);

    webcam::Image *image = device->read();
    unsigned char * data = image->get_data();
    unsigned int length = image->get_data_length();

    struct jpeg_decompress_struct dinfo;
    struct my_jpeg_error err;

    memset(&dinfo, 0, sizeof(dinfo));
    dinfo.err = my_error_mgr(&err);

    if (setjmp(err.env)) {
        success = false;
        goto fail;
    }

    jpeg_create_decompress(&dinfo);

    (void) jpeg_mem_src(&dinfo, data, length);
    (void) jpeg_read_header(&dinfo, TRUE);
    (void) jpeg_start_decompress(&dinfo);

    if( dinfo.output_width != video_resolution.get_width() ) {
        error_message = "Unexpected width in jpeg.";
        goto fail;
    }
    if( dinfo.output_height != video_resolution.get_height() ) {
        error_message = "Unexpected width in jpeg.";
        goto fail;
    }
    if( dinfo.output_components != 3 ) {
        error_message = "Unexpected number of components in jpeg.";
        goto fail;
    }

    for (int y = 0; y < dinfo.output_height; y++) {
        JSAMPROW row_pointer = (JSAMPROW)img_data + y * dinfo.output_width*dinfo.output_components;
        jpeg_read_scanlines(&dinfo, &row_pointer, 1);
    }

    jpeg_finish_decompress(&dinfo);

    fail:
        env->ReleasePrimitiveArrayCritical((jarray)jdata, img_data, JNI_ABORT);
        delete image;
        jpeg_destroy_decompress(&dinfo);
        return success;
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
  (JNIEnv *env, jobject obj, jobject type_enum ) {
    if( !camera_open ) {
      error_message = "camera not open";
      return -1;
    }

    jclass cls_enum = safe_FindClass( env,  "libwebcam/WebcamDriver$ValueType");
    jmethodID jordinal = safe_GetMethodID(env,cls_enum, "ordinal", "()I");
    jint value = env->CallIntMethod(type_enum, jordinal);

    switch( value ) {
        case 0: // min
            return device_info.get_exposure_info().min;
        case 1: // max
            return device_info.get_exposure_info().max;
        case 2: // DEFAULT
            return device_info.get_exposure_info().default_value;
        case 3: // STEP
            return device_info.get_exposure_info().step;
        case 4: // CURRENT
            return 0;
        case 5: // MANUAL
            return device_info.get_exposure_info().manual ? 1 : 0;
        case 6: // AUTOMATIC
            return device_info.get_exposure_info().automatic ? 1 : 0;
    }

    error_message = "unknown enum";

    return -1;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_readGain
  (JNIEnv *env, jobject obj, jobject type_enum) {
    if( !camera_open ) {
      error_message = "camera not open";
      return -1;
    }

    jclass cls_enum = safe_FindClass( env,  "libwebcam/WebcamDriver$ValueType");
    jmethodID jordinal = safe_GetMethodID(env,cls_enum, "ordinal", "()I");
    jint value = env->CallIntMethod(type_enum, jordinal);

    switch( value ) {
        case 0: // min
            return device_info.get_gain_info().min;
        case 1: // max
            return device_info.get_gain_info().max;
        case 2: // DEFAULT
            return device_info.get_gain_info().default_value;
        case 3: // STEP
            return device_info.get_gain_info().step;
        case 4: // CURRENT
            return 0;
        case 5: // MANUAL
            return device_info.get_gain_info().manual ? 1 : 0;
        case 6: // AUTOMATIC
            return device_info.get_gain_info().automatic ? 1 : 0;
    }

    error_message = "unknown enum";

    return -1;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_readFocus
  (JNIEnv *env, jobject obj, jobject type_enum) {
    if( !camera_open ) {
      error_message = "camera not open";
      return -1;
    }

    jclass cls_enum = safe_FindClass( env,  "libwebcam/WebcamDriver$ValueType");
    jmethodID jordinal = safe_GetMethodID(env,cls_enum, "ordinal", "()I");
    jint value = env->CallIntMethod(type_enum, jordinal);

    switch( value ) {
        case 0: // min
            return device_info.get_focus_info().min;
        case 1: // max
            return device_info.get_focus_info().max;
        case 2: // DEFAULT
            return device_info.get_focus_info().default_value;
        case 3: // STEP
            return device_info.get_focus_info().step;
        case 4: // CURRENT
            return 0;
        case 5: // MANUAL
            return device_info.get_focus_info().manual ? 1 : 0;
        case 6: // AUTOMATIC
            return device_info.get_focus_info().automatic ? 1 : 0;
    }

    error_message = "unknown enum";

    return -1;
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setExposure
  (JNIEnv *env, jobject obj, jboolean automatic, jint value) {
    if( !camera_open ) {
      error_message = "camera not open";
      return;
    }
    device->set_exposure(automatic,value);
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setGain
  (JNIEnv *env, jobject, jboolean automatic, jint value) {
    if( !camera_open ) {
      error_message = "camera not open";
      return;
    }
    device->set_gain(automatic,value);
  }

JNIEXPORT void JNICALL Java_libwebcam_WebcamDriver_setFocus
  (JNIEnv *env, jobject, jboolean automatic, jint value) {
    if( !camera_open ) {
      error_message = "camera not open";
      return;
    }
    device->set_focus(automatic,value);
  }

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_getDevice
  (JNIEnv *env, jobject) {
    return env->NewStringUTF(device_info.get_model_info().get_name().c_str());
  }

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_errorMessage
  (JNIEnv *env, jobject) {
    return env->NewStringUTF(error_message.c_str());
  }

}