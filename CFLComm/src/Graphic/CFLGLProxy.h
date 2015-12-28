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
		����GL����.�ڲ�����һЩ״̬�л���
		*/
		class GLProxy
		{
			friend class graphic::Graphic;
		public:

			//ˢ�»���״̬
			void reset();

			//����ViewPort;
			void glViewPort(const geom::Rectangle& rect ) ;

			//���õ�ǰ��ɫ������
			void useProgram(const GLProgram* program );

		private:
			GLProxy(){};


			geom::Rectangle _viewport;

			GLuint _program;

		};

		


	}
}


#endif // !CFL_GLPROXY_H
