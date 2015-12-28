#ifndef GameImage_Separate_TinyColor_Shader_H
#define GameImage_Separate_TinyColor_Shader_H

#define STRINGIFY(A)  #A

namespace cfl
{

	namespace graphic
	{
		static const char* gameimage_separate_tinycolor_vert =
			"#version 100\n"
			STRINGIFY(
			uniform mat4 vp_matrix;

			attribute vec4 vPosition;
			attribute vec4 color;
			attribute vec2 uv;

			varying vec4 tiny_color;
			varying vec2 to_uv;
			void main()
			{
				gl_Position = vp_matrix * vPosition;
				tiny_color = color;
				to_uv = uv;
			}

		);

		static const char* gameimage_separate_tinycolor_frag = STRINGIFY(
			precision mediump float;
			uniform sampler2D s_texture;
			uniform sampler2D a_texture;
			varying vec4 tiny_color;
			varying vec2 to_uv;
			void main()
			{
				vec4 texcolor = texture2D(s_texture, to_uv);
				vec4 texalpha = texture2D(a_texture, to_uv);

				texcolor.w = texalpha.x;

				gl_FragColor = texcolor * tiny_color;

			}
		);


	}
}

#undef STRINGIFY


#endif // !GameImage_Separate_TinyColor_Shader_H
