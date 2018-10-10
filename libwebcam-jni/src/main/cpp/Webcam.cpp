#include "Webcam.h"
#include <libwebcam/webcam.h>
#include <stdexcept>
#include <iostream>

using namespace std;

extern "C" {

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

JNIEXPORT void JNICALL Java_libwebcam_Webcam_lookupDevices(JNIEnv *env, jclass obj, jobject devices)
{
    try
    {
        jclass cls_dinfo = safe_FindClass( env,  "libwebcam/DeviceInfo");
        jmethodID constructor_vinfo = safe_GetMethodID(env, cls_dinfo, "<init>", "()V");

        jclass cls_list = safe_FindClass( env,  "java/util/List");
        jmethodID list_add = safe_GetMethodID(env, cls_list, "add", "(Ljava/lang/Object;)Z");

        const webcam::DeviceInfoEnumeration & enumeration = webcam::enumerator::enumerate();

        const size_t count = enumeration.count();

        for (size_t device_index = 0; device_index < count; device_index++) {
            const webcam::DeviceInfo & device_info = enumeration.get(device_index);

            jobject jinfo = env->NewObject(cls_dinfo,constructor_vinfo);

            env->CallObjectMethod(devices,list_add,jinfo);
        }

    }
    catch(const webcam::enumerator_exception & exc_)
    {
        std::cout<<exc_.what()<<std::endl;
    }
}

}