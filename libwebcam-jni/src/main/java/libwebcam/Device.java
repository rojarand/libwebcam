package libwebcam;

/**
 * @author Peter Abeles
 */
public class Device {

    public VideoSettings settings;
    public DeviceInfo info;


    public void close() {

    }

    public void open() {

    }

    public void read( Image image ) {

    }

    public VideoSettings getVideoSettings() {
        return settings;
    }

    public DeviceInfo getDeviceInfo() {
        return info;
    }

    public native void setFocus( boolean automatic , int value );

    public native void setExposure( boolean automatic , int value );

    public native void setGain( boolean automatic , int value );
}
