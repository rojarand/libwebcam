package libwebcam;

/**
 * Very simple grow array
 */
public class WebcamArray {
    public byte[] data = new byte[0];
    public int size = 0;

    public void resize(int size) {
        if (data.length < size)
            data = new byte[size];
        this.size = size;
    }

    public void reset() {
        size = 0;
    }
}
