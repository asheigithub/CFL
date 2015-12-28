#include "clearwindows.h"
#include "CFLThreadTasks.h"
#include "CFLEntry.h"


#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace cfl
{
	namespace tasks
	{
		struct _recursive_mutex : std::recursive_mutex{};
		struct _condition_variable : std::condition_variable{};

		struct _thread : std::thread
		{
			_thread(void(*exec)(TaskPool*), TaskPool* args)
				:std::thread(exec,args)
			{}
		};


		TaskPool::TaskPool() :_isdisposed(false), 
			flagexitthread(false), 
			tasklist(std::deque<TaskElement>())
			,
			mtx_tasklist(new _recursive_mutex()),
			taskincome(new _condition_variable{})
		{
			
			auto count = std::thread::hardware_concurrency() * 2;
			count = count < 1 ? 1 : count;
			
			for (size_t i = 0; i < count; i++)
			{
				workers.emplace_back(new _thread(exec, this));
			}
		}
		TaskPool::~TaskPool()
		{
			dispose();
		}
		
		bool TaskPool::checkIsPoolThread()
		{
			auto tid = std::this_thread::get_id();
			for (auto i = workers.begin(); i != workers.end(); i++)
			{
				if (tid == (*i)->get_id())
				{
					return true;
				}
			}
			
			return false;
		}

		void TaskPool::dispose()
		{
			LOGI("dispose TaskPool\n");

			if (!_isdisposed)
			{
				_isdisposed = true;

				flagexitthread = true;
				taskincome->notify_all();

				for (auto i = workers.begin(); i != workers.end(); i++)
				{
					(*i)->join();
					delete *i;
				}

				if (mtx_tasklist)
				{
					delete mtx_tasklist;
					mtx_tasklist = nullptr;
				}
				if (taskincome)
				{
					delete taskincome;
					taskincome = nullptr;
				}

				LOGI("exit taskPool\n");

			}
		}

		void TaskPool::pushtask(taskfun fun, void* args, callbackfun callbacker)
		{
			
			std::unique_lock<std::recursive_mutex> lock(*mtx_tasklist);

			TaskElement element;
			element.funpointer = fun;
			element.callbackerpointer = callbacker;
			element.args = args;

			tasklist.emplace_back(element);

			lock.unlock();

			taskincome->notify_one();
		}


		static void exec(TaskPool* tasks)
		{
			LOGI("taskPool workThread tid:%d start\n",std::this_thread::get_id());

			while (!tasks->flagexitthread)
			{
				
				TaskElement* elem = nullptr;
				TaskElement temp;
				std::unique_lock<std::recursive_mutex> lock(*(tasks->mtx_tasklist), std::defer_lock);

				if (lock.try_lock())
				{ 
					if (tasks->tasklist.begin() != tasks->tasklist.end())
					{
						elem = &(*(tasks->tasklist.begin()));
						temp = TaskElement(*elem);
						tasks->tasklist.erase(tasks->tasklist.begin());
					}

					
					lock.unlock();
				}
				if (elem)
				{
					//cfl::trace("do a task\n");
					//LOGI("tasklistsize: %d\n",tasks->tasklist.size());
					
					temp.funpointer(temp.args);
					if (temp.callbackerpointer)
					{
						temp.callbackerpointer(temp.args);
					}
					

					//cfl::trace("end a task\n");
					//std::this_thread::yield();
				}
				else
				{
					std::mutex mtx;
					std::unique_lock<std::mutex> lockwait(mtx);
					auto waitout= tasks->taskincome->wait_for(lockwait,std::chrono::milliseconds(200));
					/*if (waitout == std::cv_status::timeout)
					{
						cfl::trace("wait timeout\n");
					}*/
					//tasks->taskincome.wait(lockwait);
					lockwait.unlock();
				}
				

				

			}

			LOGI("taskPool workThread tid:%d exit\n", std::this_thread::get_id());
		}


	}

}
