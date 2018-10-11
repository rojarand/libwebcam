package libwebcam;

import boofcv.struct.image.InterleavedU8;
import org.junit.Test;

import java.util.List;

/**
 * @author Peter Abeles
 */
public class TestWebcam {

    @Test
    public void openCamera() {
        System.out.println("BEFORE");
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
    public void captureFrames() {
        System.out.println("BEFORE");
        WebcamDriver driver = new WebcamDriver();

        System.out.println("opening");

        if( !driver.open(640,480) ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }

        System.out.println("Actual Resolution: "+driver.imageWidth()+"x"+driver.imageHeight());

        InterleavedU8 image = new InterleavedU8(1,1,1);
        for (int i = 0; i < 10; i++) {
            driver.capture(image);
            System.out.println("Got image "+image.width);
        }

        System.out.println("Closing");
        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Done");
    }
}
