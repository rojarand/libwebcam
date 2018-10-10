package libwebcam;

/**
 * @author Peter Abeles
 */
public class DeviceInfo {

    public ModelInfo modelinfo = new ModelInfo();

    public ControlInfo controlGain = new ControlInfo();
    public ControlInfo controlExposure = new ControlInfo();
    public ControlInfo controlFocus = new ControlInfo();

}
