import boofcv.struct.image.InterleavedU8;
import libwebcam.WebcamDriver;

import java.io.IOException;

public class ExampleLibWebcam {
    public static void main(String[] args) throws IOException {
        WebcamDriver webcam = new WebcamDriver();

        boolean success = webcam.open(4000,2000, ".*(ZED).*");
//        boolean success = webcam.open(4000,2000);

        // It might find a device but then fail. If that's the code this information can be useful
        System.out.println("Device Type:   "+webcam.getDeviceType());
        System.out.println("Device Port:   "+webcam.getPort());
        System.out.println("Device Serial: "+webcam.getSerialNumber());

        if( !success ) {
            System.err.println("Failed to open webcam: "+webcam.errorMessage());
            System.exit(1);
        }

        InterleavedU8 color = new InterleavedU8(1, 1, 3);

        long time0 = System.nanoTime();
        for(int i = 0; i < 100; i++) {
            System.out.println("Got Frame "+i+" at "+color.width+"x"+color.height);
            if (!webcam.capture(color)) {
                throw new RuntimeException("Capture Failed");
            }

            // do something with the image
        }
        long time1 = System.nanoTime();
        System.out.println("Period: "+((time1-time0)*1e-6/100.0)+" (ms)");
        webcam.close();
    }
}
