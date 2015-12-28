#ifndef _JNITOOL_H
#define _JNITOOL_H

#include <jni.h>
#include <android_native_app_glue.h>
namespace cfl
{

	struct jniattachthread
	{
		JNIEnv* env;
		JavaVM* jvm;
		bool isAttached;
		bool success;

	};

	/*
	调用java前先将当前线程attach到主线程
	*/
	jniattachthread attachCurrentThread(android_app*);
	
	/*
	调用完毕后分离
	*/
	void detachCurrentThread(jniattachthread&);

}




#endif // !_JNITOOL_H




