package libwebcam;

import java.nio.ByteBuffer;

/**
 * @author Peter Abeles
 */
public class Image {
    public final VideoSettings settings = new VideoSettings();
    public byte[] buffer = new byte[0];

    public void resize( int size ) {
        if( buffer.length != size )
            buffer = new byte[size];
    }
}
