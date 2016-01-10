#ifndef GAMEIMAGE_SDFSHADER_H
#define GAMEIMAGE_SDFSHADER_H

#define STRINGIFY(A)  #A


namespace cfl
{

	namespace graphic
	{
		static const char* gameimage_sdfshader_vert =
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

		static const char* gameimage_sdfshader_frag = STRINGIFY(
			precision mediump float;
			uniform sampler2D s_texture;
			uniform float smoothrange;
			varying vec4 tiny_color;
			varying vec2 to_uv;
			void main()
			{
				//vec4 texcolor = texture2D(s_texture, to_uv);
				float dist = texture2D(s_texture, to_uv).x;
				float alpha = smoothstep(0.8 - smoothrange, 0.8 + smoothrange, dist);

				vec4 texcolor = vec4(1.0,1.0,1.0,alpha);

				gl_FragColor = texcolor *tiny_color;

			}
		);

	}
}




#undef STRINGIFY

#endif // !GAMEIMAGE_SDF_H