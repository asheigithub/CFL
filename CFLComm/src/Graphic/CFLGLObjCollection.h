#ifndef CFL_GLOBJCOLLECTION_H
#define CFL_GLOBJCOLLECTION_H


#include "CFLShader.h"
#include "CFLProgram.h"
#include "CFLGLTex2D.h"
#include "CFLGLVBO.h"

namespace cfl
{
	struct CFLContext;

	namespace render
	{

		//opengl�����������
		class GLObjCollection
		{
#ifdef ANDROID
			friend class cfl::CFLContext;
#else
			friend struct cfl::CFLContext;
#endif	
		public:
			//EGL��ʧʱ��������Ҫ�ڴ����е��ô˷�����
			void __notifyLostEGL(CFLContext* context);
			//EGL����ʱ����,��Ҫ�ڴ�����ʹ�ô˷���
			void __notifyGetEGL(CFLContext* context);

			//����һ��Shader��
			GLShader* createShader();
			//����һ��Shader��
			void deleteShader(GLShader*& shader);
			
			//����һ����ɫ������
			GLProgram* createProgram();
			//����һ����ɫ������
			void deleteProgram(GLProgram*& program);


			//����һ��glTex2D����
			GLTex2D* createTex2D();
			//����һ��glTex2D����
			void deleteTex2D(GLTex2D*& glTex2d);

			//����һ��VBO����
			GLVBO* createVBO();
			//����һ��VBO����
			void deleteVBO(GLVBO*& glVBO);

		private:
			GLObjCollection();
			~GLObjCollection();

			void pushDrawThreadCommand(IdrawCommand* cmd);


		};


	}
}

#endif // !CFL_GLOBJCOLLECTION_H
