#ifndef CFL_TEXTUREBASE_H
#define CFL_TEXTUREBASE_H

#include "CFLGLTex2D.h"


namespace cfl
{
	namespace render
	{
		namespace textures
		{
			//TextureBase类为纹理对象的基类。 
			class TextureBase
			{
			public:
				//手工释放与此纹理关联的资源。 释放后将不能再对此纹理进行操作
				virtual void dispose() = 0;
			protected:
				TextureBase(){};

				virtual ~TextureBase() = default;

			};

		}


	}
}



#endif // !CFL_TEXTUREBASE_H
