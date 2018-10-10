package libwebcam;

import utils.NativeUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Peter Abeles
 */
public class Webcam {

    static {
        boolean success = false;

        try {
            System.loadLibrary("webcamjni");
            success = true;
        } catch( UnsatisfiedLinkError ignore ){}

        try {
            NativeUtils.setLibraryName("webcamjni");
            // First try loading it locally from the devepmental path
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

    public static List<DeviceInfo> lookupDevices() {
        List<DeviceInfo> found = new ArrayList<>();
        lookupDevices(found);
        return found;
    }

    protected static native void lookupDevices( List<DeviceInfo> devices );
}
