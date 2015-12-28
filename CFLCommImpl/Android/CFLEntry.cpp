#include "clearwindows.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "gl3stub.h"

#include <sstream>
#include <string>
#include <memory>

#include <math.h>
#include "CFLEntry.h"
#include "CFLFile.h"
#include "CFLAndroidMainLoopThread.h"

#include "Content/CFLContent.h"

static int targetFPS = 60;
static int realFps = 0;
float GetCurrentTime();

namespace cfl
{

	
	static CFLContext* __instance = nullptr;
	CFLContext* CFLContext::getCurrentInstance()
	{
		if (__instance == nullptr)
		{
			LOGI("create instance");
			__instance = new CFLContext(new tasks::TaskPool());
		}
		//static CFLContext __instance(new tasks::taskPool());
		return __instance;
	}

	void  trace(const char *formatStr, ...)
	{
		
		va_list params;
	   char buf[BUFSIZ];
		va_start ( params, formatStr );
	   vsprintf(buf, formatStr, params); 

	   __android_log_print(ANDROID_LOG_INFO, "CFL", "%s", buf);

	   va_end ( params );
	}
	
	void trace_e(const char *formatStr, ...)
	{
		va_list params;
		char buf[BUFSIZ];
		va_start(params, formatStr);
		vsprintf(buf, formatStr, params);

		__android_log_print(ANDROID_LOG_ERROR, "CFL", "%s", buf);

		va_end(params);
	}

	void setFPS(const int fps)
	{
		targetFPS = fps;
	}

	int getFPS()
	{
		return realFps;
	}

	void registerUpdateFunc(CFLContext* ctx, const updatefun func)
	{
		ctx->onUpdate = func;
	}
	


	int getTimer()
	{
		return static_cast<int>( GetCurrentTime() * 1000);
	}
}



/**
* Our saved state data.
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Shared state for our app.
*/
struct appdata {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int glver;
	
	int animating;

	struct saved_state state;
};


using namespace cfl;
extern int cfl_main(CFLContext* ctx, int argc, char* argv[]);

/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(CFLContext* ctx) {
	
	appdata* engine = reinterpret_cast<appdata*>( ctx->platformData);

	// initialize OpenGL ES and EGL

	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/

	LOGI("initialize OPGL ES");
	
	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

	//engine->width = ANativeWindow_getWidth(engine->app->window);
	//engine->height = ANativeWindow_getHeight(engine->app->window);

	ctx->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (ctx->eglDisplay == EGL_NO_DISPLAY)
	{
		LOGE("error eglGetDisplay");
		return 0;
	}

	if (!eglInitialize(ctx->eglDisplay, &majorVersion, &minorVersion))
	{
		LOGE("error eglInitialize");
		return 0;
	}

	{
		EGLint numConfigs = 0;
		EGLint attribList[] =
		{
			EGL_RED_SIZE, 5,
			EGL_GREEN_SIZE, 6,
			EGL_BLUE_SIZE, 5,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 8,
			EGL_STENCIL_SIZE, 8,
			EGL_SAMPLE_BUFFERS, 0,
			// if EGL_KHR_create_context extension is supported, then we will use
			// EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(ctx->eglDisplay, attribList, &config, 1, &numConfigs))
		{
			LOGE("error eglChooseConfig");
			return 0;
		}

		if (numConfigs < 1)
		{
			LOGE("error eglChooseConfig numConfig<1");
			return 0;
		}
	}


	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	{
		EGLint format = 0;
		eglGetConfigAttrib(ctx->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	}

	ctx->eglSurface = eglCreateWindowSurface(ctx->eglDisplay, config,
		engine->app->window, NULL);
	if (ctx->eglSurface == EGL_NO_SURFACE)
	{
		LOGE("error eglCreateWindowSurface");
		return 0;
	}

	ctx->eglContext = eglCreateContext(ctx->eglDisplay, config,
		EGL_NO_CONTEXT, contextAttribs);

	if (ctx->eglContext == EGL_NO_CONTEXT)
	{
		LOGE("error eglCreateContext");
		return 0;
	}

	if (!eglMakeCurrent(ctx->eglDisplay, ctx->eglSurface,
		ctx->eglSurface, ctx->eglContext))
	{
		LOGE("error eglMakeCurrent");
		return 0;
	}

	ctx->capability->init();

	//{//纹理压缩格式查询//
	//	GLint num;
	//	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
	//	GLint* fs = new GLint[num]; //malloc(sizeof(GLint) * num);
	//	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, fs);
	//	for (size_t i = 0; i < num; i++)
	//	{
	//		LOGI("GL_COMPRESSED_TEXTURE_FORMATS: %x\n", *(fs + i));
	//	}
	//	delete[] fs;
	//}

	


	EGLint w;
	EGLint h;

	eglQuerySurface(ctx->eglDisplay, ctx->eglSurface, EGL_WIDTH, &w);
	eglQuerySurface(ctx->eglDisplay, ctx->eglSurface, EGL_HEIGHT, &h);

	ctx->width = w;
	ctx->height = h;

	engine->glver = ctx->capability->getGlVer();

	//engine->state.angle = 0;

	/*auto glver = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	if (glver != nullptr)
	{
		LOGI("glGetString %s", glver);
	}
	else
	{
		LOGI("glGetString NULL !!?");
	}
	if (strstr(glver, "OpenGL ES 3.0") && gl3stubInit() == GL_TRUE)
	{
		engine->glver = 3;
		LOGI("ES3");

	}
	else
	{
		engine->glver = 2;
		LOGI("ES2");
		
	}*/

	//auto exts = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
	//if (exts != nullptr)
	//{
	//	//std::string input = exts;
	//	std::stringstream ss(exts);

	//	std::string str;
	//	while (std::getline(ss, str, ' '))
	//	{
	//		if (str.empty())
	//		{
	//			break;
	//		}

	//		str.erase(0, str.find_first_not_of(" "));
	//		str.erase(str.find_last_not_of(" ") + 1);

	//		LOGI("GL_EXTENSIONS %s", str.c_str());

	//	}


	//}

	

	//glViewport(0, 0, ctx->width, ctx->height);

	LOGI("OPGL ES inited");

	ctx->glObjCollection->__notifyGetEGL(ctx);

	ctx->graphic->_onGlInit(ctx);

	return 0;
}


/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(CFLContext* context) {
	
	appdata* engine = reinterpret_cast<appdata*>( context->platformData);
	
	context->glObjCollection->__notifyLostEGL(context);

	if (context->eglDisplay != EGL_NO_DISPLAY) {
		eglMakeCurrent(context->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context->eglContext != EGL_NO_CONTEXT) {
			eglDestroyContext(context->eglDisplay, context->eglContext);
		}
		if (context->eglSurface != EGL_NO_SURFACE) {
			eglDestroySurface(context->eglDisplay, context->eglSurface);
		}
		eglTerminate(context->eglDisplay);

		LOGI("OPGL ES Disposed");

	}
	engine->animating = 0;
	context->eglDisplay = EGL_NO_DISPLAY;
	context->eglContext = EGL_NO_CONTEXT;
	context->eglSurface = EGL_NO_SURFACE;

	
}

/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	CFLContext* context = reinterpret_cast<CFLContext*>( app->userData);
	appdata* engine = reinterpret_cast<appdata*>(context->platformData);

	auto eventtype = AInputEvent_getType(event);

	if (eventtype == AINPUT_EVENT_TYPE_KEY)
	{

		auto action = AKeyEvent_getAction(event);
		auto keycode = AKeyEvent_getKeyCode(event);

		if (keycode == AKEYCODE_BACK)
		{
			
			return 0;
		}

		switch (action)
		{


		case AKEY_EVENT_ACTION_DOWN:
			LOGI("key down: %d\n", keycode);
			break;
		case AKEY_EVENT_ACTION_UP:
			LOGI("key up: %d\n", keycode);
			break;
		case AKEY_EVENT_ACTION_MULTIPLE:
			LOGI("key multiple: %d\n", keycode);

			return 0;

			break;
		}



		//LOGI("key source: %d\n", source);

		return 0;
	}
	else if (eventtype == AINPUT_EVENT_TYPE_MOTION) {
		engine->animating = 1;
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);



		//LOGI("posx:%d\n", engine->state.x);
		//LOGI("posy:%d\n", engine->state.y);




		switch (AInputEvent_getSource(event))
		{
		case AINPUT_SOURCE_TOUCHSCREEN:
		{
			int32_t id = AMotionEvent_getAction(event);
			int flags = id & AMOTION_EVENT_ACTION_MASK;

			switch (flags)
			{
			case AMOTION_EVENT_ACTION_MOVE:
			{
				size_t    cnt = AMotionEvent_getPointerCount(event);
				for (int i = 0; i < cnt; ++i)
				{
					float x = AMotionEvent_getX(event, i);
					float y = AMotionEvent_getY(event, i);

					int pid = AMotionEvent_getPointerId(event, i);


					//LOGI("TOUCHMOVE x = %f  y = %f pid:%d\n", x, y, pid);

				}

			}
			break;
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
			case AMOTION_EVENT_ACTION_DOWN:
			{
				float x = AMotionEvent_getX(event, 0);
				float y = AMotionEvent_getY(event, 0);

				//LOGI("x = %f  y = %f", x, y);
				int index = (id & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int pid = AMotionEvent_getPointerId(event, index);
				//LOGI("TOUCHDOWN x = %f  y = %f pid:%d\n", x, y, pid);
			}
			break;
			case AMOTION_EVENT_ACTION_POINTER_UP:
			case AMOTION_EVENT_ACTION_UP:
			{
				//toggleKeyboard(app);

				int index = (id & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int pid = AMotionEvent_getPointerId(event, index);
				//LOGI("TOUCHUP pid:%d\n", pid);
			}
			break;
			}
		}

		}


		return 0;
	}
	return 0;
}

/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	CFLContext* context = reinterpret_cast<CFLContext*>(app->userData);

	appdata* engine = reinterpret_cast<appdata*>( context->platformData);

	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL) {
			engine_init_display(context);
			//engine_draw_frame(engine);
			engine->animating = true;
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine_term_display(context);
		break;
	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		//engine_draw_frame(engine);
		break;
	}
}


void updateframe(CFLContext* ctx, float nLast);

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state) {
	
	__instance = nullptr;
	CFLContext* context = CFLContext::getCurrentInstance();
	

	struct appdata appdata;
	// Make sure glue isn't stripped.
	app_dummy();

	cfl::file::initDirecionaryInfo( reinterpret_cast<void*>( state));

	memset(&appdata, 0, sizeof(appdata));
	state->userData = context;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	appdata.app = state;

	context->platformData = &appdata;


	// Prepare to monitor accelerometer
	appdata.sensorManager = ASensorManager_getInstance();
	appdata.accelerometerSensor = ASensorManager_getDefaultSensor(appdata.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	appdata.sensorEventQueue = ASensorManager_createEventQueue(appdata.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		appdata.state = *(struct saved_state*)state->savedState;
	}

	cfl_main(context, 0, nullptr);

	// loop waiting for stuff to do.

	setupMainLoopthread(context);

	while (1) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		float loopBegin = GetCurrentTime();

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(appdata.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
				if (appdata.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(appdata.sensorEventQueue,
						&event, 1) > 0) {
						/*LOGI("accelerometer: x=%f y=%f z=%f",
						event.acceleration.x, event.acceleration.y,
						event.acceleration.z);*/
					}
				}
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0) {

				
				engine_term_display(context);
				
				notify_exit();

				content::Content::clearData();

				context->taskPool->dispose();
				LOGI("app exit");
				return;
			}
		}
		
		updateframe(context, loopBegin);
		
	}
}

float GetCurrentTime()
{
	struct timespec clockRealTime;
	clock_gettime(CLOCK_MONOTONIC, &clockRealTime);
	double curTimeInSeconds = clockRealTime.tv_sec + (double)clockRealTime.tv_nsec / 1e9;
	return (float)curTimeInSeconds;
}

int frame;
float frameLast = 0;
void updateframe(CFLContext* context, float loopBegin)
{
	//float nLast = GetCurrentTime();	
	
	frame++;

	if (loopBegin - frameLast >= 1)
	{
		//LOGI("FPS: %d\n", frame);
		realFps = frame;
		frame = 0;
		frameLast = loopBegin;
	}

	
	appdata* engine = reinterpret_cast<appdata*>( context->platformData);
	
	
	if (engine->animating ) {
		// Done with events; draw next animation frame.
		
		

		static float lastupdatetime = -1;
		float dettime = (lastupdatetime>0 ? loopBegin - lastupdatetime : 0);
		lastupdatetime = loopBegin;

		mainthread_mainloop(context, dettime);

		/*if ( context->eglDisplay != EGL_NO_DISPLAY)
		{
			if (context->onDraw)
			{
				context->onDraw(context);
			}
		}

		if (context->onUpdate)
		{
			context->onUpdate(dettime);
		}*/
		

		if (context->eglDisplay != EGL_NO_DISPLAY)
		{
			if (targetFPS != 60)//执行<60fps的垂直同步操作
			{
				eglSwapBuffers(context->eglDisplay, context->eglSurface);
			}
		}


		
		float nNow = GetCurrentTime();
		if (nNow - loopBegin < 1.0f / (targetFPS == 60 ? 100 : targetFPS)) //如果需要60帧，就以eglSwapBuffers的垂直同步来精确锁定，1/100 粗略睡眠一下否则就按1/fps睡眠,并且在前面执行垂直同步
		{
			timespec t;
			t.tv_sec = 0;
			t.tv_nsec = (1.0f / (targetFPS == 60 ? 100 : targetFPS) - (nNow - loopBegin))* 1e9;

			//trace("sleep: %d", (int)((1.0f / (targetFPS == 60 ? 100 : targetFPS) - (nNow - loopBegin)) * 1000));
			clock_nanosleep(CLOCK_MONOTONIC, 0, &t, NULL);
		}
		
		if (context->eglDisplay != EGL_NO_DISPLAY)
		{
			
			if (targetFPS == 60 && engine->animating) //执行60FPS垂直同步操作
			{
				
				eglSwapBuffers(context->eglDisplay, context->eglSurface);
				
			}
		}
		
		
	}

	

}

