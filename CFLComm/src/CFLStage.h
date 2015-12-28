#ifndef CFL_STATE_H
#define CFL_STATE_H

#include "Graphic/CFLColor.h"
#include "Geom/CFLRectangle.h"
namespace cfl
{
	namespace graphic
	{
		class Graphic;
	}

	/*
	StageScaleMode 类中指定要使用哪种缩放模式的值。 
	*/
	enum StageScaleMode
	{
		//整个应用程序在指定区域中可见，但不尝试保持原始高宽比。可能会发生扭曲，应用程序可能会拉伸或压缩显示
		EXACT_FIT,
		//整个应用程序在指定区域中可见，且不发生扭曲，同时保持应用程序的原始高宽比。应用程序的两侧可能会显示边框。 
		SHOW_ALL,
		//整个应用程序填满指定区域，不发生扭曲，但有可能进行一些裁切，同时保持应用程序的原始高宽比。 
		NO_BORDER,
		//整个应用程序的大小固定，因此，即使播放器窗口的大小更改，它也会保持不变。如果播放器窗口比内容小，则可能进行一些裁切。 
		NO_SCALE
	};

	struct CFLContext;
	/*
	Stage 类代表主绘图区。 
	*/
	class Stage
	{
		friend struct CFLContext;
		friend class graphic::Graphic;
	public:
		

		//返回父窗口的宽度
		size_t fullScreenWidth() const{ return _fullscreenwidth; }

		//返回父窗口的高度
		size_t fullScreenHeight() const{ return _fullscreenheight; }

		//获取或设置舞台的宽度
		size_t stageWidth() const{ return _stageWidth; }
		void stageWidth(size_t v);

		//获取或设置舞台的高度
		size_t stageHeight() const{ return _stageHeight; }
		void   stageHeight(size_t v);

		//指定要使用哪种缩放模式的值。
		StageScaleMode scaleMode() const{ return _scaleMode; }
		void		scaleMode(StageScaleMode mode );

		//计算GLViewPort。
		geom::Rectangle getGLViewPort() const;

	private:
		Stage();
		~Stage();

		void _onGLinit(CFLContext* ctx);

		size_t _fullscreenwidth;
		size_t _fullscreenheight;

		size_t _stageWidth;
		size_t _stageHeight;

		StageScaleMode _scaleMode;

		bool _hasinited;

	};

}




#endif // !CFL_STATE_H
