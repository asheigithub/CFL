#include "Graphic/CFLShader.h"
#include "CFLEntry.h"
#include "Graphic/CFLGraphic.h"
namespace cfl
{
	namespace render
	{
		GLShader::GLShader(unsigned int id, std::list<IGLObj*>::iterator iter) :IGLObj(id, iter),
			_isvalid(false),sourcecode(nullptr){};

		GLShader::~GLShader()
		{
			if (sourcecode)
			{
				delete sourcecode;
				sourcecode = nullptr;
			}
			
			close();
		};

		void GLShader::close()
		{
			if (_isvalid)
			{
				_isvalid = false;
				glDeleteShader(glhandle);

				LOGI("Shader(%d) deleted!\n", objId);
			}
			
		}

		void GLShader::doCompile()
		{
			
			GLuint shader;
			GLint compiled;

			// Create the shader object
			shader = glCreateShader(shadertype);

			if (shader == 0)
			{
				LOGE("shader create failed!");
				return;
			}
			// Load the shader source
			glShaderSource(shader, 1, &sourcecode, NULL);

			// Compile the shader
			glCompileShader(shader);

			// Check the compile status
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				GLint infoLen = 0;

				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					GLchar *infoLog = new GLchar[infoLen];
					GLsizei reallen = 0;
					glGetShaderInfoLog(shader, infoLen, &reallen, infoLog);
					infoLog[reallen] = '\0';

					LOGE("Error compiling shader:\n%s\n", infoLog);

					delete[] infoLog;

				}

				glDeleteShader(shader);
				return;
			}

			glhandle = shader;
			_isvalid = true;

			LOGI("Shader(%d) created!\n", objId);
			
		}

		void GLShader::compile(const GLchar* source, ShaderType type)
		{
			
			CFL_ASSERT(source != nullptr);
			auto len = strlen(source);
			sourcecode = new GLchar[len + 1];
			memcpy(sourcecode, source, len);
			sourcecode[len] = '\0';

			shadertype = type;

			auto fun = [this](){
				doCompile();
			};

			auto command = new  funcDrawCommand< decltype(fun)>(fun,"compileshader");
			pushMainLooperCommand(command);

		}


		void GLShader::onLostEGLContext(CFLContext* context)
		{
			close();
		}
		void GLShader::onGetNewEGLContext(CFLContext* context)
		{
			if (!_isvalid)
			{
				if (sourcecode != nullptr)
				{
					this->doCompile();

					CFL_ASSERT(_isvalid); //要求修复成功

					LOGI("Shader(%d) repaired!\n", objId);
				}
			}

			
		}
	}
}
