#ifndef _CFLASYNCFUNC
#define _CFLASYNCFUNC


#include <memory>



#include <mutex>
#include <condition_variable>



#include "CFLEntry.h"

namespace cfl
{
	
	namespace asyncFunc
	{
		/*
		当在线程池中调用endInvoke并阻塞，等待当前任务完成时会抛出。
		*/
		class InvalidEndInvokeOpException :public std::runtime_error
		{
		public:
			InvalidEndInvokeOpException():runtime_error("在线程池中调用endInvoke阻塞当前线程。由于调用线程和执行线程是同一个线程，所以极可能会导致死锁。") {}

		};



		template<typename T, typename C>
		struct AsyncHandlerBase
		{
			typedef void* anyptr;
			AsyncHandlerBase():tag( new anyptr(nullptr) ){}

			using CallBackerF = void(*)(C&);

			bool hasCamplate()
			{
				std::unique_lock<std::mutex> lock(mtx);

				return *_hascomplate;
			}
			//设置当任务完成时的回调。（回调是在任务线程上发生的）
			void setCallbackFun(CallBackerF callbacker)
			{

				std::unique_lock<std::mutex> lock(mtx);

				if (*_hascomplate)
				{
					lock.unlock();
					//已执行完毕直接返回
					callbacker(*reinterpret_cast<C*>(this));
				}
				else
				{
					memcpy(callbackfunPointerBuff, (void *)&callbacker, sizeof(CallBackerF*));
					//auto pp = reinterpret_cast<CallBackerF*>(callbackfunPointerBuff);
				}


			}

			//保存任何其他对象
			std::shared_ptr<anyptr> tag;

		protected:

			void doCallbacker()
			{
				std::unique_lock<std::mutex> lock(mtx);

				CallBackerF ff = nullptr;
				auto pp = reinterpret_cast<CallBackerF*>(callbackfunPointerBuff);
				if (pp != nullptr)
				{
					auto fun = *pp;
					if (fun != nullptr)
					{
						ff = fun;
					}

				}

				static std::allocator<CallBackerF> cbfalloc;
				cbfalloc.deallocate( reinterpret_cast<CallBackerF*>( callbackfunPointerBuff), 1);
				//free(callbackfunPointerBuff);

				lock.unlock();

				if (ff != nullptr)
				{
					ff(*reinterpret_cast<C*>(this));
				}
			}

			void* callbackfunPointerBuff;

			static std::mutex mtx;
			static std::condition_variable notifycomplate;

			std::shared_ptr<bool> _hascomplate;

			virtual void createFunpointerBuff()
			{
				_hascomplate = std::make_shared<bool>(false);

				static std::allocator<CallBackerF> cbfalloc;
				
				auto p = cbfalloc.allocate(1);

				callbackfunPointerBuff = reinterpret_cast<void*>(p);
				cbfalloc.construct(p, nullptr);
				//callbackfunPointerBuff = malloc(sizeof(CallBackerF*));
				//memset(callbackfunPointerBuff, 0, sizeof(CallBackerF*));
			}
		};
		template<typename T, typename C>
		std::mutex AsyncHandlerBase<T, C>::mtx;
		template<typename T, typename C>
		std::condition_variable AsyncHandlerBase<T, C>::notifycomplate;




		template<typename RETURNTYPE>
		struct AsyncHandler : public AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>
		{
			template<typename Fn, typename R, typename ...A>
			friend struct RunTask;

			template<typename Fn, typename ...A1>
			friend auto beginInvoke( const Fn _f, const A1&... a1)->AsyncHandler < decltype(_f(a1...)) >;

			//阻塞当前线程等待任务结束，并返回任务结果
			RETURNTYPE& endInvoke() const
			{
				while (true)
				{
					std::unique_lock<std::mutex> lock(AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::mtx);
					if (*AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::_hascomplate)
					{
						break;
					}

					if (CFLContext::getCurrentInstance()->taskPool->checkIsPoolThread())
					{
						throw InvalidEndInvokeOpException();
					}

					AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::notifycomplate.wait(lock);
					lock.unlock();
				}
				return *(*_value);
			}



		private:

			//***获取返回对象地址的分配器***
			static std::allocator<RETURNTYPE>& getRetAlloc()
			{
				static std::allocator<RETURNTYPE> alloc;
				return alloc;
			}

			typedef decltype(getRetAlloc().allocate(1)) RETPointer;

			std::shared_ptr<RETPointer> _value;

			std::allocator<RETPointer>& getRetPointerAlloc()
			{
				static  std::allocator<RETPointer> retpointeralloc;
				return retpointeralloc;
			}

			void  createFunpointerBuff() override final
			{
				
				auto ptralloc = getRetPointerAlloc();
				auto ptr = ptralloc.allocate(1);
				ptralloc.construct(ptr, nullptr);

				_value =
					std::shared_ptr<RETPointer>( ptr, //new RETPointer(),
					[this](RETPointer* add)
				{
					auto dealloc = getRetAlloc();
					dealloc.destroy(*add);
					//delete *add;
					dealloc.deallocate(*add, 1);
					//delete add;
					
					this->getRetPointerAlloc().deallocate(add, 1);

					;
				});

				//_value = temp;

				AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::createFunpointerBuff();

				/*_hascomplate = std::make_shared<bool>(false);
				callbackfunPointerBuff = malloc(sizeof(CallBackerF*));
				memset(callbackfunPointerBuff, 0, sizeof(CallBackerF*));*/
			}



		};


		template<>
		struct AsyncHandler<void> :public AsyncHandlerBase<void, AsyncHandler<void>>
		{
			template<typename Fn, typename R, typename ...A>
			friend struct RunTask;

			template<typename Fn, typename ...A>
			friend auto beginInvoke(  const Fn _f, const A& ...a1)->AsyncHandler < decltype(_f(a1...)) >;
			
			//阻塞当前线程等待任务结束，并返回任务结果。
			void endInvoke() const
			{
				while (true)
				{
					std::unique_lock<std::mutex> lock(mtx);
					if (*_hascomplate)
					{
						break;
					}

					if (CFLContext::getCurrentInstance()->taskPool->checkIsPoolThread())
					{
						throw  InvalidEndInvokeOpException();
					}

					notifycomplate.wait(lock);
					lock.unlock();
				}
				return;
			}


		};

#define _PARALIST(...)  __VA_ARGS__  
#define _STDGET(n) std::get<n+2>(*pack)



#define _PACKARGS(RET ,PARAMETER, ARGUMENT ) \
	static void* packArgs(const F& fn, const AsyncHandler<RET>& handler PARAMETER  )			\
		{																						\
	typedef decltype(std::make_tuple(fn, handler ARGUMENT)) ArgsTuple;							\
																						\
	static std::allocator<ArgsTuple> alloc;												\
																						\
	auto pt = alloc.allocate(1);														\
	alloc.construct(pt, fn, handler ARGUMENT);											\
																						\
	return reinterpret_cast<void*>(pt);													\
		}																						\

#define _GETRUN(PARAMETER, ARGUMENT, RUNARGS)																			\
static auto getRun(const F& fn, const AsyncHandler<RETURNTYPE>& handler PARAMETER)				\
-> void(*)(void*)																		\
		{																						\
	typedef decltype(std::make_tuple(fn, handler ARGUMENT)) ArgsTuple;							\
																						\
	auto runfunc = [](void * args)														\
			{																					\
		ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);							\
																						\
		F fn = std::get<0>(*pack);														\
		AsyncHandler<RETURNTYPE> asynchandler = std::get<1>(*pack);						\
																						\
		auto ret = fn(RUNARGS);																\
																						\
																						\
		static std::allocator<ArgsTuple> alloc;											\
		alloc.destroy(pack);															\
		alloc.deallocate(pack, 1);														\
																						\
		std::unique_lock<std::mutex> lock(AsyncHandler<RETURNTYPE>::mtx);				\
				{																		\
														\
																						\
																						\
			auto alloc = asynchandler.getRetAlloc();									\
			auto retaddress = alloc.allocate(1);										\
			alloc.construct(retaddress, ret);											\
			*asynchandler._value = retaddress;											\
			*asynchandler._hascomplate = true;											\
																						\
																						\
			AsyncHandler<RETURNTYPE>::notifycomplate.notify_all();						\
																						\
				}																				\
		lock.unlock();																	\
																						\
		asynchandler.doCallbacker();													\
			};																					\
																						\
	return runfunc;																		\
		}																						\


		template<typename F, typename RETURNTYPE, typename ...A>
		struct RunTask
		{
			_PACKARGS(RETURNTYPE, _PARALIST(), _PARALIST())

				_GETRUN(_PARALIST(), _PARALIST(), _PARALIST())


				template<typename T1>
			_PACKARGS(RETURNTYPE, _PARALIST(, const T1& a1), _PARALIST(, a1))

				template<typename T1>
			_GETRUN(_PARALIST(, const T1& a1), _PARALIST(, a1), _PARALIST(_STDGET(0)))


				template<typename T1, typename T2>
			_PACKARGS(RETURNTYPE,
				_PARALIST(, const T1& a1, const T2& a2),
				_PARALIST(, a1, a2))

				template<typename T1, typename T2>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2),
				_PARALIST(, a1, a2),
				_PARALIST(_STDGET(0), _STDGET(1)))


				template<typename T1, typename T2, typename T3>
			_PACKARGS(RETURNTYPE,
				_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
				_PARALIST(, a1, a2, a3))

				template<typename T1, typename T2, typename T3>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
				_PARALIST(, a1, a2, a3),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2)))

				template<typename T1, typename T2, typename T3, typename T4>
			_PACKARGS(RETURNTYPE,
				_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
				_PARALIST(, a1, a2, a3, a4))

				template<typename T1, typename T2, typename T3, typename T4>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
				_PARALIST(, a1, a2, a3, a4),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3)))


				template<typename T1, typename T2, typename T3, typename T4, typename T5>
			_PACKARGS(RETURNTYPE,
				_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
				_PARALIST(, a1, a2, a3, a4, a5))

				template<typename T1, typename T2, typename T3, typename T4, typename T5>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
				_PARALIST(, a1, a2, a3, a4, a5),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3), _STDGET(4)))



				/*static auto getRun(const F& fn, const AsyncHandler<RETURNTYPE>& handler)
				-> void(*)(void*)
				{
				typedef decltype(std::make_tuple(fn, handler)) ArgsTuple;

				auto runfunc = [](void * args)
				{
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn = std::get<0>(*pack);
				AsyncHandler<RETURNTYPE> asynchandler = std::get<1>(*pack);

				auto ret = fn();


				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<RETURNTYPE>::mtx);
				{
				*asynchandler._hascomplate = true;


				auto alloc = asynchandler.getRetAlloc();
				auto retaddress = alloc.allocate(1);
				alloc.construct(retaddress, ret);
				*asynchandler._value = retaddress;



				AsyncHandler<RETURNTYPE>::notifycomplate.notify_all();

				}
				lock.unlock();

				asynchandler.doCallbacker();
				};

				return runfunc;
				}*/



				/*template<typename T1>
				static void* packArgs(const F& fn,const AsyncHandler<RETURNTYPE>& handler,const T1& a1 )
				{
				typedef decltype(std::make_tuple(fn,handler, a1)) ArgsTuple;

				static std::allocator<ArgsTuple> alloc;

				auto pt = alloc.allocate(1);
				alloc.construct( pt, fn, handler,a1);

				return reinterpret_cast<void*>(pt);
				}*/

				/*template<typename T1>
				static auto getRun(const F& fn, const AsyncHandler<RETURNTYPE>& handler, const T1& a1)
				-> void(*)(void*)
				{
				typedef decltype(std::make_tuple(fn, handler, a1)) ArgsTuple;

				auto runfunc= [](void * args)
				{
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn = std::get<0>(*pack);
				AsyncHandler<RETURNTYPE> asynchandler = std::get<1>(*pack);

				auto ret = fn(std::get<2>(*pack));


				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<RETURNTYPE>::mtx);
				{
				*asynchandler._hascomplate = true;


				auto alloc = asynchandler.getRetAlloc();
				auto retaddress = alloc.allocate(1);
				alloc.construct(retaddress, ret);
				*asynchandler._value = retaddress;



				AsyncHandler<RETURNTYPE>::notifycomplate.notify_all();

				}
				lock.unlock();

				asynchandler.doCallbacker();
				};

				return runfunc;
				}*/



				/*static void run(void* args)
				{
				F* fp = nullptr; A* a1; AsyncHandler<RETURNTYPE>* hp = nullptr;
				typedef decltype(std::make_tuple(*fp, *hp, *a1)) ArgsTuple;
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn= std::get<0>(*pack);
				AsyncHandler<RETURNTYPE> asynchandler = std::get<1>(*pack);

				auto ret = fn(std::get<2>(*pack));


				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<RETURNTYPE>::mtx);
				{
				*asynchandler._hascomplate = true;


				auto alloc = asynchandler.getRetAlloc();
				auto retaddress=alloc.allocate(1);
				alloc.construct(retaddress,ret);
				*asynchandler._value = retaddress;



				AsyncHandler<RETURNTYPE>::notifycomplate.notify_all();

				}
				lock.unlock();

				asynchandler.doCallbacker();
				}*/
		};


#undef _GETRUN
#define _GETRUN(PARAMETER, ARGUMENT, RUNARGS) \
static auto getRun(const F& fn, const AsyncHandler<void>& handler PARAMETER)		\
-> void(*)(void*)														\
				{																		\
	typedef decltype(std::make_tuple(fn, handler ARGUMENT)) ArgsTuple;			\
																		\
	auto runfunc = [](void * args)										\
					{																	\
		ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);			\
																		\
		F fn = std::get<0>(*pack);										\
		AsyncHandler<void> asynchandler = std::get<1>(*pack);			\
		fn(RUNARGS);															\
																		\
		static std::allocator<ArgsTuple> alloc;							\
		alloc.destroy(pack);											\
		alloc.deallocate(pack, 1);										\
																		\
		std::unique_lock<std::mutex> lock(AsyncHandler<void>::mtx);		\
						{															\
			*asynchandler._hascomplate = true;							\
			AsyncHandler<void>::notifycomplate.notify_all();			\
						}															\
		lock.unlock();													\
																		\
		asynchandler.doCallbacker();									\
					};																\
																		\
	return runfunc;														\
			}																		\


		//特化返回类型为void的模板
		template<typename F, typename ... A>
		struct RunTask<F, void, A...>
		{
			_PACKARGS(void, _PARALIST(), _PARALIST())

				_GETRUN(_PARALIST(), _PARALIST(), _PARALIST())

				template<typename T1>
			_PACKARGS(void, _PARALIST(, const T1& a1), _PARALIST(, a1))

				template<typename T1>
			_GETRUN(_PARALIST(, const T1& a1), _PARALIST(, a1), _PARALIST(_STDGET(0)))

				template<typename T1, typename T2>
			_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2),
				_PARALIST(, a1, a2))

				template<typename T1, typename T2>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2),
				_PARALIST(, a1, a2),
				_PARALIST(_STDGET(0), _STDGET(1)))

				template<typename T1, typename T2, typename T3>
			_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2, const T3& a3),
				_PARALIST(, a1, a2, a3))

				template<typename T1, typename T2, typename T3>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
				_PARALIST(, a1, a2, a3),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2)))

				template<typename T1, typename T2, typename T3, typename T4>
			_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
				_PARALIST(, a1, a2, a3, a4))

				template<typename T1, typename T2, typename T3, typename T4>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
				_PARALIST(, a1, a2, a3, a4),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3)))


				template<typename T1, typename T2, typename T3, typename T4, typename T5>
			_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
				_PARALIST(, a1, a2, a3, a4, a5))

				template<typename T1, typename T2, typename T3, typename T4, typename T5>
			_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
				_PARALIST(, a1, a2, a3, a4, a5),
				_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3), _STDGET(4)))


				/*static void* packArgs(const F& fn, const AsyncHandler<void>& handler, const T1&  a1)
				{

				typedef decltype(std::make_tuple(fn,handler, a1)) ArgsTuple;

				static std::allocator<ArgsTuple> alloc;

				auto pt = alloc.allocate(1);
				alloc.construct(pt, fn,handler, a1);

				return reinterpret_cast<void*>(pt);
				}*/


				/*static auto getRun(const F& fn, const AsyncHandler<void>& handler)
				-> void(*)(void*)
				{
				typedef decltype(std::make_tuple(fn, handler)) ArgsTuple;

				auto runfunc = [](void * args)
				{
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn = std::get<0>(*pack);
				AsyncHandler<void> asynchandler = std::get<1>(*pack);
				fn();

				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<void>::mtx);
				{
				*asynchandler._hascomplate = true;
				AsyncHandler<void>::notifycomplate.notify_all();
				}
				lock.unlock();

				asynchandler.doCallbacker();
				};

				return runfunc;
				}*/


				/*template<typename T1>
				static auto getRun(const F& fn, const AsyncHandler<void>& handler, const T1& a1)
				-> void(*)(void*)
				{
				typedef decltype(std::make_tuple(fn, handler, a1)) ArgsTuple;

				auto runfunc = [](void * args)
				{
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn = std::get<0>(*pack);
				AsyncHandler<void> asynchandler = std::get<1>(*pack);
				fn(std::get<2>(*pack));

				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<void>::mtx);
				{
				*asynchandler._hascomplate = true;
				AsyncHandler<void>::notifycomplate.notify_all();
				}
				lock.unlock();

				asynchandler.doCallbacker();
				};

				return runfunc;
				}*/

				/*static void run(void* args)
				{
				F* fp = nullptr; A* a1 = nullptr; AsyncHandler<void>* hp = nullptr;
				typedef decltype(std::make_tuple(*fp, *hp, *a1)) ArgsTuple;
				ArgsTuple* pack = reinterpret_cast<ArgsTuple*>(args);

				F fn = std::get<0>(*pack);
				AsyncHandler<void> asynchandler = std::get<1>(*pack);
				fn(std::get<2>(*pack));

				static std::allocator<ArgsTuple> alloc;
				alloc.destroy(pack);
				alloc.deallocate(pack, 1);

				std::unique_lock<std::mutex> lock(AsyncHandler<void>::mtx);
				{
				*asynchandler._hascomplate = true;
				AsyncHandler<void>::notifycomplate.notify_all();
				}
				lock.unlock();

				asynchandler.doCallbacker();
				}*/

		};

		
		template<typename Fn, typename ...A>
		auto beginInvoke(const Fn _f, const A&... a1) ->AsyncHandler<decltype(_f(a1...))> //发起一次异步调用
		{
			typedef decltype(_f(a1...)) ReturnType;

			auto asynchandler = AsyncHandler<ReturnType>();
			asynchandler.createFunpointerBuff();


			auto argsbuff = RunTask<Fn, ReturnType, A...>::packArgs(_f, asynchandler, a1...);

			CFLContext::getCurrentInstance()->taskPool->pushtask(RunTask<Fn, ReturnType, A...>::getRun(_f, asynchandler, a1...),
				(void*)argsbuff,
				nullptr);

			return asynchandler;
		}



	}


}



#endif // !_CFLASYNCFUNC
