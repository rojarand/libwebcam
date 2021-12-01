package libwebcam;

import utils.NativeUtils;

import java.io.File;

/**
 * Bare bones driver that lets you change settings
 *
 * @author Peter Abeles
 */
public class WebcamDriver {

    // The raw image data in whatever format it likes
    WebcamArray rawData = new WebcamArray();
    boolean convertedToRgb;

    static {
        String[] libs = new String[]{"webcam","webcamjni"};

        for( String lib : libs ) {
            boolean success = false;
            try {
                System.loadLibrary(lib);
                success = true;
            } catch( UnsatisfiedLinkError ignore ){}

            try {
                NativeUtils.setLibraryName(lib);
                // First try loading it locally from the devepmental path
                success = success || NativeUtils.loadLocalPath(new File("build_release/libwebcam-jni"));
                success = success || NativeUtils.loadLocalPath(new File("../build_release/libwebcam-jni"));
                success = success || NativeUtils.loadLocalPath(new File("build/libwebcam-jni"));
                success = success || NativeUtils.loadLocalPath(new File("../build/libwebcam-jni"));
                success = success || NativeUtils.loadLibraryFromJar("/");
                success = success || NativeUtils.loadLibraryFromJar("/arm64-v8a");
                success = success || NativeUtils.loadLibraryFromJar("/armeabi-v7a");

                if (!success)
                    System.err.println("Failed to load native library");
            } catch( Exception e ) {
                e.printStackTrace();
            }
        }
    }
    /**
     * Opens a camera at the closest match to the specified resolution.
     * If camera is already open it will change the resolution to the specified value. If
     * it is open it and the device string doesn't match the current device it will close the camera
     * and search for a match.
     *
     * @param width requested image width.
     * @param height requested image height.
     * @param device identifier for the device you wish to connect to. regex. if multiple
     *               matches then first one is selected. null means pick default
     * @return true if a camera has been successfully opened
     */
    public native boolean open( int width , int height , String device );

    public boolean open( int width , int height ) {
        return open(width, height, null);
    }

    public native boolean close();

    public native boolean isOpen();

    public boolean capture( WebcamImage image ) {
        convertedToRgb = false;
        String format = getWebcamFormat();
        image.reshape(imageWidth(),imageHeight(),imageBands());
        if( format.equals("YUYV") )
        {
            // 1/2 resolution along the columns and full resolution along the rows
            rawData.resize(image.width*image.height*2);
            if( !capture(rawData.data,rawData.data.length) ) {
                return false;
            }
            yuyvToRgb(rawData.data,image);
            return true;
        } else {
            // If it was a RGB image it will be in RGB format already
            convertedToRgb = format.equals("MJPG") || format.equals("JPEG");
            return capture(image.data,image.data.length);
        }
    }

    protected native boolean capture( byte[] data , int length );

    public native int imageWidth();
    public native int imageHeight();
    public native int imageBands();

    public native int readExposure( ValueType type );
    public native int readGain( ValueType type );
    public native int readFocus( ValueType type );

    public native void setExposure( boolean automatic , int value );
    public native void setGain( boolean automatic , int value );
    public native void setFocus( boolean automatic , int value );

    /** The format that the image was encoded in. If not in RGB format that means it's still in this format */
    public native String getWebcamFormat();

    /**
     * Returns true if the image has been converted into an RGB image already
     */
    public boolean isRgbImage() {
        return convertedToRgb;
    }

    // e.g. /dev/video0
    public native String getPort();

    // USB serial number or empty string if non was found
    public native String getSerialNumber();

    // e.g. Logitech BRIO
    public native String getDeviceType();

    public native String errorMessage();

    public enum ValueType {
        MIN,MAX,DEFAULT,STEP,CURRENT,MANUAL,AUTOMATIC
    }

    public static void yuyvToRgb(byte[] dataYV, WebcamImage output) {

        final int yStride = output.width*2;

        //CONCURRENT_BELOW BoofConcurrency.loopFor(0, output.height, row -> {
        for( int row = 0; row < output.height; row++ ) {
            int indexY = row*yStride;
            int indexU = indexY+1;
            int indexOut = row*output.stride;

            for( int col = 0; col < output.width; col++, indexY += 2 ) {
//                System.out.println("row "+row+" col "+col+" indexY "+indexY+" indexU "+indexU);
                int y = 1191*((dataYV[indexY] & 0xFF) - 16);
                int cb = (dataYV[ indexU ] & 0xFF) - 128;
                int cr = (dataYV[ indexU+2] & 0xFF) - 128;

//                System.out.println(y+" "+cb+" "+cr);

//				if( y < 0 ) y = 0;
                y = ((y >>> 31)^1)*y;

                int r = (y + 1836*cr) >> 10;
                int g = (y - 547*cr - 218*cb) >> 10;
                int b = (y + 2165*cb) >> 10;

//				if( r < 0 ) r = 0; else if( r > 255 ) r = 255;
//				if( g < 0 ) g = 0; else if( g > 255 ) g = 255;
//				if( b < 0 ) b = 0; else if( b > 255 ) b = 255;

                r *= ((r >>> 31)^1);
                g *= ((g >>> 31)^1);
                b *= ((b >>> 31)^1);

                // The bitwise code below isn't faster than than the if statement below
//				r |= (((255-r) >>> 31)*0xFF);
//				g |= (((255-g) >>> 31)*0xFF);
//				b |= (((255-b) >>> 31)*0xFF);

                if( r > 255 ) r = 255;
                if( g > 255 ) g = 255;
                if( b > 255 ) b = 255;

                output.data[indexOut++] = (byte)r;
                output.data[indexOut++] = (byte)g;
                output.data[indexOut++] = (byte)b;

                indexU += 4*(col&0x1);
            }
        }
        //CONCURRENT_ABOVE });
    }
}
