#include "Graphic/CFLGraphic.h"
#include "headers/CFLLineRender.h"
#include "headers/CFLGameImageRender.h"

#include "CFLEntry.h"
#include "headers/FontTexture.h"
#include "headers/GameImageEffectNormal.h"
#include "headers/GameImageEffectSDF.h"


namespace cfl
{
	namespace graphic
	{
		using namespace cfl::render;

		static geom::Matrix drawline_mtx;//转换画线坐标的矩阵


		struct Graphic::g
		{
			g(RenderThreadLooper* looper,GLProxy* glproxy) :renderLooper(looper),
				inited(false),
				totalframes(0),
				lastInvokeRender(nullptr),
				gllinerender( nullptr),
				gameimagerender(nullptr),
				glProxy(glproxy)
			{}

			~g()
			{
				if (gllinerender)
				{
					delete gllinerender;
					gllinerender = nullptr;
				}
				if (gameimagerender)
				{
					delete gameimagerender;
					gameimagerender = nullptr;
				}
				if (glProxy)
				{
					delete glProxy;
					glProxy = nullptr;
				}
			}

			void pushCommand(IdrawCommand* cmd)
			{
				renderLooper->pushCommand(cmd);
			}

			//将一个渲染器设为激活。。如果没有改变，返回false,如果有改变，返回true
			bool activeRender(IGraphicRender* render)
			{
				bool ret = (lastInvokeRender != render);

				if (lastInvokeRender != render)
				{
					if (lastInvokeRender)
					{
						lastInvokeRender->deActive(CFLContext::getCurrentInstance());
					}
					if (render)
					{
						render->onActive(CFLContext::getCurrentInstance());
					}
				}

				lastInvokeRender = render;

				return ret;

			}


			RenderThreadLooper* const renderLooper;
			bool inited;
			size_t totalframes;

			GLProxy* glProxy;

			IGLLineRender* gllinerender;//画线器
			IGameImageRender* gameimagerender;//GameImage绘制器

		private:

			IGraphicRender* lastInvokeRender;//上次活动的绘制器

			
		};

		//每帧渲染队列交换前被调用
		void Graphic::onBeforRenderQueueSwap(CFLContext* ctx)
		{
			//动态更新需要在交换render前完成，否则本帧无法生效
			
			font::updateFontTexture();
			
			

			_g->totalframes = ctx->totalframes;
			_g->activeRender(nullptr);

			_g->gllinerender->swapRenderQueue(ctx);
			_g->gameimagerender->swapRenderQueue(ctx);

			

		}

		//每帧渲染队列交换后被调用。
		void Graphic::onRednerQueueSwaped(CFLContext* ctx)
		{
			//转换画线的矩阵
			drawline_mtx.identity();
			drawline_mtx.d = -1;
			drawline_mtx.scale(2.0f / ctx->stage->stageWidth(), 2.0f / ctx->stage->stageHeight());
			drawline_mtx.translate(-1.0f, 1.0f);


			auto rect = ctx->stage->getGLViewPort();
			getGLProxy()->glViewPort(rect);

		}
		
		void Graphic::_onGlInit(CFLContext* ctx)
		{
			ctx->stage->_onGLinit(ctx);

			if (!_g->inited)
			{
				
				_g->gllinerender->onGLinited(ctx);

				_g->gameimagerender->onGLinited(ctx);

				_g->inited = true;
			}
			_g->totalframes = ctx->totalframes;

			_g->glProxy->reset();
		}

		Graphic::Graphic(render::RenderThreadLooper* looper) :_g(new g(looper , new GLProxy() ))
		{
			
		}
		Graphic::~Graphic()
		{
			delete _g;
		}

		render::GLProxy* Graphic::getGLProxy() const
		{
			return _g->glProxy;
		}


		void Graphic::_onCapabilityInited(cfl::Capability* capablilty) //当capability加载完成后被Capability调用
		{
			//**此时即可选择画线器half或者float版本**
			if (capablilty->getGlVer() >= 3 || capablilty->is_OES_vertex_half_float() )
			{
				_g->gllinerender = new HalfFloatGLLineRender(_g->renderLooper,this);
			}
			else
			{
				_g->gllinerender = new GLLineRender(_g->renderLooper,this);
			}
			
			_g->gameimagerender = new GameImageRender(_g->renderLooper,this);

			//capablilty->getGlVer();
		}

		void Graphic::clear(float r, float g, float b, float a, float depth, int stencil, clearMaskBitValue clearmask)
		{
			auto fun =[=](){
				
				if (clearmask & ClearMaskBit::color)
				{
					glClearColor(r, g, b, a);
				}
				if (clearmask & ClearMaskBit::depth)
				{
					glClearDepthf(depth);
				}
				if (clearmask & ClearMaskBit::stencil)
				{
					glClearStencil(stencil);
				}
				glClear(clearmask);

			};
			
			auto command =new funcDrawCommand< decltype(fun)>(fun,"clear screen");

			_g->pushCommand(command);
		}



		void Graphic::drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,const Color& color, float linewidth)
		{
			auto ischange = _g->activeRender(_g->gllinerender);

			auto stp = stpos * drawline_mtx;
			auto edp = edpos * drawline_mtx;

			if (!ischange)
			{
				_g->gllinerender->appendLine(stp, edp,color,linewidth);
			}
			else
			{
				_g->gllinerender->drawLine(stp, edp,color,linewidth);
			}

			

		}

		static void doDrawGameImage(const GameImage& image,
			float tx,
			float ty,
			const cfl::geom::Matrix3D* matrix,
			float opacity,
			const cfl::geom::Rectangle* clip,
			float scale,
			float roation,
			const Color* color,
			FilterMode filter,
			BlendMode blendmode,
			Graphic::g* _g,
			IGameImageEffect* effect = GameImageEffectNormal::getInstance(),
			GameImageEffectDX effectDx = nullptr
			)
		{
			if (scale <= 0)
			{
				return;
			}

			if (image.isNull())
			{
				LOGI("GameImage为空!\n");
				return;
			}

			if (clip != nullptr && clip->width <= 0)
			{
				return;
			}
			else if (clip != nullptr)
			{
				//var imgrect:Rectangle = new Rectangle(image.PadLeft, image.PadTop, image.ClipWidth, image.ClipHeight );
				if (clip->getRight() < image->padLeft ||
					clip->getLeft() >= image->padLeft + image->clipWidth ||
					clip->getTop() >= image->padTop + image->clipHeight ||
					clip->getBottom() < image->padTop)
				{
					return;
				}

			}



			if (image->clipWidth == 0 || image->clipHeight == 0)
			{
				return;
			}




			image->refTexture->setLastRefCounter(_g->totalframes);

			if (image->refTexture->isFailed())
			{
				return;
			}
			else if (!image->refTexture->isDone())
			{
				image->refTexture->upload();// true);

				//return;
			}


			auto isappend = !(_g->activeRender(_g->gameimagerender));

			_g->gameimagerender->drawGameImage(isappend,
				std::move(image),
				tx, ty,
				matrix, opacity, clip,
				scale, roation, color,
				filter, blendmode, effect,effectDx
				);


		}


		void Graphic::drawGameImage(const GameImage& image,
			float tx,
			float ty,
			const cfl::geom::Matrix3D* matrix,
			float opacity,
			const cfl::geom::Rectangle* clip,
			float scale,
			float roation,
			const Color* color,
			FilterMode filter,
			BlendMode blendmode)
		{
			doDrawGameImage(image, tx, ty, matrix, opacity, clip, scale, roation, color, filter, blendmode,_g);
		}

		void Graphic::drawString(const GameImage& image, float tx, float ty, float size)
		{
			

			doDrawGameImage(image, tx, ty, nullptr, 1, nullptr, size/1024, 0, nullptr,
				FilterMode::linear, BlendMode::alphablend, _g, GameImageEffectSDF::getInstance(size), EffectSDFDX);



		}


	}
}