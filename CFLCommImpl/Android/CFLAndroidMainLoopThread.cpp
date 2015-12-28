#include <clearwindows.h>
#include "CFLAndroidMainLoopThread.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace cfl
{
	std::condition_variable flag_threadquit;


	static std::condition_variable flag_drawThread_complet;


	static std::mutex mtxforupdate;
	static std::mutex mtxfordraw; //执行update时锁定，draw线程要等待它解锁

	static bool runningflag;       //标记整体主循环正在执行

	static size_t drawCount;         //绘图计数器
	static size_t updateCount;       //update计数器

	static std::atomic<bool> atomic_updaterun; //表示收到信号量开始执行一次更新。

	void updateWorker(CFLContext* esContext);


	static float dt;
	static std::thread updateThread;
	void notify_exit()
	{
		runningflag = false;
		flag_drawThread_complet.notify_all();

		atomic_updaterun = true;

		updateThread.join(); //等待子线程退出
	}



	void setupMainLoopthread(CFLContext* esContext)
	{
		drawCount = 0;
		updateCount = 0;

		atomic_updaterun = false;

		runningflag = true;
		updateThread = std::thread(updateWorker, esContext);

		LOGI("update thread started.\n");

	}

	void updateWorker(CFLContext* esContext)
	{
		
		while (runningflag)
		{
			std::unique_lock<std::mutex> lock(mtxforupdate);
			flag_drawThread_complet.wait(lock); //等待绘图线程通知更新
			lock.unlock();


			if (updateCount == drawCount)
			{
				std::this_thread::yield();
				continue;
			}

			atomic_updaterun = true;

			std::unique_lock<std::mutex> lockonupdate(mtxfordraw);		//立即锁定mtxfordraw.绘图线程通过这个互斥元确定是否更新完成

			
			if (esContext && esContext->onUpdate)
			{
				esContext->onUpdate(esContext, dt);
			}
			updateCount++;
			

			lockonupdate.unlock();
		}

		

		LOGI("update thread quit\n");

		//flag_threadquit.notify_all();

	}

	void mainthread_mainloop(CFLContext* esContext, float deltaTime)
	{
		
		dt = deltaTime;

		drawCount++; //绘图计数器+1
		
		esContext->totalframes = drawCount;

		
		while (true)
		{
			flag_drawThread_complet.notify_all(); //通知更新线程更新一次
			if (atomic_updaterun)
			{
				break;
			}
			std::this_thread::yield();
		}
		atomic_updaterun = false; //重置

		
		if (esContext)
		{
			esContext->renderLooper->render(esContext);
		}
		//int yieldcount = 0;
		//等待更新完毕
		while (true)
		{
			std::unique_lock<std::mutex> lockwaitupdate(mtxfordraw);
			if (drawCount == updateCount)
			{
				break;
			}
			lockwaitupdate.unlock();

			//yieldcount++;
			//CFL_ASSERT(yieldcount < 100);
			std::this_thread::yield();
		}
		esContext->renderLooper->swapQueue(esContext);
		//auto ed = getTimer();
		//trace("frame use: %d\n", ed - st);

	}
}
