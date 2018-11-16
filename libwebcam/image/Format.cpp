#include "Format.h"
#include <linux/videodev2.h>

#include <iostream>

namespace webcam {

    int lookup_format_four_cc(int format) {
        char four_cc_str[5] =
                {
                        static_cast<char>((format >> 0) & 0xFF),
                        static_cast<char>((format >> 8) & 0xFF),
                        static_cast<char>((format >> 16)& 0xFF),
                        static_cast<char>((format >> 24)& 0xFF),
                        0
                };
//        std::cout << "four_cc " << four_cc_str << std::endl;
        return lookup_format(std::string(four_cc_str));
    }

    std::string lookup_format(int native_format_) {
        switch (native_format_) {
            case V4L2_PIX_FMT_GREY:
                return "GREY";
            case V4L2_PIX_FMT_RGB24:
                return "RGB24";
            case V4L2_PIX_FMT_BGR24:
                return "BGR24";
            case V4L2_PIX_FMT_YUYV:
                return "YUYV";
            case V4L2_PIX_FMT_UYVY:
                return "UYVY";
            case V4L2_PIX_FMT_H264:
                return "H264";
            case V4L2_PIX_FMT_JPEG:
                return "JPEG";
            case V4L2_PIX_FMT_MJPEG:
                return "MJPG";
            case V4L2_PIX_FMT_Y12I:
                return "Y12I";
            case V4L2_PIX_FMT_Y8I:
                return "Y8I ";
            case V4L2_PIX_FMT_Z16:
                return "Z16 ";
            default:
                return "UNKNOWN";
        }
    }

    int lookup_format( const std::string& format ) {
        if( format.compare("RGB24") == 0)
            return V4L2_PIX_FMT_RGB24;
        else if( format.compare("BGR24") == 0)
            return V4L2_PIX_FMT_BGR24;
        else if( format.compare("BGR24") == 0)
            return V4L2_PIX_FMT_RGB24;
        else if( format.compare("YUYV") == 0)
            return V4L2_PIX_FMT_YUYV;
        else if( format.compare("UYVY") == 0)
            return V4L2_PIX_FMT_UYVY;
        else if( format.compare("H264") == 0)
            return V4L2_PIX_FMT_H264;
        else if( format.compare("JPEG") == 0)
            return V4L2_PIX_FMT_JPEG;
        else if( format.compare("MJPG") == 0)
            return V4L2_PIX_FMT_MJPEG;
        else if( format.compare("GREY") == 0)
            return V4L2_PIX_FMT_GREY;
        else if( format.compare("Y12I") == 0)
            return V4L2_PIX_FMT_Y12I;
        else if( format.compare("Y8I ") == 0)
            return V4L2_PIX_FMT_Y8I;
        else if( format.compare("Z16 ") == 0)
            return V4L2_PIX_FMT_Z16;
        else {
//            std::cout << "Unknown Format "<<format<<std::endl;
            return -1;
        }

    }
}
