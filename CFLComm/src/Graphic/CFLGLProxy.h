#ifndef CFL_GLPROXY_H
#define CFL_GLPROXY_H

#include "../Geom/CFLRectangle.h"
#include "../Graphic/CFLProgram.h"

namespace cfl
{
	namespace graphic
	{
		class Graphic;
	}
	namespace render
	{


		/*
		代理GL操作.内部缓存一些状态切换等
		*/
		class GLProxy
		{
			friend class graphic::Graphic;
		public:

			//刷新缓存状态
			void reset();

			//设置ViewPort;
			void glViewPort(const geom::Rectangle& rect ) ;

			//设置当前着色器程序
			void useProgram(const GLProgram* program );

		private:
			GLProxy(){};


			geom::Rectangle _viewport;

			GLuint _program;

		};

		


	}
}


#endif // !CFL_GLPROXY_H
