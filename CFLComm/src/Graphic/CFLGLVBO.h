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
			//������Ŀ��
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
				//�����������޸�һ�Σ�ʹ�ö��
				gl_static_draw=GL_STATIC_DRAW,
				//�����������ظ��޸ģ����ʹ��
				gl_dynamic_draw = GL_DYNAMIC_DRAW,
				//�����������޸�һ�Σ�ʹ����������
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
		���㻺��������VBO
		*/
		class GLVBO :public IGLObj
		{
			

			/*
			���̸���VBO..ֻ���ڻ�ͼ�̵߳���
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


			
			/*����VBO
			target-- VboTarget
			usage ---VboUsage
			size ---���������ݴ洢��С�����ֽ�����ʾ
			datasource   ---����Դ
			offset ---����Դָ��ƫ����
			genNOw ---��������.ֻ�ڻ�ͼ�̵߳��ò���Ч
			autoclosedatasource --�Ƿ��ύ���ݺ��Զ�ɾ���ͻ������ݡ�Ĭ��Ϊ��
			*/
			void genBuffer(vbotarget::VboTarget target,
				vbousage::VboUsage usage,
				GLsizeiptr size,
				std::shared_ptr<content::IGLDataResource> datasource,
				size_t offset,
				bool genNow = false,
				bool autoclosedatasource=false);

			/*
			���͸���VBO�����ݵ�ָ��
			buff_offset --�����������е�ƫ��
			size        --���޸ĵ��ֽ���
			datasource          --����Դ
			offset      --����Դָ��ƫ����
			autoclosedatasource --�Ƿ��ύ���ݺ��Զ�ɾ���ͻ������ݡ�Ĭ��Ϊ��
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
