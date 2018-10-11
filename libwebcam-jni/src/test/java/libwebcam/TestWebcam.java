package libwebcam;

import boofcv.gui.image.ImagePanel;
import boofcv.gui.image.ShowImages;
import boofcv.io.image.ConvertBufferedImage;
import boofcv.struct.image.InterleavedU8;
import org.junit.Test;

import java.awt.image.BufferedImage;
import java.awt.image.ImageProducer;
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

        BufferedImage buffered = new BufferedImage(640,480,BufferedImage.TYPE_INT_BGR);
        ImagePanel gui = ShowImages.showWindow(buffered,"foo",true);
        InterleavedU8 image = new InterleavedU8(1,1,1);
        for (int i = 0; i < 1000; i++) {
            driver.capture(image);
//            System.out.println("Got image "+image.width);
            ConvertBufferedImage.convertTo(image,buffered,true);
            gui.setImageRepaint(buffered);
        }

        System.out.println("Closing");
        if( !driver.close() ) {
            System.err.println("Failed: "+driver.errorMessage());
            System.exit(1);
        }
        System.out.println("Done");
    }
}
