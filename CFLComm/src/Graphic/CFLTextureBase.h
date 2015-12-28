#ifndef CFL_TEXTUREBASE_H
#define CFL_TEXTUREBASE_H

#include "CFLGLTex2D.h"


namespace cfl
{
	namespace render
	{
		namespace textures
		{
			//TextureBase��Ϊ�������Ļ��ࡣ 
			class TextureBase
			{
			public:
				//�ֹ��ͷ���������������Դ�� �ͷź󽫲����ٶԴ�������в���
				virtual void dispose() = 0;
			protected:
				TextureBase(){};

				virtual ~TextureBase() = default;

			};

		}


	}
}



#endif // !CFL_TEXTUREBASE_H
