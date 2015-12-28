#ifndef CFL_IGLDATARESOURCE_H

#define CFL_IGLDATARESOURCE_H

#include <memory>

namespace cfl
{
	namespace content
	{
		class IGLDataResource;

		typedef void(*GLDataResourceLoadCallbacker)(std::shared_ptr<IGLDataResource>, void* args);

		/*
		定义GLES的二进制资源接口
		例如原始顶点数据，原生纹理图片数据等等
		*/
		class IGLDataResource 
		{
		public:
			virtual ~IGLDataResource() = default;

			//读取数据
			virtual void* getData() = 0;
			//数据字节长度
			virtual size_t getByteLength() = 0;

			//是否已完成操作
			virtual bool isDone() = 0;
			//数据是否正确
			virtual bool isValid() = 0;
			//查询错误信息
			virtual const char* getError()=0;
			//同步加载资源
			virtual void load() = 0;
			//异步加载资源
			virtual void loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args) = 0;
			//获取加载进度
			virtual float getProgress() = 0;
			//通知可以释放资源。。不过释放之后，则下次使用需要再次读取。
			virtual void close() = 0;
		
			

		};


	}
}




#endif // !CFL_IGLDATARESOURCE_H
