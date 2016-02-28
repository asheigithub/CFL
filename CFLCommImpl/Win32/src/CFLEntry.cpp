#include "CFLEntry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <windows.h>

#include "CFLFile.h"
#include "CFLMainLoopThread.h"
#include <thread>
#include <sstream>
#include "Content/CFLContent.h"
#include "Graphic/CFLFont.h"

#include "Win32Input.h"

static int targetFPS=60;
static int realFps=0;
namespace cfl
{
	
	CFLContext* CFLContext::getCurrentInstance()
	{
		
		static CFLContext ins;
			
		return &ins;
	}

	void  trace(const char *formatStr, ...)
	{
		va_list params;
		char buf[BUFSIZ*4];

		va_start(params, formatStr);
		vsprintf_s(buf, formatStr, params);

		printf("%s", buf);

		va_end(params);
	}

	void trace_e(const char *formatStr, ...)
	{
		va_list params;
		char buf[BUFSIZ*4];

		va_start(params, formatStr);
		vsprintf_s(buf, formatStr, params);

		printf("%s", buf);

		va_end(params);
	}

	void setFPS(const int fps)
	{
		targetFPS = fps;
	}

	int  getFPS()
	{
		return realFps;
	}


	




	LONG64 GetSysTickCount64()
	{
		static LARGE_INTEGER TicksPerSecond = { 0 };
		LARGE_INTEGER Tick;

		if (!TicksPerSecond.QuadPart)
		{
			QueryPerformanceFrequency(&TicksPerSecond);
		}

		QueryPerformanceCounter(&Tick);

		LONG64 Seconds = Tick.QuadPart / TicksPerSecond.QuadPart;
		LONG64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart*Seconds);
		LONG64 MillSeconds = LeftPart * 1000 / TicksPerSecond.QuadPart;
		LONG64 Ret = Seconds * 1000 + MillSeconds;
		_ASSERT(Ret>0);
		return Ret;
	}


	int getTimer()
	{
		return GetSysTickCount64() & 0x7fffffff;
	}

	void registerUpdateFunc(CFLContext* ctx, const updatefun func)
	{
		ctx->onUpdate = func;
	}
	


}
using namespace cfl;


static LRESULT WINAPI CFLWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
		auto data=(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
		CFLContext *esContext = (CFLContext *)data;
		/*
		if ( esContext && esContext->drawFunc )
		{
		esContext->drawFunc ( esContext );
		eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
		}
		*/
		ValidateRect(esContext->eglNativeWindow, NULL);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:
	{
		POINT      point;
		CFLContext *esContext = (CFLContext *)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

		GetCursorPos(&point);

		/*if (esContext && esContext->keyFunc)
			esContext->keyFunc(esContext, (unsigned char)wParam,
			(int)point.x, (int)point.y);*/
	}
	break;
	
	case WM_LBUTTONDOWN:
	{
		SetCapture(hWnd);
		input::InputState::getInstance()->onMouseButtonDown[0] = true;
		input::InputState::getInstance()->mousebuttonStates[0] = true;

		input::InputState::getInstance()->touches.clear();
		input::InputState::getInstance()->touches.push_back( input::Touch() );

		auto touch = &(input::InputState::getInstance()->currentToTouch);
		touch->fingerId = 0;
		touch->phase = cfl::input::touchPhase::Began;

		touch->position = input::Input::mousePositon();
		touch->deltaPosition = cfl::geom::Vector2();
		touch->deltaTime = 0.0f;
		
	}
	break;
	
	case WM_LBUTTONUP:
	{
		input::InputState::getInstance()->onMouseButtonUp[0] = true;
		input::InputState::getInstance()->mousebuttonStates[0] = false;
		if (GetCapture() == hWnd)
		{
			ReleaseCapture();
		}

		
		auto touch = &(input::InputState::getInstance()->currentToTouch);
		touch->fingerId = 0;
		touch->phase = cfl::input::touchPhase::Ended;

		

		

	}
	break;

	case WM_RBUTTONDOWN:
	{
		SetCapture(hWnd);
		input::InputState::getInstance()->onMouseButtonDown[1] = true;
		input::InputState::getInstance()->mousebuttonStates[1] = true;
	}
	break;
	case WM_RBUTTONUP:
	{
		input::InputState::getInstance()->onMouseButtonUp[1] = true;
		input::InputState::getInstance()->mousebuttonStates[1] = false;
		if (GetCapture() == hWnd)
		{
			ReleaseCapture();
		}
	}
	break;

	case WM_MBUTTONDOWN:
	{
		SetCapture(hWnd);
		input::InputState::getInstance()->onMouseButtonDown[2] = true;
		input::InputState::getInstance()->mousebuttonStates[2] = true;
	}
	break;
	case WM_MBUTTONUP:
	{
		input::InputState::getInstance()->onMouseButtonUp[2] = true;
		input::InputState::getInstance()->mousebuttonStates[2] = false;
		if (GetCapture() == hWnd)
		{
			ReleaseCapture();
		}
	}
	break;


	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}


static int WinCreate(CFLContext *context, const char *title)
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	TCHAR* classname = "CFL_GLES3";


	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)CFLWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = classname;

	if (!RegisterClass(&wndclass))
	{
		return FALSE;
	}

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = context->width;
	windowRect.bottom = context->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	
	context->eglNativeWindow
		= CreateWindow(
		classname,
		reinterpret_cast<LPCSTR>( title),
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
		

	// Set the ESContext* to the GWL_USERDATA so that it is available to the
	// ESWindowProc
	SetWindowLongPtr(context->eglNativeWindow, GWL_USERDATA, (LONG)(LONG_PTR)context);


	if (context->eglNativeWindow == NULL)
	{
		return FALSE;
	}

	ShowWindow(context->eglNativeWindow, TRUE);

	return TRUE;
}

static decltype(getTimer()) lasttick = 0;
static int frame = 0;

static void calFps()
{
	frame++;
	auto tick = getTimer(); //GetTickCount();
	if (lasttick > 0)
	{
		if (tick - lasttick >= 1000)
		{
			realFps = frame;
			frame = 0;
			lasttick = tick;
		}
	}
	else
	{
		lasttick = tick;
	}
}


bool _winMsg(MSG& msg, bool& done)
{
	int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

	if (gotMsg)
	{
		if (msg.message == WM_QUIT)
		{
			done = 1;

			closeGL(cfl::CFLContext::getCurrentInstance());

			cfl::CFLContext::getCurrentInstance()->taskPool->dispose();
			
			cfl::CFLContext::getCurrentInstance()->glObjCollection->__notifyLostEGL(
				cfl::CFLContext::getCurrentInstance());
			notify_exit();

			content::Content::clearData();

			cfl::graphic::font::Font::clearFonts();


		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	return false;
}

static bool done = false;
static void WinLoop(CFLContext *esContext)
{
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	float deltaTime = 0;

	MSG msg = { 0 };
	//bool done = false;

	bool queryflag = true;

	
	
	setupMainLoopthread(esContext);

	while (!done)
	{
		//if (queryflag)
		{
			//queryflag = false;
			QueryPerformanceCounter(&nLast);
		}

		
		if (_winMsg(msg, done)){ continue; }

	{
		//{ //winMsg时间
		//	trace("====\n");

		//	QueryPerformanceCounter(&nNow);
		//	auto _winmsgtime=(nNow.QuadPart - nLast.QuadPart) * 1000 / nFreq.QuadPart;
		//	trace("winmsgtime %d\n", (int)_winmsgtime);
		//}


		  queryflag = true;

		  calFps();

		 
		  mainthread_mainloop(esContext, deltaTime);

		  input::InputState::getInstance()->mousebuttonStates[0] = GetAsyncKeyState(VK_LBUTTON) ? true : false;
		  input::InputState::getInstance()->mousebuttonStates[1] = GetAsyncKeyState(VK_RBUTTON) ? true : false;
		  input::InputState::getInstance()->mousebuttonStates[2] = GetAsyncKeyState(VK_MBUTTON) ? true : false;
		  input::InputState::getInstance()->update(deltaTime);


		  //{ //winMsg时间
			 // QueryPerformanceCounter(&nNow);
			 // auto _mainloopgtime = (nNow.QuadPart - nLast.QuadPart) * 1000 / nFreq.QuadPart;
			 // trace("mainthread_mainloop %d\n", (int)_mainloopgtime);
		  //}

		  {
			  {//***先睡眠
				  QueryPerformanceCounter(&nNow);

				  //auto targetInterval = (LONGLONG)(1.0f / (targetFPS) * nFreq.QuadPart);

				  //auto d = (targetInterval - (nNow.QuadPart - nLast.QuadPart)) / nFreq.QuadPart * 1000;

				  auto targetinterval = 1.0f / targetFPS * 1000;
				  auto realinterval = (float)(nNow.QuadPart - nLast.QuadPart) / nFreq.QuadPart * 1000;

				  auto d = (int)(targetinterval - realinterval);

				  if (d>0)
				  {
					   //auto before = getTimer();
					   std::this_thread::sleep_for(std::chrono::milliseconds(d));
					   //Sleep( static_cast<DWORD>( d));
					   //auto end = getTimer();
					   //trace("need:%d,real sleep: %d\n",(int)d, (end - before));
				  }
			  }
			  do
			  {
				  QueryPerformanceCounter(&nNow);

				  //auto targetInterval = (LONGLONG)(1.0f / targetFPS * nFreq.QuadPart);

				  //auto d = (targetInterval - (nNow.QuadPart - nLast.QuadPart)) * 1000 / nFreq.QuadPart;
				  auto targetinterval = 1.0f / targetFPS * 1000;
				  auto realinterval = (float)(nNow.QuadPart - nLast.QuadPart) / nFreq.QuadPart * 1000;

				  auto d = targetinterval - realinterval;

				  if (d > 0)
				  {
					  std::this_thread::yield();
				  }
				  else
				  {
					  break;
				  }
			  } while (true);

			  if (esContext->eglDisplay != EGL_NO_DISPLAY)
			  {
					eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
			  }
			  
			  

			  QueryPerformanceCounter(&nNow);
			  deltaTime = ((float)(nNow.QuadPart - nLast.QuadPart)) / nFreq.QuadPart;

			  

		  }

	  }

	}
	

}

static int WINAPI esCreateWindow(CFLContext *esContext, const char *title, int width,int height)
{

	
	if (esContext == NULL)
	{
		return -1;
	}


	esContext->width = width;
	esContext->height = height;


	if (!WinCreate(esContext, title))
	{
		return 0;
	}
	return 1;
}

static int WINAPI initGLSurface(CFLContext *esContext)
{
	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };


	esContext->eglDisplay =  eglGetDisplay( GetDC( esContext->eglNativeWindow));
	if (esContext->eglDisplay == EGL_NO_DISPLAY)
	{
		return 0;
	}

	// Initialize EGL
	if (!eglInitialize(esContext->eglDisplay, &majorVersion, &minorVersion))
	{
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
	   if (!eglChooseConfig(esContext->eglDisplay, attribList, &config, 1, &numConfigs))
	   {
		   return 0;
	   }

	   if (numConfigs < 1)
	   {
		   return 0;
	   }
   }

   // Create a surface
   esContext->eglSurface = eglCreateWindowSurface(esContext->eglDisplay, config,
	   esContext->eglNativeWindow, NULL);

   if (esContext->eglSurface == EGL_NO_SURFACE)
   {
	   return 0;
   }

   // Create a GL context
   esContext->eglContext = eglCreateContext(esContext->eglDisplay, config,
	   EGL_NO_CONTEXT, contextAttribs);

   if (esContext->eglContext == EGL_NO_CONTEXT)
   {
	   return 0;
   }

   // Make the context current
   if (!eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
	   esContext->eglSurface, esContext->eglContext))
   {
	   return 0;
   }

   esContext->capability->init();

   //{//纹理压缩格式查询//
	  // GLint num;
	  // glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
	  // GLint* fs = new GLint[num]; //malloc(sizeof(GLint) * num);
	  // glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, fs);
	  // for (size_t i = 0; i < num; i++)
	  // {
		 //  LOGI("GL_COMPRESSED_TEXTURE_FORMATS: %x\n", *(fs + i));
	  // }
	  // delete[] fs;
   //}

   //auto exts = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
   //if (exts != nullptr)
   //{
	  // //std::string input = exts;
	  // std::stringstream ss(exts);

	  // std::string str;
	  // while (std::getline(ss, str, ' '))
	  // {
		 //  if (str.empty())
		 //  {
			//   break;
		 //  }

		 //  str.erase(0, str.find_first_not_of(" "));
		 //  str.erase(str.find_last_not_of(" ") + 1);

		 //  LOGI("GL_EXTENSIONS %s\n", str.c_str());

	  // }


   //}



   esContext->glObjCollection->__notifyGetEGL(esContext);
   esContext->graphic->_onGlInit(esContext);

   return 1;
}




extern int cfl_main(CFLContext* ctx,int argc, char* argv[]);


int  main(int argc, char* argv[])
{
	
	auto _hwndConsole = GetConsoleWindow();
	if (_hwndConsole != NULL)
	{
		HMENU hmenu = GetSystemMenu(_hwndConsole, FALSE); //删掉关闭按钮
		if (hmenu != NULL)
		{
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}

	CFLContext* ctx = CFLContext::getCurrentInstance()  ;
	
	esCreateWindow(ctx, "Hello CFL", 1280, 720);
	
	initGLSurface(ctx);

	

	file::initDirecionaryInfo(nullptr);

	cfl_main(ctx,argc, argv);



	WinLoop(ctx);

	/*if (ctx.shutdownFunc != NULL)
	{
		esContext.shutdownFunc(&esContext);
	}*/

	

	return 0;
}

void cfl::exitApp()
{
	done = true;
}

void cfl::closeGL(CFLContext* context)
{
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

	context->eglDisplay = EGL_NO_DISPLAY;
	context->eglContext = EGL_NO_CONTEXT;
	context->eglSurface = EGL_NO_SURFACE;

}
void cfl::buildGL(CFLContext* ctx)
{
	initGLSurface(ctx);
}