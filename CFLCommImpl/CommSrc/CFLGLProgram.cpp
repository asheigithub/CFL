#include "Graphic/CFLProgram.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace render
	{
		
		GLProgram::GLProgram(unsigned int id, std::list<IGLObj*>::iterator iter) :IGLObj(id, iter),
			_isvalid(false),vertex(nullptr),fragment(nullptr),_autodeleteshader(false){};

		GLProgram::~GLProgram()
		{
			close();
		};

		void GLProgram::close()
		{
			
			if (_isvalid)
			{
				_isvalid = false;

				for (auto i = uniforms.begin(); i != uniforms.end(); i++)
				{
					delete i->second;
				}

				uniforms.clear();
				glDeleteProgram(glhandle);
				LOGI("Program(%d) deleted!\n", objId);
			}

		}

		void  GLProgram::doLink()
		{
			CFL_ASSERT(vertex->isValid());
			CFL_ASSERT(fragment->isValid());

			auto g_hShaderProgram = glCreateProgram();
			glAttachShader(g_hShaderProgram, vertex->getHandle());
			glAttachShader(g_hShaderProgram, fragment->getHandle());

			// Link the vertex shader and fragment shader together
			glLinkProgram(g_hShaderProgram);

			// Check for link success
			GLint nLinkResult = 0;
			glGetProgramiv(g_hShaderProgram, GL_LINK_STATUS, &nLinkResult);
			if (!nLinkResult)
			{
				GLchar Log[1024];
				GLint nLength;
				glGetProgramInfoLog(g_hShaderProgram, 1024, &nLength, Log);

				LOGE("Error link glProgram:\n%s\n", Log);

				return;
			}

			

			//当程序连接完成后，即可删除Shader的原始资源....

			LOGI("glProgram(%d) linked!\n", objId);

			if (_autodeleteshader)
			{
				vertex->close();
				fragment->close();
			}
			
			_isvalid = true;
			glhandle = g_hShaderProgram;

			queryUniform();
			queryAttribs();
		}

		void  GLProgram::link(GLShader* const vertexShader, GLShader* const fragmentShader, bool autodeleteshader)
		{
			CFL_ASSERT(vertexShader != nullptr);
			CFL_ASSERT(fragmentShader != nullptr);

			_autodeleteshader = autodeleteshader;

			vertex = vertexShader;
			fragment = fragmentShader;

			auto fun = [this](){
				doLink();
			};

			auto command = new funcDrawCommand< decltype(fun)>(fun,"doLink");
			pushMainLooperCommand(command);
		}

		void GLProgram::onLostEGLContext(CFLContext* context)
		{
			close();
		}
		void GLProgram::onGetNewEGLContext(CFLContext* context)
		{
			if (!_isvalid)
			{
				if (vertex && fragment)
				{
					doLink();
					CFL_ASSERT(_isvalid); //要求修复成功
					LOGI("glProgram(%d) repaired!\n", objId);

				}

			}
			
		}

		ActiveUniformsIV* GLProgram::getUniform(const char* name) const
		{
			auto res= uniforms.find(name);
			if (res != uniforms.end())
			{
				auto ret= (res->second);
				return ret;
			}
			return nullptr;
		}

		ActiveAttributeIV* GLProgram::getAttribute(const char* name) const
		{
			auto res = attribs.find(name);
			if (res != attribs.end())
			{
				auto ret = (res->second);
				return ret;
			}
			return nullptr;
		}

		void GLProgram::queryUniform()
		{
			GLint maxUniformlen;
			GLint numUniforms;
			char* uniformName;
			
			GLuint progObj = glhandle;

			glGetProgramiv(progObj, GL_ACTIVE_UNIFORMS, &numUniforms);
			glGetProgramiv(progObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformlen);

			uniformName = new char[maxUniformlen];

			for (GLint i = 0; i < numUniforms; i++)
			{
				GLint size;
				GLenum type;
				GLint location;

				glGetActiveUniform(progObj, i, maxUniformlen, NULL, &size, &type, uniformName);
				location = glGetUniformLocation(progObj, uniformName);
				
				
				
				uniforms.emplace(std::string(uniformName),new ActiveUniformsIV(i,size, UniformType(type),location ) );
				LOGI("uniform %s ,size:%d, type:%x (%d)\n", uniformName,size, type, location);

			}




			delete uniformName;

		}

		void GLProgram::queryAttribs()
		{
			GLint maxAttribslen;
			GLint numAttribs;
			char* attribName;

			GLuint progObj = glhandle;

			glGetProgramiv(progObj, GL_ACTIVE_ATTRIBUTES, &numAttribs);
			glGetProgramiv(progObj, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribslen);

			attribName = new char[maxAttribslen];

			for (GLint i = 0; i < numAttribs; i++)
			{
				GLint size;
				GLenum type;
				GLint location;

				glGetActiveAttrib(progObj, i, maxAttribslen, NULL, &size, &type, attribName);
				location = glGetAttribLocation(progObj, attribName);



				attribs.emplace(std::string(attribName), new ActiveAttributeIV(i, size, AttributeType(type), location));
				LOGI("attrib %s ,size:%d, type:%x (%d)\n", attribName, size, type, location);

			}




			delete attribName;

		}
	}
}
