#ifndef _CFLEntry_H
#define _CFLEntry_H

#ifndef CFL_API
#ifdef WIN32
#define CFL_API  __cdecl
#define CFL_CALLBACK  __cdecl
#else
#define CFL_API
#define CFL_CALLBACK
#endif
#endif

#include <assert.h>


#define LOGI(...) ( trace(__VA_ARGS__))
//#define LOGE(...) ( trace_e( __FILE__#__VA_ARGS__))
#define LOGE(format,...) trace_e("File: "  __FILE__  ", Line: %05d: " format "\n", __LINE__, ##__VA_ARGS__)


#ifdef WIN32
//assert宏定义
#define CFL_ASSERT(cond)  assert(cond)

#else

#define CFL_ASSERT(cond)  if(!(cond)){ LOGE("assert!  %s\n" ,#cond) ;} 

#endif

#include <sstream>
#include "CFLThreadTasks.h"
#include "CFLDrawCommand.h"
#include "Graphic/CFLGraphic.h"
#include "Graphic/CFLGLObj.h"
#include "Graphic/CFLGLObjCollection.h"
#include "CFLCapability.h"
#include "CFLStage.h"
#include "Text/CFLTextElementEnumerator.h"
#include "CFLString.h"
#include "Input/CFLInput.h"

namespace cfl
{
	struct CFLContext;
	/**
	输出
	*/
	void CFL_API trace(const char *formatStr, ...);

	static void CFL_API trace(const bool v)
	{
		if (v)
		{
			trace("true");
		}
		else
		{
			trace("false");
		}
	}


	/*
	输出错误信息
	*/
	void CFL_API trace_e(const char *formatStr, ...);


	void CFL_API setFPS(const int fps);

	int CFL_API getFPS();

	/*
	获取运行时间的毫秒数
	*/
	int CFL_API getTimer();

	
	using updatefun = void(CFL_CALLBACK *)(CFLContext* context ,float detTime);
	
	/*
	注册update函数
	*/
	void CFL_API registerUpdateFunc(CFLContext*, const updatefun);
	

#ifdef WIN32
	//模拟OPGL丢失
	void closeGL(CFLContext*);
	void buildGL(CFLContext*);
	void exitApp();
#endif





	struct CFLContext
	{
		friend void mainthread_mainloop(CFLContext* esContext, float deltaTime);
		friend class render::IGLObj;
		friend class render::GLObjCollection;
	private:

		bool _isdisposed=false;

		~CFLContext()
		{
			__dispose();
		}
		CFLContext(CFLContext& rhs) = delete;

		CFLContext& operator=(CFLContext& rhs) = delete;
#ifdef ANDROID
		CFLContext(cfl::tasks::TaskPool* pool):taskPool(pool),
			renderLooper(new render::RenderThreadLooper()), 
			graphic(new graphic::Graphic( renderLooper)),
			glObjCollection( new render::GLObjCollection() ),
			capability(new Capability()),
			stage(new Stage())
		{
#else
		CFLContext() :taskPool(new cfl::tasks::TaskPool()), 
			renderLooper(new render::RenderThreadLooper()), 
			graphic( new graphic::Graphic( renderLooper)), 
			glObjCollection( new render::GLObjCollection() ),
			capability(new Capability()),
			stage(new Stage())
		{
#endif			
			
			platformData = nullptr;

			width = 0;
			height = 0;
			totalframes = 0;

			onUpdate = nullptr;
			
#ifndef __APPLE__
			/// Display handle
			eglNativeDisplay = nullptr;

			/// Window handle
			eglNativeWindow = nullptr;

			/// EGL display
			eglDisplay = nullptr;

			/// EGL context
			eglContext = nullptr;

			/// EGL surface
			eglSurface = nullptr;
#endif
		};

		render::RenderThreadLooper* const renderLooper;

	public:

		static CFLContext* getCurrentInstance();

		render::GLObjCollection* glObjCollection;

		graphic::Graphic* graphic;

		tasks::TaskPool* const taskPool; 

		/// Put platform specific data here
		void       *platformData;

		//总共运行的帧数
		size_t       totalframes;

		/// Window width
		int       width;

		/// Window height
		int       height;


		updatefun onUpdate;

		Capability* capability;

		Stage*		stage;

#ifndef __APPLE__
		/// Display handle
		EGLNativeDisplayType eglNativeDisplay;

		/// Window handle
		EGLNativeWindowType  eglNativeWindow;

		/// EGL display
		EGLDisplay  eglDisplay;

		/// EGL context
		EGLContext  eglContext;

		/// EGL surface
		EGLSurface  eglSurface;
#endif
		//不要再代码里调用这个方法!!
		void __dispose()
		{
			if (!_isdisposed)
			{
				_isdisposed = true;

				delete graphic;
				delete renderLooper;
				delete glObjCollection;
				delete capability;
				delete taskPool;
				delete stage;

			}
		}

	};



}


#define CFL_IFGLERROR(A) \
	{								\
		auto glerror = glGetError();\
		if (glerror != GL_NO_ERROR)	\
		{							\
			switch (glerror)		\
			{						\
			case GL_INVALID_ENUM:	\
				LOGE("glerror: GL_INVALID_ENUM");break;	\
			case GL_INVALID_VALUE:					\
				LOGE("glerror: GL_INVALID_VALUE");break;	\
			case GL_INVALID_OPERATION:				\
				LOGE("glerror: GL_INVALID_OPERATION");break;\
			case GL_OUT_OF_MEMORY:						\
				LOGE("glerror: GL_OUT_OF_MEMORY");break;		\
			default:									\
				LOGE("glerror: 尼玛不知道了");			\
				break;									\
			}											\
														\
			{A;}										\
		}												\
														\
	}													\


#ifdef _DEBUG

#define CFL_CHECKGLERROR CFL_IFGLERROR(;)

#else

#define CFL_CHECKGLERROR

#endif



#endif