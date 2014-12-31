#include "jni.h"
#include "TestJni.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_TestJni_buddy(JNIEnv *env,jobject obj) {
		printf("Hello buddy\n");
}
