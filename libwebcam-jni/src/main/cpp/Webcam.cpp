#include "Webcam.h"
#include <libwebcam/webcam.h>
#include <libwebcam/info/DeviceInfo.h>
#include <libwebcam/info/VideoInfo.h>
#include <libwebcam/image/Format.h>
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
webcam::Shape video_resolution;
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
                        webcam::Shape& selected , std::string &format )
{
    const webcam::VideoInfoEnumeration & video_info_enumeration =
                        device_info.get_video_info_enumeration();
    size_t video_count = video_info_enumeration.count();

    int best_score = -1;

    cout << "Total Video Modes " <<video_count<< endl;
    for (size_t video_index = 0; video_index < video_count; video_index++) {
        const webcam::VideoInfo &video_info = video_info_enumeration.get(video_index);
        std::string format_name =  webcam::lookup_format(video_info.get_format());
        const webcam::Resolutions &resolutions = video_info.get_resolutions();

//        cout << "  image[" << video_index << "] " << format_name << " ID " << video_info.get_format() << endl;

        if( format_name != "MJPG" && format_name != "JPEG") {
            // for debugging save the name of a format which didn't work
            format = format_name;
            continue;
        }

        int format_width,format_height;
        if( resolutions.find_best_match(width,height,format_width,format_height) ) {
            int dw = width-format_width;
            int dh = height-format_height;
            int score = dw*dw + dh*dh;

            if( best_score == -1 || best_score > score ) {
                best_score = score;
                selected = webcam::Shape(format_width,format_height);
                format = format_name;
            }
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
//        cout << " camera not open already" << endl << flush;

      // Pick a webcam to open. The first one is a great choice!
      try {
          const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();
          const size_t count = enumeration.count();

        cout << " Searching v4l devices. total = " << count << endl << endl;

          if( count > 1 ) {
            std::cout << "Multiple cameras to choose from. Selecting first matching. regex='" << str_regex << "`" << endl;
          } else if( count == 0 ) {
            error_message = "No cameras found";
            return JNI_FALSE;
          }

          bool matched = false;
          for( size_t deviceIdx = 0; deviceIdx < count; deviceIdx++ ) {
//              cout << "Start Device Loop " << deviceIdx << endl;
              device_info = enumeration.get(deviceIdx);

              std::string device_name = device_info.get_model_info().get_name();
//              cout << "device found '" << device_name << "'" << endl;
              if( regex_match(device_name, device_regex) ) {
                  cout << "  matched! " << device_name << endl;
                  selected_device = deviceIdx+1;
                  matched = true;
                  break;
              } else {
                  cout << "  didn't match " << device_name << endl;
              }
          }

          if( !matched ) {
              stringstream ss;
              ss << "No matching device found for "<<str_regex;
              error_message = ss.str();
              return JNI_FALSE;
//          } else {
//              cout << "Still Good s" << endl << std::flush;
          }
          
      } catch(const webcam::enumerator_exception & exc_) {
          error_message = exc_.what();
          std::cout<<exc_.what()<<std::endl;
          return JNI_FALSE;
      }

//      cout << " selecting resolution" << endl << flush;
      if( !select_resolution( device_info,width,height,video_resolution,video_format) ) {
        error_message = string("Loc 1: failed to select resolution. No MJPG modes? Found: ").append(video_format);
          return JNI_FALSE;
      }
    } else {
      // see if the resolution has changed
      webcam::Shape selected;
      std::string format;
      if( !select_resolution( device_info,width,height,selected,format) ) {
         error_message = string("Loc 2: failed to select resolution. No MJPG modes? Found: ").append(format);
          return JNI_FALSE;
      }
      if( selected.width == video_resolution.width &&
          selected.height == video_resolution.height )
      {
          return JNI_TRUE;
      }

      // change camera resolution
      video_resolution = selected;
      video_format = format;

       device->close();
       delete device;
       device = NULL;
    }

//    cout << " configuring settings" << endl << flush;
    int format = webcam::lookup_format(video_format);
    webcam::VideoSettings video_settings;
    video_settings.set_format(format);
    video_settings.set_fps(500);
    video_settings.set_resolution(video_resolution);

//    cout << " opening" << endl << flush;

    try {
        device = new webcam::Device(selected_device,video_settings,device_info);
        device->open();
        camera_open = true;
        return JNI_TRUE;
    } catch( webcam::webcam_exception &e ) {
        camera_open = false;
        delete device;
        device = NULL;
        error_message = e.what();
        std::cout << "FAILURE: " << e.get_details() << std::endl;
        return JNI_FALSE;
    }
  }

JNIEXPORT jboolean JNICALL Java_libwebcam_WebcamDriver_close
  (JNIEnv *env, jobject obj) {
  if( !camera_open )
      return JNI_TRUE;

  device->close();
  delete device;
  device = NULL;
  return JNI_TRUE;
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

    if( dinfo.output_width != video_resolution.width ) {
        error_message = "Unexpected width in jpeg.";
        goto fail;
    }
    if( dinfo.output_height != video_resolution.height ) {
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
    return video_resolution.width;
  }

JNIEXPORT jint JNICALL Java_libwebcam_WebcamDriver_imageHeight
  (JNIEnv *env, jobject obj) {
    return video_resolution.height;
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

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_getPort
        (JNIEnv *env, jobject) {
    return env->NewStringUTF(device_info.get_port().c_str());
}

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_getSerialNumber
        (JNIEnv *env, jobject) {
    return env->NewStringUTF(device_info.get_serial().c_str());
}

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_getDeviceType
  (JNIEnv *env, jobject) {
    return env->NewStringUTF(device_info.get_model_info().get_name().c_str());
  }

JNIEXPORT jstring JNICALL Java_libwebcam_WebcamDriver_errorMessage
  (JNIEnv *env, jobject) {
    return env->NewStringUTF(error_message.c_str());
  }

}