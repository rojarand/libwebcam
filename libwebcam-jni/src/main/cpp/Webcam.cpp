#include "Webcam.h"
#include <stdexcept>

#include <stdio.h>

extern "C" {

JNIEXPORT void JNICALL Java_libwebcam_Webcam_lookupDevices(JNIEnv *env, jobject obj, jobject devices)
{
    printf("Hello JNI!\n");
}

}