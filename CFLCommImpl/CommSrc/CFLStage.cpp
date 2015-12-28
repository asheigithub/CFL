#include "CFLStage.h"
#include "CFLEntry.h"
namespace cfl
{
	Stage::Stage() :
		_fullscreenwidth(0),
		_fullscreenheight(0),
		_stageWidth(0),
		_stageHeight(0),
		_scaleMode(StageScaleMode::SHOW_ALL),
		_hasinited(false)
	{
	}

	Stage::~Stage()
	{
	}

	void Stage::_onGLinit(CFLContext* ctx)
	{
		_fullscreenwidth = ctx->width;
		_fullscreenheight = ctx->height;

		if (!_hasinited)
		{
			_hasinited = true;

			_stageWidth = _fullscreenwidth;
			_stageHeight = _fullscreenheight;
		}

	}

	void Stage::stageWidth(size_t v)
	{
		_hasinited = true;
		_stageWidth = v;
	}

	void Stage::stageHeight(size_t v)
	{
		_hasinited = true;
		_stageHeight = v;
	}

	void Stage::scaleMode(StageScaleMode mode)
	{
		_hasinited = true;
		_scaleMode = mode;
	}

	geom::Rectangle Stage::getGLViewPort() const
	{
		switch (_scaleMode)
		{
		case cfl::EXACT_FIT:

			return geom::Rectangle(0.0f, 0.0f, (float)fullScreenWidth(), (float)fullScreenHeight());
			break;
		case cfl::SHOW_ALL:
			{
				float wratio = (float)fullScreenWidth() / stageWidth();
				float rratio = (float)fullScreenHeight() / stageHeight();

				if (wratio > rratio)
				{
					float fixwidth = rratio * stageWidth();
					float fixheight = rratio * stageHeight();

					return geom::Rectangle( (fullScreenWidth()-fixwidth)/2  ,0.0f , fixwidth, fixheight);
				}
				else
				{
					float fixwidth = wratio * stageWidth();
					float fixheight = wratio * stageHeight();

					return geom::Rectangle(0.0f, (fullScreenHeight()-fixheight)/2  , fixwidth, fixheight);
				}

				
			}

			break;
		case cfl::NO_BORDER:
			{
				float wratio = (float)fullScreenWidth() / stageWidth();
				float rratio = (float)fullScreenHeight() / stageHeight();

				if (wratio < rratio)
				{
					float fixwidth = rratio * stageWidth();
					float fixheight = rratio * stageHeight();

					return geom::Rectangle((fullScreenWidth() - fixwidth) / 2, 0.0f, fixwidth, fixheight);
				}
				else
				{
					float fixwidth = wratio * stageWidth();
					float fixheight = wratio * stageHeight();

					return geom::Rectangle(0.0f, (fullScreenHeight() - fixheight) / 2, fixwidth, fixheight);
				}
			}
			break;
		case cfl::NO_SCALE:
			return geom::Rectangle(((float)fullScreenWidth()-stageWidth())/2.0f, ((float)fullScreenHeight()-stageHeight())/2.0f, (float)stageWidth(), (float)stageHeight());
			break;
		default:
			return geom::Rectangle(0.0f, 0.0f, (float)fullScreenWidth(), (float)fullScreenHeight());
			break;
		}
	}

}