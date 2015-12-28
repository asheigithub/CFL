#ifndef CFL_VERTEXPOINTER_H
#define CFL_VERTEXPOINTER_H


#include <EGL/egl.h>
#include <EGL/eglext.h>

//#include <GLES2/gl2ext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif

#include "IGLDataResouce.h"
#include <memory>

namespace cfl
{
	namespace render
	{

		namespace vertexPointerType
		{
			/*
			������������
			*/
			enum VertexPointerType
			{
				glByte = GL_BYTE,
				glUnsignedByte = GL_UNSIGNED_BYTE,
				glShort = GL_SHORT,
				glUnsignedShort = GL_UNSIGNED_SHORT,
				glFloat = GL_FLOAT,
				glFixed = GL_FIXED,

				glHalf = 0x8D61 //GL_HALF_FLOAT_OES
				

			};
		}

		/*
		������������
		*/
		class VertexPointer
		{
		public:
			
			
			VertexPointer(const GLuint index,const GLint size,
				const vertexPointerType::VertexPointerType type ,
				const bool normalized,const GLsizei stride,const std::shared_ptr<content::IGLDataResource> ptr );
			
			//������������--���ܻ���Ҫ�޸ġ�����Ϊ��Ҫ�ͳ�������locationһ��
			GLuint index;
			const GLint size;
			const vertexPointerType::VertexPointerType type;
			const GLboolean normalized;
			const GLsizei stride;

			
			const std::shared_ptr<content::IGLDataResource> datasource;

		};

		


	}
}



#endif // !CFL_VERTEXPOINTER_H
