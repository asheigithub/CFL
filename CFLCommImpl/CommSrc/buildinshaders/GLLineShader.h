//定义画线的shader
#ifndef CFL_GLLINESHADER_H
#define CFL_GLLINESHADER_H

#define STRINGIFY(A)  #A

namespace cfl
{

	namespace graphic
	{

		static const char* gl_line_shader_vert = 
			"#version 100\n"
			STRINGIFY(
			attribute vec4 a_position;
			attribute vec4 line_color;
			varying vec4 to_color;
			void main()
			{
				gl_Position = a_position;
				to_color = line_color;
			}

		);

		static const char* gl_line_shader_frag = STRINGIFY(
			precision mediump float;
			varying vec4 to_color;
			void main()
			{
				
				gl_FragColor = to_color;

			}
		);


	}
}

#undef STRINGIFY

#endif // !CFL_GLLINESHADER_H