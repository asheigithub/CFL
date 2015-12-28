#ifndef CFL_GLOBJ_H
#define CFL_GLOBJ_H

#include "../CFLDrawCommand.h"
#include <memory>
#include <list>
namespace cfl
{
	struct CFLContext;
	namespace render
	{
		//包装opengl对象的接口。这些对象的特点是需要可以处理上下文丢失的情况。
		class IGLObj
		{
			friend class GLObjCollection;
		public:
			IGLObj(unsigned int id, std::list<IGLObj*>::iterator iter) :objId(id), listiter(iter),glhandle(0){}
			virtual ~IGLObj() = default;

			//获取原始句柄
			inline GLuint getHandle() const { return glhandle; }

			const unsigned int objId;

			//当前对象是否有效
			virtual bool isValid()=0;

			//释放占用的资源
			virtual void close()=0;

		private:
			//当上下文丢失时发生。此时的实际opgl对象已不可用。
			virtual void onLostEGLContext(CFLContext* context) = 0;

			virtual void onGetNewEGLContext(CFLContext* context) = 0;

			const std::list<IGLObj*>::iterator listiter;

		protected:
			void pushMainLooperCommand(IdrawCommand* cmd);
			
			
			GLuint glhandle;//原始句柄

		};

	}
}





#endif // !CFL_GLOBJ_H
