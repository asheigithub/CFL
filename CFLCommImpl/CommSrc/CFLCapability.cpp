#include "CFLCapability.h"

#include "CFLEntry.h"

namespace cfl
{
	void Capability::init()
	{
		if (!_hasinited)
		{
			_hasinited = true;

			//**本机字节序查询**
			union NUM
			{
				int a;
				char b;
			}num;
			
			num.a = 0x1234;
			if (num.b == 0x12)
			{
				endian = content::Endian::big_endian;
			}
			else
			{
				endian = content::Endian::little_endian;
			}




			{//纹理压缩格式查询//
				GLint num;
				glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
				GLint* fs = new GLint[num]; //malloc(sizeof(GLint) * num);
				glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, fs);
				for (GLint i = 0; i < num; i++)
				{
					_compressed_texure_formats.push_back(*(fs + i));
					LOGI("GL_COMPRESSED_TEXTURE_FORMATS: %x\n", *(fs + i));
				}
				delete[] fs;
			}

			auto glver = reinterpret_cast<const char*>(glGetString(GL_VERSION));
			if (glver != nullptr)
			{
				LOGI("glGetString %s\n", glver);
			}
			else
			{
				LOGI("glGetString NULL !!?\n");
			}
#ifdef WIN32
			_glver = 3;
#else
			if (strstr(glver, "OpenGL ES 3.0") && gl3stubInit() == GL_TRUE)
			{
				_glver = 3;
				LOGI("ES3\n");

			}
			else
			{
				_glver = 2;
				LOGI("ES2\n");

			}

#endif

			auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			if (renderer != nullptr)
			{
				_renderer = renderer;
			}

			auto exts = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
			if (exts != nullptr)
			{
				//std::string input = exts;
				std::stringstream ss(exts);

				std::string str;
				while (std::getline(ss, str, ' '))
				{
					if (str.empty())
					{
						break;
					}

					str.erase(0, str.find_first_not_of(" "));
					str.erase(str.find_last_not_of(" ") + 1);

					_extensions.push_back(str);

					LOGI("GL_EXTENSIONS %s\n", str.c_str());

				}


			}

			CFLContext::getCurrentInstance()->graphic->_onCapabilityInited(this);
		}
	}


	bool Capability::is_OES_vertex_half_float()
	{
		static bool issupport = false;
		static bool hasquery = false;

		if (hasquery)
		{
			return issupport;
		}
		else
		{
			for (auto i = _extensions.begin(); i != _extensions.end(); i++)
			{
				if ((*i) == "GL_OES_vertex_half_float")
				{
					issupport=true;
					break;
				}
			}
			hasquery = true;
			return issupport;

		}

		
	}

}