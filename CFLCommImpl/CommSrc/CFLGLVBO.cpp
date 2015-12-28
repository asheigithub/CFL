#include "Graphic/CFLGLVBO.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace render
	{
		using namespace content;

		GLVBO::GLVBO(unsigned int id, std::list<IGLObj*>::iterator iter) :IGLObj(id, iter),
			_isvalid(false),
			_target(vbotarget::gl_array_buffer),
			_usage(vbousage::gl_static_draw),
			_size(0),
			_datasource(nullptr),
			_offset(0),
			_autoclosedatasource(false)
		{
			
		}

		GLVBO::~GLVBO()
		{
			close();
		}

		void GLVBO::close()
		{
			if (_isvalid)
			{
				_isvalid = false;

				glDeleteBuffers(1, &glhandle);
				LOGI("VBO(%d) deleted!\n", objId);
			}
		}



		void GLVBO::doGenBuffer()
		{
			GLuint vboIds[1];

			{

				glGenBuffers(1, vboIds);
				CFL_CHECKGLERROR;

				if (_datasource != nullptr)
				{

					glBindBuffer(_target, vboIds[0]);
					CFL_CHECKGLERROR;

					glBufferData(_target, _size,
						reinterpret_cast<char*>(_datasource->getData()) + _offset, _usage);

					CFL_IFGLERROR(
						glDeleteBuffers(1, vboIds);
					return;
					)

					glBindBuffer(_target, 0);

					if (_autoclosedatasource)
					{
						_datasource->close();
					}
					
				}
			}

			

			_isvalid = true;
			glhandle = vboIds[0];

		}

		void GLVBO::genBuffer(vbotarget::VboTarget target,
			vbousage::VboUsage usage,
			GLsizeiptr size,
			std::shared_ptr<content::IGLDataResource> datasource,
			size_t offset,
			bool genNow,
			bool autoclosedatasource)
		{
			if (_isvalid)
			{
				LOGE("VBO已初始化\n");
				return;
			}

			_target = target;
			_usage = usage;
			_size = size;
			_datasource = datasource;
			_offset = offset;
			_autoclosedatasource = autoclosedatasource;

			if (datasource != nullptr)
			{
				if (!datasource->isDone())
				{
					datasource->load();
				}

				if (!datasource->isValid())
				{
					LOGE("genBuffer error, datasource error: %s\n", datasource->getError());
					return;
				}
			}

			if (genNow)
			{
				doGenBuffer();
			}
			else
			{
				auto fun = [this](){
					//LOGI("befor build tex %d\n", objId);
					doGenBuffer();
					//LOGI("end build tex %d\n", objId);
				};

				auto command = new funcDrawCommand< decltype(fun)>(fun, "doGenVBO");
				pushMainLooperCommand(command);
			}
			

		}


		void doBuffSubData(
			GLVBO* vbo, 
			GLsizeiptr buff_offset, 
			GLsizeiptr size,
			std::shared_ptr<content::IGLDataResource> datasource,
			size_t offset,
			bool autoclosedatasource)
		{
			CFL_ASSERT(vbo->isValid());

			
			glBindBuffer(vbo->_target, vbo->glhandle);

			CFL_CHECKGLERROR;

			if (buff_offset + size > vbo->_size)
			{
				CFL_ASSERT(buff_offset == 0);

				//重分配
				glBufferData(vbo->_target, size,
					reinterpret_cast<char*>(datasource->getData()) + offset, vbo->_usage);
				//更新源
				vbo->_size = buff_offset + size;
				vbo->_datasource = datasource;   //更新数据源
				vbo->_offset = offset;
			}
			else
			{
				glBufferSubData(vbo->_target, buff_offset, size, 
					reinterpret_cast<char*>(datasource->getData()) + offset);
			}
			glBindBuffer(vbo->_target,0);
			CFL_CHECKGLERROR;

		}

		void GLVBO::buffSubData(GLsizeiptr buff_offset,
			GLsizeiptr size,
			std::shared_ptr<content::IGLDataResource> datasource,
			size_t offset,
			bool autoclosedatasource
			)
		{
			CFL_ASSERT(datasource != nullptr);

			if (!datasource->isDone())
			{
				datasource->load();
			}

			if (!datasource->isValid())
			{
				LOGE("buffSubData error, datasource error: %s\n", datasource->getError());
				return;
			}

			if (buff_offset + size > _size && buff_offset > 0)
			{
				LOGE("修改的数据量超过了原有的大小，而且无法重新分配。");
				return;
			}


			auto fun = [this,datasource,buff_offset,size,offset,autoclosedatasource]()
			{
				doBuffSubData(this,buff_offset, size, datasource, offset, autoclosedatasource);
			};
			auto command = new funcDrawCommand< decltype(fun)>(fun, "buffSubData");
			pushMainLooperCommand(command);
		}


		void GLVBO::onLostEGLContext(CFLContext* context)
		{
			close();
		}
		void GLVBO::onGetNewEGLContext(CFLContext* context)
		{
			if (!_isvalid)
			{
				//LOGI("befor glVBO(%d) repair\n", objId);

				doGenBuffer();
				CFL_ASSERT(_isvalid); //要求修复成功
				LOGI("glVBO(%d) repaired!\n", objId);
			}



		}

	}
}
