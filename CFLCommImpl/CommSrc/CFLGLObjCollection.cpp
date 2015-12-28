#include "Graphic/CFLGLObj.h"
#include "CFLEntry.h"
#include "Graphic/CFLGLObjCollection.h"

#include <list>
#include <atomic>
#include <mutex>

namespace cfl
{

	namespace render
	{
		static std::mutex* mtx_objlist;
		static std::list<IGLObj*>* objlist;//全局GLObj列表
		static std::atomic<unsigned int> idseed;

		GLObjCollection::GLObjCollection()
		{
			mtx_objlist = new std::mutex();
			objlist = new std::list<IGLObj*>();
		}
		GLObjCollection::~GLObjCollection()
		{
			delete mtx_objlist;
			delete objlist;
			
		}

		void GLObjCollection::pushDrawThreadCommand(IdrawCommand* cmd)
		{
			CFLContext::getCurrentInstance()->renderLooper->pushCommand(cmd);
		}

		void GLObjCollection::__notifyLostEGL(CFLContext* context)
		{
			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto end = objlist->end();
			for (auto begin = objlist->begin(); begin != end; begin++)
			{
				(*begin)->onLostEGLContext(context);
			}

		}
		void GLObjCollection::__notifyGetEGL(CFLContext* context)
		{
			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto end = objlist->end();
			for (auto begin = objlist->begin(); begin !=end; begin++)
			{
				(*begin)->onGetNewEGLContext(context);
			}
		}

		/***************
			Shader部分
		*/

		static std::allocator<GLShader> alloc_Shader;
		GLShader* GLObjCollection::createShader()
		{
			auto addr = alloc_Shader.allocate(1);

			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto pointer = objlist->insert(objlist->end(),addr);
			lock.unlock();

			return new(addr)GLShader(idseed++, pointer);

		}
		void GLObjCollection::deleteShader(GLShader*& shader)
		{
			if (shader != nullptr)
			{
				std::unique_lock<std::mutex> lock(*mtx_objlist);
				objlist->erase(shader->listiter);
				lock.unlock();

				auto fun = [shader](){
					shader->~GLShader();
					//alloc_Shader.destroy(shader);
					alloc_Shader.deallocate(shader, 1);
				};

				auto command = new  funcDrawCommand<decltype(fun)>(fun,"deleteShader");
				pushDrawThreadCommand(command);

				shader = nullptr;
			}

			
		}

		/***************
			Program部分
		*/
		static std::allocator<GLProgram> alloc_Program;
		GLProgram* GLObjCollection::createProgram()
		{
			auto addr = alloc_Program.allocate(1);

			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto pointer = objlist->insert(objlist->end(), addr);
			lock.unlock();

			return new(addr)GLProgram(idseed++, pointer);
		}
		void GLObjCollection::deleteProgram(GLProgram*& program)
		{
			if (program != nullptr)
			{
				std::unique_lock<std::mutex> lock(*mtx_objlist);
				objlist->erase(program->listiter);
				lock.unlock();

				auto fun = [program](){
					program->~GLProgram();
					alloc_Program.deallocate(program, 1);
				};

				auto command = new  funcDrawCommand<decltype(fun)>(fun,"deleteProgram");
				pushDrawThreadCommand(command);

				program = nullptr;
			}
		}

		/******
			glTex2D部分
		**/
		static std::allocator<GLTex2D> alloc_Tex2d;
		GLTex2D* GLObjCollection::createTex2D()
		{
			auto addr = alloc_Tex2d.allocate(1);

			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto pointer = objlist->insert(objlist->end(), addr);
			lock.unlock();

			return new(addr)GLTex2D(idseed++, pointer);
		}

		void GLObjCollection::deleteTex2D(GLTex2D*& glTex2d)
		{
			if (glTex2d != nullptr)
			{
				std::unique_lock<std::mutex> lock(*mtx_objlist);
				objlist->erase(glTex2d->listiter);
				lock.unlock();

				auto fun = [glTex2d](){
					glTex2d->~GLTex2D();
					alloc_Tex2d.deallocate(glTex2d, 1);
				};

				auto command = new  funcDrawCommand<decltype(fun)>(fun,"deleteTex2D");
				pushDrawThreadCommand(command);

				glTex2d = nullptr;
			}
		}

		/******
			glVBO部分
		**/
		static std::allocator<GLVBO> alloc_VBO;
		GLVBO* GLObjCollection::createVBO()
		{
			auto addr = alloc_VBO.allocate(1);

			std::unique_lock<std::mutex> lock(*mtx_objlist);
			auto pointer = objlist->insert(objlist->end(), addr);
			lock.unlock();

			return new(addr)GLVBO(idseed++, pointer);
		}
		void GLObjCollection::deleteVBO(GLVBO*& glVBO)
		{
			if (glVBO != nullptr)
			{
				std::unique_lock<std::mutex> lock(*mtx_objlist);
				objlist->erase(glVBO->listiter);
				lock.unlock();

				auto fun = [glVBO](){
					glVBO->~GLVBO();
					alloc_VBO.deallocate(glVBO, 1);
				};

				auto command = new  funcDrawCommand<decltype(fun)>(fun, "deleteVBO");
				pushDrawThreadCommand(command);

				glVBO = nullptr;
			}
		}
	}
}