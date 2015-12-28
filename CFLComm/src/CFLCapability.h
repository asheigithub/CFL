#ifndef CFL_CAPABILITY_H
#define CFL_CAPABILITY_H

#include <vector>
#include <string>
#include <sstream>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif

#include "Content/CFLEndian.h"

namespace cfl
{

	/*
	������OpenGLES����
	*/
	class Capability
	{
	public:
		Capability()
			:_glver(2),
			_hasinited(false),
			_compressed_texure_formats(std::vector<int>()),
			_extensions(std::vector<std::string>()),
			endian(content::Endian::little_endian)
		{};
		
		void init();
		
		//��ѯ�����Ĵ�С����
		content::Endian getEndian() const { return endian; };


		//��ѯ�Ƿ�֧�ְ뾫����չ
		bool is_OES_vertex_half_float();
		
		int getGlVer() const { return _glver; }

	private:
		int _glver;

		content::Endian endian;

		bool _hasinited;

		std::vector<int> _compressed_texure_formats;

		std::vector<std::string> _extensions;

	};

	
}


#endif // !CFL_CAPABILITY_H
