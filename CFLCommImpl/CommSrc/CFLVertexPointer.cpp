#include "Graphic/CFLVertexPointer.h"



namespace cfl
{
	namespace render
	{
		using namespace content;

		VertexPointer::VertexPointer(GLuint index, 
			const GLint size,
			const vertexPointerType::VertexPointerType type,
			const bool normalized, const GLsizei stride, const std::shared_ptr<IGLDataResource> ptr)
			:index(index),
			size(size),
			type(type),
			normalized( normalized?GL_TRUE:GL_FALSE ),
			stride(stride),
			datasource(ptr)
		{
			
		}

		
	}
}
