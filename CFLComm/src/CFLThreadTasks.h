#ifndef _CFLTHREADTASKS_H
#define _CFLTHREADTASKS_H

#include <deque>


namespace cfl
{
	struct CFLContext;
	namespace tasks
	{
		struct _recursive_mutex;
		struct _condition_variable;
		struct _thread;
		
		/*
		工作线程池
		*/
		class TaskPool;

		using taskfun = void(*)(void*);
		using callbackfun = taskfun;

		static void exec(TaskPool* tasks);
		/*
		工作线程元素
		*/
		struct TaskElement
		{
			friend TaskPool;
			friend  void exec(TaskPool* tasks);
			taskfun funpointer;
			callbackfun callbackerpointer;
			void* args;

			~TaskElement()
			{
				//cfl::trace("deleted %p argpointer %p\n",funpointer,args);
			}

		private:
			TaskElement(){};
			
			//std::thread* bindthread;
		};

		
		class TaskPool
		{
			friend  void exec(TaskPool* tasks);
			friend CFLContext;
		public:
			void dispose();
			
			~TaskPool();

			TaskPool(TaskPool& obj) = delete;
			TaskPool& operator=(TaskPool& lhs) = delete;

			void pushtask(taskfun fun, void* args, callbackfun callbacker);

			//检测当前线程是否是线程池中的线程
			bool checkIsPoolThread();

		private:
			TaskPool();
			std::deque<TaskElement> tasklist; //工作任务队列
			_recursive_mutex* mtx_tasklist;            //操作工作任务队列时的互斥元

			//std::mutex mtx_worker;              //工作线程等待通知用互斥元
			_condition_variable* taskincome; //通知来任务了
			
			std::deque<_thread*> workers; //工作线程队列

			bool flagexitthread;
			bool _isdisposed;
		};
	}
}






#endif // !_CFLTHREADTASKS_H
