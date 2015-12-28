#ifndef _CFLMAINLOOPTHREAD_H
#define _CFLMAINLOOPTHREAD_H



#include "CFLEntry.h"

namespace cfl
{
	
	//初始化游戏循环线程
	void setupMainLoopthread(CFLContext*);

	//执行一次游戏循环
	void mainthread_mainloop(CFLContext*, float);

	//通知线程退出
	void notify_exit();


	

}



#endif // !_CFLMAINLOOPTHREAD_H




