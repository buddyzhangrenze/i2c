import java.util.*;
public class HelloWorld {
	public static void main(String argv[]) {
		new HelloWorld();
	}
	public HelloWorld() {
		new TestJni().buddy();
	}

}
