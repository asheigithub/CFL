#ifndef CFLTOUCHPHASE_H
#define CFLTOUCHPHASE_H

namespace cfl
{
	namespace input
	{
		namespace touchPhase
		{
			/*
			������ָ������״̬��
			*/
			enum TouchPhase
			{
				/*
				��ָ�Ѵ�����Ļ��
				*/
				Began,
				/*
				��ָ����Ļ���ƶ���
				*/
				Moved,
				/*
				��ָ������Ļ������û���ƶ���
				*/
				Stationary,
				/*
				��ָ����Ļ���ƿ�������һ�����������״̬�� 
				*/
				Ended,
				/*
				ϵͳȡ�����ٴ��������û�����Ļ�ŵ������ϻ򳬹�����Ӵ�ͬʱ����������һ�����������״̬�� 
				*/
				Canceled

			};


		}

	}

}


#endif // !CFLTOUCHPHASE_H
