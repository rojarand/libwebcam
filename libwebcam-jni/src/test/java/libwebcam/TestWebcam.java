package libwebcam;

import org.junit.Test;

import java.util.List;

/**
 * @author Peter Abeles
 */
public class TestWebcam {

    @Test
    public void listDevices() {
        List<DeviceInfo> devices = Webcam.lookupDevices();
        System.out.println("Found devices = "+devices.size());
        for( DeviceInfo d : devices ) {
            System.out.println(d.modelinfo.desc);
        }
    }
}
