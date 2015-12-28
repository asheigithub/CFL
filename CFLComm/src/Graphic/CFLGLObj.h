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
		//��װopengl����Ľӿڡ���Щ������ص�����Ҫ���Դ��������Ķ�ʧ�������
		class IGLObj
		{
			friend class GLObjCollection;
		public:
			IGLObj(unsigned int id, std::list<IGLObj*>::iterator iter) :objId(id), listiter(iter),glhandle(0){}
			virtual ~IGLObj() = default;

			//��ȡԭʼ���
			inline GLuint getHandle() const { return glhandle; }

			const unsigned int objId;

			//��ǰ�����Ƿ���Ч
			virtual bool isValid()=0;

			//�ͷ�ռ�õ���Դ
			virtual void close()=0;

		private:
			//�������Ķ�ʧʱ��������ʱ��ʵ��opgl�����Ѳ����á�
			virtual void onLostEGLContext(CFLContext* context) = 0;

			virtual void onGetNewEGLContext(CFLContext* context) = 0;

			const std::list<IGLObj*>::iterator listiter;

		protected:
			void pushMainLooperCommand(IdrawCommand* cmd);
			
			
			GLuint glhandle;//ԭʼ���

		};

	}
}





#endif // !CFL_GLOBJ_H
