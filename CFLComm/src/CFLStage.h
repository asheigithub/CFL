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
	StageScaleMode ����ָ��Ҫʹ����������ģʽ��ֵ�� 
	*/
	enum StageScaleMode
	{
		//����Ӧ�ó�����ָ�������пɼ����������Ա���ԭʼ�߿�ȡ����ܻᷢ��Ť����Ӧ�ó�����ܻ������ѹ����ʾ
		EXACT_FIT,
		//����Ӧ�ó�����ָ�������пɼ����Ҳ�����Ť����ͬʱ����Ӧ�ó����ԭʼ�߿�ȡ�Ӧ�ó����������ܻ���ʾ�߿� 
		SHOW_ALL,
		//����Ӧ�ó�������ָ�����򣬲�����Ť�������п��ܽ���һЩ���У�ͬʱ����Ӧ�ó����ԭʼ�߿�ȡ� 
		NO_BORDER,
		//����Ӧ�ó���Ĵ�С�̶�����ˣ���ʹ���������ڵĴ�С���ģ���Ҳ�ᱣ�ֲ��䡣������������ڱ�����С������ܽ���һЩ���С� 
		NO_SCALE
	};

	struct CFLContext;
	/*
	Stage ���������ͼ���� 
	*/
	class Stage
	{
		friend struct CFLContext;
		friend class graphic::Graphic;
	public:
		

		//���ظ����ڵĿ��
		size_t fullScreenWidth() const{ return _fullscreenwidth; }

		//���ظ����ڵĸ߶�
		size_t fullScreenHeight() const{ return _fullscreenheight; }

		//��ȡ��������̨�Ŀ��
		size_t stageWidth() const{ return _stageWidth; }
		void stageWidth(size_t v);

		//��ȡ��������̨�ĸ߶�
		size_t stageHeight() const{ return _stageHeight; }
		void   stageHeight(size_t v);

		//ָ��Ҫʹ����������ģʽ��ֵ��
		StageScaleMode scaleMode() const{ return _scaleMode; }
		void		scaleMode(StageScaleMode mode );

		//����GLViewPort��
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
