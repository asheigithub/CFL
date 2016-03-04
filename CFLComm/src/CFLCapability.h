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
#include "CFLString.h"

namespace cfl
{

	/*
	本机的OpenGLES能力
	*/
	class Capability
	{
	public:
		Capability()
			:_glver(2),
			_renderer(),
			_hasinited(false),
			_compressed_texure_formats(std::vector<int>()),
			_extensions(std::vector<std::string>()),
			endian(content::Endian::little_endian)
		{};
		
		void init();
		
		//查询本机的大小端序
		content::Endian getEndian() const { return endian; };


		//查询是否支持半精度扩展
		bool is_OES_vertex_half_float();
		
		int getGlVer() const { return _glver; };

		CFLString getRenderer() const { return _renderer; };

	private:
		int _glver;

		CFLString _renderer;

		content::Endian endian;

		bool _hasinited;

		std::vector<int> _compressed_texure_formats;

		std::vector<std::string> _extensions;

	};

	
}


#endif // !CFL_CAPABILITY_H
