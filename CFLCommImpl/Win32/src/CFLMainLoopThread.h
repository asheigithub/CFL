#ifndef _CFLMAINLOOPTHREAD_H
#define _CFLMAINLOOPTHREAD_H



#include "CFLEntry.h"

namespace cfl
{
	
	//��ʼ����Ϸѭ���߳�
	void setupMainLoopthread(CFLContext*);

	//ִ��һ����Ϸѭ��
	void mainthread_mainloop(CFLContext*, float);

	//֪ͨ�߳��˳�
	void notify_exit();


	

}



#endif // !_CFLMAINLOOPTHREAD_H




