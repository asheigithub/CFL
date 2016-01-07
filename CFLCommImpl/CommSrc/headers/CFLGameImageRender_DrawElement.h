#ifndef CFL_GAMEIMAGERENDER_DRAWELEMENT_H
#define CFL_GAMEIMAGERENDER_DRAWELEMENT_H

#include "CFLGameImageRender.h"
#include "IGameImageEffect.h"

#include <vector>

namespace cfl
{
	namespace graphic
	{
		//绘制一次GameImage的参数
		class drawElement
		{

		private:
			drawElement() :image(nullptr),
				r(1), g(1), b(1), a(1),
				opacity(1), matrix(nullptr),
				scale(1), roation(0), clip(nullptr),
				tx(0), ty(0), 
				blendmode(BlendMode::alphablend),
				filtermode(FilterMode::linear),
				__group(0),
				backmatrix(geom::Matrix3D()),
				backclip(geom::Rectangle()),
				p1(geom::Vector4()), p2(geom::Vector4()), p3(geom::Vector4()), p4(geom::Vector4()),
				_leftu(0), _rightu(0), _topv(0), _bottomv(0),
				_screenleft(0), _screentop(0), _screenright(0), _screenbottom(0),
				effectData()
			{
				
			}
		public:
			float r;
			float g;
			float b;
			float a;

			float opacity;
			GameImage image;
			geom::Matrix3D* matrix;

			float scale;
			float roation;

			geom::Rectangle* clip;

			float  tx ;
			float  ty ;

			BlendMode  blendmode;
			FilterMode filtermode;

		
			int __group;

			geom::Matrix3D  backmatrix;
			geom::Rectangle backclip;

			geom::Vector4 p1;
			geom::Vector4 p2;
			geom::Vector4 p3;
			geom::Vector4 p4;

			float _leftu ;
			float _rightu ;

			float  _topv ;
			float  _bottomv ;


			float  _screenleft ;
			float  _screentop ;
			float  _screenright ;
			float  _screenbottom ;


			GameImageEffectData effectData;


			//绘图指令调用不能多线程！
			static drawElement* getDrawElement();
			static void reset();
			

		};

		static std::vector<drawElement*> elementPool;
		static size_t poolidx;

		drawElement* drawElement::getDrawElement()
		{
			while ((poolidx >= elementPool.size()))
			{
				for (size_t i = 0; i < 1024; i++)
				{
					elementPool.push_back(new drawElement());
				}
			} 
			
			auto d = elementPool[poolidx];
			d->matrix = nullptr;
			d->clip = nullptr;

			++poolidx;
			return d;
		}

		void drawElement::reset()
		{
			static GameImage empty = GameImage(nullptr);

			if (elementPool.size() ==0)
			{
				for (size_t i = 0; i < 1024*8; i++)
				{
					elementPool.push_back( new drawElement() );
				}
			}

			while ( poolidx< elementPool.size() && !elementPool[poolidx]->image.isNull()  )
			{
				elementPool[poolidx]->image = empty;
				elementPool[poolidx]->effectData.reset();
				poolidx++;
			}

			poolidx = 0;
		}


		

		
	}
}



#endif // !CFL_GAMEIMAGERENDER_DRAWELEMENT_H
