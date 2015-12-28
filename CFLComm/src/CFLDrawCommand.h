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
		//��Ⱦ����Ľӿ�
		class IdrawCommand
		{
		public:
			IdrawCommand(const char* summary) :cmdSummary(summary){}
			virtual ~IdrawCommand() = default;

			//��Ⱦ����ִ���������
			virtual void execute(CFLContext* context)=0;
			//֪ͨ�Ѿ�ִ�С���֮��û��滺��,��delete��delete��
			virtual void over()=0;
			//һ��ָ���ժҪ�ַ����������ڵ��ԣ�
			const char* cmdSummary;

		};

		template<typename EF>
		class funcDrawCommand :public IdrawCommand//�ɽ�һ�����������װ�������ģ�幤����
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






		//��Ⱦ����
		class DrawQueue;
		//Ԥִ���������
		class PrepareQueue;
		
		
		//��Ⱦ�̵߳Ĺ�����
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
			
			//����һ����Ⱦ�߳�ָ�������һ����Ⱦʱִ��
			void pushCommand(IdrawCommand* cmd);

			void render(CFLContext* context);

			void swapQueue(CFLContext* context);//������Ⱦ����


			

		private:

			RenderThreadLooper();
			~RenderThreadLooper();

			DrawQueue* backQueue;
			DrawQueue* renderQueue;


			
		};


	}

	

	

}





#endif // !CFL_DRAWCOMMAND_H
