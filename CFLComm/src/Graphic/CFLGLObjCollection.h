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

		//opengl对象操作集合
		class GLObjCollection
		{
#ifdef ANDROID
			friend class cfl::CFLContext;
#else
			friend struct cfl::CFLContext;
#endif	
		public:
			//EGL丢失时发生，不要在代码中调用此方法。
			void __notifyLostEGL(CFLContext* context);
			//EGL创建时发生,不要在代码中使用此方法
			void __notifyGetEGL(CFLContext* context);

			//创建一个Shader。
			GLShader* createShader();
			//销毁一个Shader。
			void deleteShader(GLShader*& shader);
			
			//创建一个着色器程序
			GLProgram* createProgram();
			//销毁一个着色器程序
			void deleteProgram(GLProgram*& program);


			//创建一个glTex2D对象
			GLTex2D* createTex2D();
			//销毁一个glTex2D对象
			void deleteTex2D(GLTex2D*& glTex2d);

			//创建一个VBO对象
			GLVBO* createVBO();
			//销毁一个VBO对象
			void deleteVBO(GLVBO*& glVBO);

		private:
			GLObjCollection();
			~GLObjCollection();

			void pushDrawThreadCommand(IdrawCommand* cmd);


		};


	}
}

#endif // !CFL_GLOBJCOLLECTION_H
