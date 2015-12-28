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
		�����̳߳�
		*/
		class TaskPool;

		using taskfun = void(*)(void*);
		using callbackfun = taskfun;

		static void exec(TaskPool* tasks);
		/*
		�����߳�Ԫ��
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

			//��⵱ǰ�߳��Ƿ����̳߳��е��߳�
			bool checkIsPoolThread();

		private:
			TaskPool();
			std::deque<TaskElement> tasklist; //�����������
			_recursive_mutex* mtx_tasklist;            //���������������ʱ�Ļ���Ԫ

			//std::mutex mtx_worker;              //�����̵߳ȴ�֪ͨ�û���Ԫ
			_condition_variable* taskincome; //֪ͨ��������
			
			std::deque<_thread*> workers; //�����̶߳���

			bool flagexitthread;
			bool _isdisposed;
		};
	}
}






#endif // !_CFLTHREADTASKS_H
