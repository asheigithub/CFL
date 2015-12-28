#ifndef CFL_GLVBO_H
#define CFL_GLVBO_H

#include "CFLGLObj.h"
#include "IGLDataResouce.h"

namespace cfl
{
	namespace render
	{
		namespace vbotarget
		{
			//缓冲区目标
			enum VboTarget
			{
				gl_array_buffer = GL_ARRAY_BUFFER,
				gl_element_array_buffer= GL_ELEMENT_ARRAY_BUFFER
			};
		}
		namespace vbousage
		{
			enum VboUsage
			{
				//缓冲区对象修改一次，使用多次
				gl_static_draw=GL_STATIC_DRAW,
				//缓冲区对象重复修改，多次使用
				gl_dynamic_draw = GL_DYNAMIC_DRAW,
				//缓冲区对象修改一次，使用少数几次
				gl_stream_draw =GL_STREAM_DRAW


			};
		}
		class GLVBO;
		void doBuffSubData(
			GLVBO* vbo,
			GLsizeiptr buff_offset,
			GLsizeiptr size,
			std::shared_ptr<content::IGLDataResource> datasource,
			size_t offset,
			bool autoclosedatasource = false);

		/*
		顶点缓冲区对象VBO
		*/
		class GLVBO :public IGLObj
		{
			

			/*
			立刻更新VBO..只能在绘图线程调用
			*/
			friend void doBuffSubData(
				GLVBO* vbo,
				GLsizeiptr buff_offset,
				GLsizeiptr size,
				std::shared_ptr<content::IGLDataResource> datasource,
				size_t offset,
				bool autoclosedatasource );

			friend class GLObjCollection;
		public:
			inline bool isValid() override final{ return _isvalid; };

			void close() override final;


			
			/*创建VBO
			target-- VboTarget
			usage ---VboUsage
			size ---缓冲区数据存储大小，以字节数表示
			datasource   ---数据源
			offset ---数据源指针偏移量
			genNOw ---立即创建.只在绘图线程调用才有效
			autoclosedatasource --是否提交数据后自动删除客户端数据。默认为否
			*/
			void genBuffer(vbotarget::VboTarget target,
				vbousage::VboUsage usage,
				GLsizeiptr size,
				std::shared_ptr<content::IGLDataResource> datasource,
				size_t offset,
				bool genNow = false,
				bool autoclosedatasource=false);

			/*
			发送更新VBO的数据的指令
			buff_offset --缓冲区数据中的偏移
			size        --被修改的字节数
			datasource          --数据源
			offset      --数据源指针偏移量
			autoclosedatasource --是否提交数据后自动删除客户端数据。默认为否
			*/
			void buffSubData(GLsizeiptr buff_offset,
				GLsizeiptr size,
				std::shared_ptr<content::IGLDataResource> datasource,
				size_t offset,
				bool autoclosedatasource=false
				);
			

			vbotarget::VboTarget getTarget() const { return _target; }
			vbousage::VboUsage getUsage() const { return _usage; }
			GLsizeiptr getSize() const { return _size; }

		private:
			GLVBO(unsigned int id, std::list<IGLObj*>::iterator iter);
			~GLVBO();

			void onLostEGLContext(CFLContext* context) override final;

			void onGetNewEGLContext(CFLContext* context) override final;

			void doGenBuffer();
			

			vbotarget::VboTarget _target;
			vbousage::VboUsage _usage;
			GLsizeiptr _size;
			std::shared_ptr<content::IGLDataResource> _datasource;
			size_t _offset;
			bool _autoclosedatasource;

			bool _isvalid;

		};

		
	}
}


#endif // !CFL_GLVBO_H
