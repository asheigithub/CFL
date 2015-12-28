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
	����javaǰ�Ƚ���ǰ�߳�attach�����߳�
	*/
	jniattachthread attachCurrentThread(android_app*);
	
	/*
	������Ϻ����
	*/
	void detachCurrentThread(jniattachthread&);

}




#endif // !_JNITOOL_H




