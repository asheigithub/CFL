#include "jnitool.h"
#include <jni.h>

namespace cfl
{

	jniattachthread attachCurrentThread(android_app* state)
	{
		jniattachthread result;
		result.isAttached = false;

		JavaVM* lJavaVM = state->activity->vm;
		JNIEnv* env;

		int status = (lJavaVM)->GetEnv((void **)&env, JNI_VERSION_1_6);
		if (status < 0)
		{
			//***此处需要attachthread;
			status = lJavaVM->AttachCurrentThread(&env, NULL);
			if (status < 0) {

				result.success = false;
				return result;
			}
			result.isAttached = true;
		}

		result.jvm = lJavaVM;
		result.env = env;
		result.success = true;
		return result;

	}

	void detachCurrentThread(jniattachthread& jniatt)
	{
		if (jniatt.isAttached)
		{
			jniatt.jvm->DetachCurrentThread();
		}
	}

}


