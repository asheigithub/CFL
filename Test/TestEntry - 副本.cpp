// TestEntry.cpp : 定义控制台应用程序的入口点。

#include "CFLEntry.h"
#include "CFLFile.h"

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace CFL::Tasks;

CFL::CFLContext* context;

float times = 0;
void up(float dettime)
{
	static int mmm = 0;

	times += dettime;
	//CFL::trace("%f\n", dettime);
	if (times >= 1)
	{
		CFL::trace("%d\n", (int)CFL::getFPS());
		times = 0;
		
		

		taskfun func = [](void * arg)->void
		{
			
			auto v = reinterpret_cast<std::tuple<CFL::File::fileInfoPtr, CFL::File::fileData,int>*>(arg);
			auto f = std::get<0>(*v);
			CFL::trace("taskid %d,%s\n", std::get<2>(*v) ,(f)->getFilePath());
			
			auto data = &std::get<1>(*v);
			(f)->readFile(data);
			(f).reset();
			
		};

		callbackfun callbacker = [](void * arg)->void
		{
			auto v = reinterpret_cast<std::tuple<CFL::File::fileInfoPtr, CFL::File::fileData,int>*>(arg);
			auto f = std::get<0>(*v);

			auto data = std::get<1>(*v);
			CFL::trace("taskid %d,filelen %d\n", std::get<2>(*v) , data.length);

			data.close();
			
			delete v;
		};

		for (size_t i = 0; i < 5; i++)
		{
			
			/*auto dir = CFL::File::getDirectory(CFL::File::dirtype::asset);
			
			std::tuple<decltype(dir->getfile("")), CFL::File::fileData,int>* para =
				new std::tuple<decltype(dir->getfile("")), CFL::File::fileData,int>(
					dir->getfile("earth.bmp"), CFL::File::fileData(),i);

			context->taskPool.pushtask(func, (void *)( para), callbacker);*/
		}



	}

	

};

//#define ACCESS _access  
//#define MKDIR(a) _mkdir((a))

void* testdo();

int cfl_main(CFL::CFLContext* ctx, int argc, char* argv[])
{
	context = ctx;

	//CFL::setFPS(30);
	CFL::drawfun kk = [](CFL::CFLContext* context )
	{
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	};
	
	CFL::registerDrawFunc(ctx, kk);

	CFL::registerUpdateFunc(ctx, up);

	testdo();

	return 0;
}




template<typename T, typename C>
struct AsyncHandlerBase
{
	using CallBackerF = void(*)(C&);

	bool hasCamplate()
	{
		std::unique_lock<std::mutex> lock(mtx);

		return *_hascomplate;
	}

	void setCallbackFun(CallBackerF callbacker)
	{

		std::unique_lock<std::mutex> lock(mtx);

		if (*_hascomplate)
		{
			lock.unlock();
			//已执行完毕直接返回
			callbacker( *reinterpret_cast<C*>(this));
		}
		else
		{
			memcpy(callbackfunPointerBuff, (void *)&callbacker, sizeof(CallBackerF*));
			auto pp = reinterpret_cast<CallBackerF*>(callbackfunPointerBuff);
		}


	}



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
		free(callbackfunPointerBuff);

		lock.unlock();

		if (ff != nullptr)
		{
			ff(* reinterpret_cast<C*>( this));
		}
	}

	void* callbackfunPointerBuff;

	static std::mutex mtx;
	static std::condition_variable notifycomplate;

	std::shared_ptr<bool> _hascomplate;

	virtual void createFunpointerBuff()
	{
		_hascomplate = std::make_shared<bool>(false);

		callbackfunPointerBuff = malloc(sizeof(CallBackerF*));
		memset(callbackfunPointerBuff, 0, sizeof(CallBackerF*));
	}
};
template<typename T, typename C>
std::mutex AsyncHandlerBase<T,C>::mtx;
template<typename T, typename C>
std::condition_variable AsyncHandlerBase<T,C>::notifycomplate;




template<typename RETURNTYPE>
struct AsyncHandler : public AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>
{
	template<typename Fn, typename R, typename ...A> 
	friend struct RunTask;

	template<typename Fn, typename ...A1>
	friend auto getTaskFun(const Fn _f, const A1&... a1)->AsyncHandler < decltype(_f(a1...)) > ;
	

	RETURNTYPE& endAsync() const
	{
		while (!(*AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::_hascomplate))
		{
			std::unique_lock<std::mutex> lock(AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::mtx);
			if (*AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::_hascomplate)
			{
				break;
			}
			AsyncHandlerBase<RETURNTYPE, AsyncHandler<RETURNTYPE>>::notifycomplate.wait(lock);
			lock.unlock();
		}
		return *(*_value);					
	}

	

private:

	//***获取返回对象地址的分配器***
	static std::allocator<RETURNTYPE> getRetAlloc()
	{
		static std::allocator<RETURNTYPE> alloc;
		return alloc;
	}

	typedef decltype(getRetAlloc().allocate(1)) RETPointer;

	std::shared_ptr<RETPointer> _value;
	
	

	void  createFunpointerBuff() override final
	{
		
		_value=
			std::shared_ptr<RETPointer> (new RETPointer(),
				[](RETPointer* add)
			{
				auto dealloc = getRetAlloc();
				dealloc.destroy(*add);
				//delete *add;
				dealloc.deallocate( *add, 1);
				delete add;
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
	template<typename Fn, typename R,typename ...A>
	friend struct RunTask;

	template<typename Fn, typename ...A>
	friend auto getTaskFun(const Fn _f, const A& ...a1)->AsyncHandler < decltype(_f(a1...)) >;
	
	void endAsync() const
	{
		while (!(*_hascomplate))
		{
			std::unique_lock<std::mutex> lock(mtx);
			if (*_hascomplate)
			{
				break;
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
		{																				\
			*asynchandler._hascomplate = true;											\
																						\
																						\
			auto alloc = asynchandler.getRetAlloc();									\
			auto retaddress = alloc.allocate(1);										\
			alloc.construct(retaddress, ret);											\
			*asynchandler._value = retaddress;											\
																						\
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


template<typename F,typename RETURNTYPE,typename ...A>
struct RunTask
{
	_PACKARGS(RETURNTYPE, _PARALIST(), _PARALIST())

	_GETRUN(_PARALIST(), _PARALIST(), _PARALIST())


	template<typename T1>
	_PACKARGS(RETURNTYPE, _PARALIST(, const T1& a1), _PARALIST(, a1))

	template<typename T1>
	_GETRUN(_PARALIST(, const T1& a1), _PARALIST(, a1), _PARALIST( _STDGET(0) ))


	template<typename T1,typename T2>
	_PACKARGS(RETURNTYPE, 
		_PARALIST(, const T1& a1, const T2& a2),
		_PARALIST(, a1,a2))

	template<typename T1,typename T2>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2), 
		_PARALIST(, a1, a2),
		_PARALIST(_STDGET(0), _STDGET(1)))


	template<typename T1, typename T2,typename T3>
	_PACKARGS(RETURNTYPE,
		_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
		_PARALIST(, a1, a2,a3))

	template<typename T1, typename T2,typename T3>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
		_PARALIST(, a1, a2,a3),
		_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2)))

		template<typename T1, typename T2, typename T3, typename T4>
	_PACKARGS(RETURNTYPE,
		_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
		_PARALIST(, a1, a2, a3,a4))

		template<typename T1, typename T2, typename T3, typename T4>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
		_PARALIST(, a1, a2, a3,a4),
		_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3)))


		template<typename T1, typename T2, typename T3, typename T4, typename T5>
	_PACKARGS(RETURNTYPE,
		_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
		_PARALIST(, a1, a2, a3, a4,a5))

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
		_PARALIST(, a1, a2, a3, a4,a5),
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
struct RunTask<F,void,A...>
{
	_PACKARGS(void, _PARALIST(), _PARALIST())

	_GETRUN(_PARALIST(), _PARALIST(), _PARALIST())

	template<typename T1>
	_PACKARGS(void, _PARALIST(, const T1& a1), _PARALIST(, a1))
	
	template<typename T1>
	_GETRUN(_PARALIST(, const T1& a1), _PARALIST(, a1), _PARALIST(_STDGET(0)))

	template<typename T1,typename T2>
	_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2), 
					_PARALIST(, a1,a2))

	template<typename T1, typename T2>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2), 
		_PARALIST(, a1, a2), 
		_PARALIST(_STDGET(0), _STDGET(1)))

	template<typename T1, typename T2,typename T3>
	_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2,  const T3& a3),
		_PARALIST(, a1, a2,a3) )

	template<typename T1, typename T2,typename T3>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3),
		_PARALIST(, a1, a2,a3),
		_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2)))

		template<typename T1, typename T2, typename T3, typename T4>
	_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
		_PARALIST(, a1, a2, a3,a4))

		template<typename T1, typename T2, typename T3, typename T4>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4),
		_PARALIST(, a1, a2, a3,a4),
		_PARALIST(_STDGET(0), _STDGET(1), _STDGET(2), _STDGET(3)))
	

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
	_PACKARGS(void, _PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
		_PARALIST(, a1, a2, a3, a4,a5))

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
	_GETRUN(_PARALIST(, const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5),
		_PARALIST(, a1, a2, a3, a4,a5),
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

template<typename Fn,typename ...A>
auto getTaskFun(const Fn _f, const A&... a1) ->AsyncHandler<decltype(_f(a1...))>
{
	typedef decltype(_f(a1...)) ReturnType;

	auto asynchandler= AsyncHandler<ReturnType>();
	asynchandler.createFunpointerBuff();
	

	auto argsbuff = RunTask<Fn, ReturnType, A...>::packArgs( _f,asynchandler,a1...);

	context->taskPool.pushtask(RunTask<Fn, ReturnType, A...>::getRun(_f, asynchandler, a1...),
		(void*)argsbuff, 
		nullptr);

	return asynchandler;
}




int tf(const char* c){ 
	CFL::trace("%s\n", c); 
	return 0; 
}
void tf(int c){ CFL::trace("%d\n", c); }


void* testdo()
{
	

	void(*fp)(int) = tf;
	int(*fp2)(const char*) = tf;

	
	auto handler= getTaskFun(fp2,"haha");
	
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	handler.setCallbackFun(
		[](decltype(handler)& h)
		{
			
			auto result=h.endAsync();

			CFL::trace("callback! result:%d\n",result);

			; 
		});

	auto vh= getTaskFun(fp,123);
	vh.endAsync();

	//getTaskFun(tf,true);
	//getTaskFun(tf,2.0f);

	
	const char * c = "bcd";
	auto h = getTaskFun([](int a,char b,bool c, char* d,CFL::drawfun ptr)
	{ 
		CFL::trace("twoargs a:%d b:%c\n",a,b);
		return "twoargs"; //std::get<0>(a)+std::get<1>(a); 
	}, 2,'c',false, const_cast<char *>( c),nullptr);

	//CFL::trace(" h: %s\n",h.endAsync());

	auto nargs = getTaskFun([]()
	{
		CFL::trace("noargs!!\n");
		//return "noargs"; //std::get<0>(a)+std::get<1>(a); 
	});

	auto filetest = [](const CFL::File::fileInfoPtr file)
	{
		//auto file=dir->getfile("earth.bmp");
		CFL::trace("file len:%d\n", file->getFileLength());
		return file;
	};

	auto dir = CFL::File::getDirectory(CFL::File::dirtype::asset);

	for (size_t i = 0; i < 5; i++)
	{
		auto handle=getTaskFun(filetest, dir->getfile("earth.bmp"));

		//auto f= handle.endAsync();
		handle.setCallbackFun(
			[](decltype(handle)& sender)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
			auto f= sender.endAsync();
			CFL::trace( " callbacker! %s tid:%d \n"  ,f->getFilePath(),std::this_thread::get_id() );

		}
			
			);
	}
	

	return 0;
}