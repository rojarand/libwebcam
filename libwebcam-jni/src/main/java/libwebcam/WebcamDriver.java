package libwebcam;

import boofcv.struct.image.InterleavedU8;
import org.ddogleg.struct.GrowQueue_I8;
import utils.NativeUtils;

import java.io.File;

/**
 * Bare bones driver that lets you change settings
 *
 * @author Peter Abeles
 */
public class WebcamDriver {

    static {
        boolean success = false;

        try {
            System.loadLibrary("webcamjni");
            success = true;
        } catch( UnsatisfiedLinkError ignore ){}

        try {
            NativeUtils.setLibraryName("webcamjni");
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

    public boolean capture( InterleavedU8 image ) {
        image.reshape(imageWidth(),imageHeight(),imageBands());
        return capture(image.data,image.data.length);
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

    public native String getDevice();

    public native String errorMessage();

    public enum ValueType {
        MIN,MAX,DEFAULT,STEP,CURRENT,MANUAL,AUTOMATIC
    }
}
