#ifndef CFLTOUCH_H
#define CFLTOUCH_H

#include "CFLTouchPhase.h"
#include "../Geom/CFLGeom.h"
namespace cfl
{
	namespace input
	{



		/*
		������¼һ����ָ��������Ļ�ϵ�״̬��
		*/
		struct Touch
		{
			Touch() :fingerId(0), 
				position(0,0),
				deltaPosition(0,0),
				deltaTime(0),
				tapCount(1),
				phase( touchPhase::Canceled )
			{
				
			}

			//������Ψһ������
			int fingerId;
			//������λ�á� 
			geom::Vector2 position;
			//�����ϴθı�ľ��������� 
			geom::Vector2 deltaPosition;
			//���ϴθı��ʱ�������� 
			float deltaTime;
			//��������� 
			int tapCount;

			//����������״̬��
			touchPhase::TouchPhase phase;

		};

	}
}


#endif // !CFLTOUCH_H
