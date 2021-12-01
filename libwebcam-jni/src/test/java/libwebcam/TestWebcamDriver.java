package libwebcam;

import boofcv.gui.image.ImagePanel;
import boofcv.gui.image.ShowImages;
import boofcv.io.image.ConvertBufferedImage;
import boofcv.misc.BoofMiscOps;
import boofcv.struct.image.InterleavedU8;
import libwebcam.WebcamDriver.ValueType;
import org.junit.jupiter.api.Test;

import java.awt.image.BufferedImage;

/**
 * @author Peter Abeles
 */
public class TestWebcamDriver {

    @Test
    public void openCamera() {
        WebcamDriver driver = new WebcamDriver();

        System.out.println("opening");

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        System.out.println("Actual Resolution: "+driver.imageWidth()+"x"+driver.imageHeight());

        System.out.println("Closing");
        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Done");
    }

    @Test
    public void openNonExistentDevice() {
        WebcamDriver driver = new WebcamDriver();

        System.out.println("opening");

        if( driver.open(640,480, "ASDASDSADSAD") ) {
            System.err.println("Shouldn't have opened any device!"+driver.getDeviceType());
            System.exit(1);
        }
        System.out.println("success");
    }

    @Test
    public void openBRIO() {
        WebcamDriver driver = new WebcamDriver();

        System.out.println("opening");

        if( !driver.open(640,480, ".*(BRIO).*") ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Closing");
        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Done");
    }

    @Test
    public void captureFrames() {
        System.out.println("BEFORE");
        WebcamDriver driver = new WebcamDriver();

        System.out.println("opening");

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        System.out.println("Actual Resolution: "+driver.imageWidth()+"x"+driver.imageHeight());

        var buffered = new BufferedImage(640,480,BufferedImage.TYPE_INT_BGR);
        ImagePanel gui = ShowImages.showWindow(buffered,"foo",true);
        var image = new WebcamImage();
        readImages(driver, buffered, gui, image, 1000);

        System.out.println("Closing");
        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Done");
    }

    @Test
    public void checkReadSettings() {
        WebcamDriver driver = new WebcamDriver();

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        System.out.println("exposure     min: "+driver.readExposure(ValueType.MIN));
        System.out.println("exposure     max: "+driver.readExposure(ValueType.MAX));
        System.out.println("exposure    step: "+driver.readExposure(ValueType.STEP));
        System.out.println("exposure default: "+driver.readExposure(ValueType.DEFAULT));

        System.out.println("focus        min: "+driver.readFocus(ValueType.MIN));
        System.out.println("focus        max: "+driver.readFocus(ValueType.MAX));
        System.out.println("focus       step: "+driver.readFocus(ValueType.STEP));
        System.out.println("focus    default: "+driver.readFocus(ValueType.DEFAULT));

        System.out.println("gain         min: "+driver.readGain(ValueType.MIN));
        System.out.println("gain         max: "+driver.readGain(ValueType.MAX));
        System.out.println("gain        step: "+driver.readGain(ValueType.STEP));
        System.out.println("gain     default: "+driver.readGain(ValueType.DEFAULT));

        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
    }

    @Test
    public void checkWriteExposure() {
        WebcamDriver driver = new WebcamDriver();

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        driver.setExposure(true,driver.readExposure(ValueType.DEFAULT));
        driver.setFocus(true,driver.readFocus(ValueType.DEFAULT));
        driver.setGain(false,driver.readGain(ValueType.DEFAULT));

        var buffered = new BufferedImage(640,480,BufferedImage.TYPE_INT_BGR);
        ImagePanel gui = ShowImages.showWindow(buffered,"foo",true);
        var image = new WebcamImage();

        readImages(driver, buffered, gui, image, 60);

        int min = driver.readExposure(ValueType.MIN);
        int max = driver.readExposure(ValueType.MAX);
        int step = driver.readExposure(ValueType.STEP);
        System.out.println(min+" "+max+" "+step);

        for (int exposure = min; exposure < max; exposure += step*200) {
            System.out.println("  exposure="+exposure);
            driver.setExposure(false,exposure);
            readImages(driver, buffered, gui, image, 30);
        }

        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        BoofMiscOps.sleep(1000);
    }

    @Test
    public void checkWriteGain() {
        WebcamDriver driver = new WebcamDriver();

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        driver.setExposure(false,driver.readExposure(ValueType.DEFAULT));
        driver.setFocus(true,driver.readFocus(ValueType.DEFAULT));
        driver.setGain(true,driver.readGain(ValueType.DEFAULT));

        var buffered = new BufferedImage(640,480,BufferedImage.TYPE_INT_BGR);
        ImagePanel gui = ShowImages.showWindow(buffered,"foo",true);
        var image = new WebcamImage();

        readImages(driver, buffered, gui, image, 60);

        int min = driver.readGain(ValueType.MIN);
        int max = driver.readGain(ValueType.MAX);
        int step = driver.readGain(ValueType.STEP);
        System.out.println(min+" "+max+" "+step);

        for (int gain = min; gain < max; gain += step*3) {
            System.out.println("  gain="+gain);
            driver.setExposure(false,gain);
            readImages(driver, buffered, gui, image, 30);
        }

        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        BoofMiscOps.sleep(1000);
    }

    @Test
    public void checkWriteFocus() {
        WebcamDriver driver = new WebcamDriver();

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        driver.setExposure(true,driver.readExposure(ValueType.DEFAULT));
        driver.setFocus(true,driver.readFocus(ValueType.DEFAULT));
        driver.setGain(true,driver.readGain(ValueType.DEFAULT));

        var buffered = new BufferedImage(640,480,BufferedImage.TYPE_INT_BGR);
        ImagePanel gui = ShowImages.showWindow(buffered,"foo",true);
        var image = new WebcamImage();

        readImages(driver, buffered, gui, image, 60);

        int min = driver.readFocus(ValueType.MIN);
        int max = driver.readFocus(ValueType.MAX);
        int step = driver.readFocus(ValueType.STEP);
        System.out.println(min+" "+max+" "+step);

        for (int focus = min; focus < max; focus += step*2) {
            System.out.println("  focus="+focus);
            driver.setFocus(false,focus);
            readImages(driver, buffered, gui, image, 30);
        }

        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        BoofMiscOps.sleep(1000);
    }

    private void readImages(WebcamDriver driver, BufferedImage buffered, ImagePanel gui, WebcamImage image, int i2) {
        var boof = new InterleavedU8();
        for (int i = 0; i < i2; i++) {
            driver.capture(image);
            boof.data = image.data;
            boof.width = image.width;
            boof.height = image.height;
            boof.stride = image.stride;
            boof.setNumberOfBands(image.bands);
            ConvertBufferedImage.convertTo(boof, buffered, true);
            gui.setImageRepaint(buffered);
        }
    }
}
