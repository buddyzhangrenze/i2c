import java.util.*;
public class TestJni {
	/*
	 * c interface for Java.
	 */
	public native void buddy();
	static {
		try{
			/*
			 * Load C lib *.so
			 */
			System.loadLibrary("TestJni");
		}catch(Throwable e) {
			System.out.println("The Load problem");
		}
	}
}
