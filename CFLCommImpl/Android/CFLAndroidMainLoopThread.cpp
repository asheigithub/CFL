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
	static std::mutex mtxfordraw; //ִ��updateʱ������draw�߳�Ҫ�ȴ�������

	static bool runningflag;       //���������ѭ������ִ��

	static size_t drawCount;         //��ͼ������
	static size_t updateCount;       //update������

	static std::atomic<bool> atomic_updaterun; //��ʾ�յ��ź�����ʼִ��һ�θ��¡�

	void updateWorker(CFLContext* esContext);


	static float dt;
	static std::thread updateThread;
	void notify_exit()
	{
		runningflag = false;
		flag_drawThread_complet.notify_all();

		atomic_updaterun = true;

		updateThread.join(); //�ȴ����߳��˳�
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
			flag_drawThread_complet.wait(lock); //�ȴ���ͼ�߳�֪ͨ����
			lock.unlock();


			if (updateCount == drawCount)
			{
				std::this_thread::yield();
				continue;
			}

			atomic_updaterun = true;

			std::unique_lock<std::mutex> lockonupdate(mtxfordraw);		//��������mtxfordraw.��ͼ�߳�ͨ���������Ԫȷ���Ƿ�������

			
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

		drawCount++; //��ͼ������+1
		
		esContext->totalframes = drawCount;

		
		while (true)
		{
			flag_drawThread_complet.notify_all(); //֪ͨ�����̸߳���һ��
			if (atomic_updaterun)
			{
				break;
			}
			std::this_thread::yield();
		}
		atomic_updaterun = false; //����

		
		if (esContext)
		{
			esContext->renderLooper->render(esContext);
		}
		//int yieldcount = 0;
		//�ȴ��������
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
