package libwebcam;

/**
 * Storage for image data. Data format depends on the webcam mode.
 */
public class WebcamImage {
    public byte[] data = new byte[0];
    /** Image width */
    public int width;
    /** Image height */
    public int height;
    /** Number of bands */
    public int bands;
    /** Side between rows */
    public int stride;

    public WebcamImage() {}

    public WebcamImage(int width, int height, int bands) {
        reshape(width,height,bands);
    }

    public void reshape(int width, int height, int bands) {
        int length = width*height*bands;
        if (data.length < length)
            data = new byte[length];
        this.width = width;
        this.height = height;
        this.bands = bands;
        this.stride = width*bands;
    }

    public int get(int x, int y) {
        return data[y*stride + x] & 0xFF;
    }

    public int get(int x, int y, int band) {
        return data[y*stride + x*bands + band] & 0xFF;
    }

    public void set(int x, int y, int value) {
        data[y*stride + x] = (byte)value;
    }
}
