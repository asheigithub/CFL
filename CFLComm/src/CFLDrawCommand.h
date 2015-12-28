#ifndef CFL_DRAWCOMMAND_H
#define CFL_DRAWCOMMAND_H




#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif


namespace cfl
{
	struct CFLContext;
	void mainthread_mainloop(CFLContext* esContext, float deltaTime);
	
	namespace render
	{
		//渲染命令的接口
		class IdrawCommand
		{
		public:
			IdrawCommand(const char* summary) :cmdSummary(summary){}
			virtual ~IdrawCommand() = default;

			//渲染队列执行这条命令。
			virtual void execute(CFLContext* context)=0;
			//通知已经执行。（之后该缓存缓存,该delete就delete）
			virtual void over()=0;
			//一个指令的摘要字符串，（便于调试）
			const char* cmdSummary;

		};

		template<typename EF>
		class funcDrawCommand :public IdrawCommand//可将一个函数对象包装成命令的模板工具类
		{
		public:
			funcDrawCommand(EF execfun, const char* summary) :IdrawCommand(summary), execfun(execfun)
			{

			}

			void execute(CFLContext* context) override final
			{
				
				execfun();
			}

			void over() override final
			{
				delete this;
			}
		private:
			EF execfun;

		};






		//渲染队列
		class DrawQueue;
		//预执行命令队列
		class PrepareQueue;
		
		
		//渲染线程的工作类
		class RenderThreadLooper
		{
			
			//friend struct cfl::CFLContext;
			friend void cfl::mainthread_mainloop(CFLContext* esContext, float deltaTime);
#ifdef ANDROID
			friend class cfl::CFLContext ;
#else
			friend struct cfl::CFLContext;
#endif	

		public:
			
			//发送一条渲染线程指令。将在下一次渲染时执行
			void pushCommand(IdrawCommand* cmd);

			void render(CFLContext* context);

			void swapQueue(CFLContext* context);//交换渲染队列


			

		private:

			RenderThreadLooper();
			~RenderThreadLooper();

			DrawQueue* backQueue;
			DrawQueue* renderQueue;


			
		};


	}

	

	

}





#endif // !CFL_DRAWCOMMAND_H
