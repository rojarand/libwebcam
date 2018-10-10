package libwebcam;

/**
 * @author Peter Abeles
 */
public class VideoSettings {
    public String format;
    public int fps;
    public int quality;
    public int width,height;
    public Parameter focus = new Parameter();
    public Parameter exposure = new Parameter();
}
