#ifndef CFL_SHADER_H
#define CFL_SHADER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif

#include "CFLGLObj.h"
//#include "CFLGraphic.h"

namespace cfl
{
	namespace render
	{
		enum ShaderType
		{
			vertex_shader = GL_VERTEX_SHADER,
			fragment_shader = GL_FRAGMENT_SHADER
		};

		//��ɫ����
		class GLShader :public IGLObj
		{
			friend class GLObjCollection;
			
		public:
			
			//������ɫ����������
			void compile(const GLchar* source,ShaderType type );

			void onLostEGLContext(CFLContext* context) override final;

			void onGetNewEGLContext(CFLContext* context) override final;

			inline bool isValid() override final{ return _isvalid; };

			void close() override final;

		private:
			GLShader(unsigned int id, std::list<IGLObj*>::iterator iter);
			~GLShader();

			
			GLchar* sourcecode;

			ShaderType shadertype;

			bool _isvalid;

			void doCompile();

		};


	}
}


#endif // !CFL_SHADER_H
