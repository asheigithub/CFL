#ifndef GameImageAtlasTinyColor_H
#define GameImageAtlasTinyColor_H

#define STRINGIFY(A)  #A
namespace cfl
{

	namespace graphic
	{
		static const char* gameimage_atlas_tinycolor_vert =
			"#version 100\n"
			STRINGIFY(
			uniform mat4 vp_matrix;
			attribute vec4 vPosition;
			attribute vec4 color;
			attribute vec2 uv;
			varying vec4 tiny_color;
			varying vec2 to_uv;
			varying vec2 to_auv;
			void main()
			{
				gl_Position = (vp_matrix * vPosition);
				tiny_color = color;
				vec2 tmpvar_1;
				tmpvar_1.x = uv.x;
				float tmpvar_2;
				tmpvar_2 = (uv.y * 0.5);
				tmpvar_1.y = tmpvar_2;
				to_uv = tmpvar_1;
				vec2 tmpvar_3;
				tmpvar_3.x = uv.x;
				tmpvar_3.y = (tmpvar_2 + 0.5);
				to_auv = tmpvar_3;
			}

		);

		static const char* gameimage_atlas_tinycolor_frag = STRINGIFY(
			precision mediump float;
			uniform sampler2D s_texture;
			varying vec4 tiny_color;
			varying vec2 to_uv;
			varying vec2 to_auv;
			void main()
			{
				lowp vec4 texcolor_1;
				texcolor_1.xyz = texture2D(s_texture, to_uv).xyz;
				texcolor_1.w = texture2D(s_texture, to_auv).x;
				gl_FragColor = (texcolor_1 * tiny_color);
			}
		);
	}
}

#undef STRINGIFY

#endif // !GameImageAtlasTinyColor_H
