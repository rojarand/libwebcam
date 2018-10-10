#include "Webcam.h"
#include <libwebcam/webcam.h>
#include <stdexcept>

#include <stdio.h>

extern "C" {

JNIEXPORT void JNICALL Java_libwebcam_Webcam_lookupDevices(JNIEnv *env, jobject obj, jobject devices)
{
    const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();

    const int count = enumeration.count();

    printf("Hello JNI! found=%d\n",count);
}

}