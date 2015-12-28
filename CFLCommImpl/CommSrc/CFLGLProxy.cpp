#include "Graphic/CFLGLProxy.h"
#include "CFLDrawCommand.h"

#include "CFLEntry.h"



namespace cfl
{
	
	namespace render
	{

		void GLProxy::reset()
		{
			_viewport = geom::Rectangle();
			_program = 0;
		}

		void GLProxy::glViewPort(const geom::Rectangle& rect) 
		{


			if (_viewport != rect)
			{
				_viewport = rect;
				glViewport((GLint)rect.x, (GLint)rect.y, (GLint)rect.width, (GLint)rect.height);
			}
		}

		void GLProxy::useProgram(const GLProgram* program)
		{
			if (_program != program->getHandle())
			{
				glUseProgram(program->getHandle());
				_program = program->getHandle();
			}
			
		}


	}
}