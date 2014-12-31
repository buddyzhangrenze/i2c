import java.util.*;
public class TestJni {
	public native void buddy();
	static {
		try{
			System.loadLibrary("TestJni");
		}catch(Throwable e) {
			System.out.println("The Load problem");
		}
	}
}
