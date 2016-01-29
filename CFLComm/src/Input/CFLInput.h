#ifndef CFLINPUT_H
#define CFLINPUT_H

#include "CFLTouch.h"

namespace cfl
{
	namespace input
	{
		class Input
		{
		public:
			//��ǰ����������������λ�á�GUI����ϵ���Ͻ�Ϊ(0,0)
			static cfl::geom::Vector2 mousePositon();

			/*
			��ָ������갴ť������ʱ����true
			buttonֵ�趨Ϊ 0��Ӧ��� �� 1��Ӧ�Ҽ� �� 2��Ӧ�м���
			*/
			static bool getMouseButton(unsigned int button);

			/*
			���û�����ָ����갴������һ֡����true��
			����Ҫ��ÿ֡�����е�������������˺�ÿһ֡����״̬ʱ��
			�������᷵��true�����û��ͷ������갴ťȻ�����°�������
			��ťֵ�趨Ϊ 0��Ӧ��� �� 1��Ӧ�Ҽ� �� 2��Ӧ�м���
			*/
			static bool getMouseButtonDown(unsigned int button);
			/*
			���û��ͷ�ָ����갴������һ֡����true��
			����Ҫ��ÿ֡�����е�������������˺�ÿһ֡����״̬ʱ��
			�������᷵��true�����û��ͷ������갴ťȻ�����°�������
			��ťֵ�趨Ϊ 0��Ӧ��� �� 1��Ӧ�Ҽ� �� 2��Ӧ�м���
			*/
			static bool getMouseButtonUp(unsigned int button);

		};

		
	}
}




#endif // !CFLINPUT_H
