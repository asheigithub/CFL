#ifndef CFL_PROGRAM_H
#define CFL_PROGRAM_H


#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif

#include "CFLGLObj.h"
#include "CFLShader.h"

#include <unordered_map>

namespace cfl
{
	namespace render
	{
		namespace uniformType
		{ 
			//统一变量的类型
			enum UniformType
			{
				//es2

				glFloat= GL_FLOAT,
				glFloat_Vec2= GL_FLOAT_VEC2,
				glFloat_Vec3= GL_FLOAT_VEC3, 
				glFloat_Vec4= GL_FLOAT_VEC4,
				glInt= GL_INT,
				glInt_Vec2= GL_INT_VEC2,
				glInt_Vec3= GL_INT_VEC3,
				glInt_Vec4= GL_INT_VEC4,
				glBool= GL_BOOL,
				glBool_Vec2= GL_BOOL_VEC2,
				glBool_Vec3= GL_BOOL_VEC3,
				glBool_Vec4= GL_BOOL_VEC4,
				glFloat_Mat2= GL_FLOAT_MAT2,
				glFloat_Mat3= GL_FLOAT_MAT3,
				glFloat_Mat4= GL_FLOAT_MAT4,
				glSampler2D= GL_SAMPLER_2D,
				glSamplerCUBE= GL_SAMPLER_CUBE,

				//es3   es3部分暂时先放着
				/*glUnsignedInt = GL_UNSIGNED_INT,
				glUnsignedInt_Vec2 = GL_UNSIGNED_INT_VEC2,
				glUnsignedInt_Vec3 = GL_UNSIGNED_INT_VEC3,
				glUnsignedInt_Vec4 = GL_UNSIGNED_INT_VEC4,
				glFloatMat2x3 =GL_FLOAT_MAT2x3,*/


			};
		}

		namespace attributeType
		{ 
			//顶点属性类型
			enum AttributeType
			{
				//es2
				glFloat= GL_FLOAT,
				glFloat_Vec2= GL_FLOAT_VEC2,
				glFloat_Vec3= GL_FLOAT_VEC3,
				glFloat_Vec4= GL_FLOAT_VEC4,
				glFloat_Mat2= GL_FLOAT_MAT2,
				glFloat_Mat3= GL_FLOAT_MAT3,
				glFloat_Mat4= GL_FLOAT_MAT4

				//es3部分先放着
			};

		}

		using namespace uniformType;
		//活动统一变量信息
		struct ActiveUniformsIV
		{
			ActiveUniformsIV(GLuint index, GLsizei size, UniformType type, GLint location)
				:indices(index), size(size), type(type), location(location)
			{}

			GLuint indices;
			GLsizei size;
			UniformType type;
			GLint       location;
		};

		using namespace attributeType;
		//活动属性信息
		struct ActiveAttributeIV
		{
			ActiveAttributeIV(GLuint index, GLsizei size, AttributeType type, GLint location)
				:indices(index), size(size), type(type), location(location)
			{}
			GLuint indices;
			GLsizei size;
			AttributeType type;
			GLint         location;
		};


		//着色器程序。
		class GLProgram :public IGLObj
		{
			friend class GLObjCollection;

		public:

			void link( GLShader* const vertexShader,  GLShader* const fragmentShader,bool autodeleteshader=true);

			

			inline bool isValid() override final{ return _isvalid; };

			void close() override final;

			ActiveUniformsIV* getUniform(const char* name) const;
			ActiveAttributeIV* getAttribute(const char* name) const;
		private:
			GLProgram(unsigned int id, std::list<IGLObj*>::iterator iter);
			~GLProgram();

			void onLostEGLContext(CFLContext* context) override final;

			void onGetNewEGLContext(CFLContext* context) override final;

			bool _isvalid;

			bool _autodeleteshader;//链接完毕后是否自动删除Shader.

			GLShader* vertex;
			GLShader* fragment;

			void doLink();

			std::unordered_map<std::string, ActiveUniformsIV*> uniforms;
			std::unordered_map<std::string, ActiveAttributeIV*> attribs;

			void queryUniform();
			void queryAttribs();
		};


	}
}




#endif // !CFL_PROGRAM_H
