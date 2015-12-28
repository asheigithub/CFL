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
		����GLES�Ķ�������Դ�ӿ�
		����ԭʼ�������ݣ�ԭ������ͼƬ���ݵȵ�
		*/
		class IGLDataResource 
		{
		public:
			virtual ~IGLDataResource() = default;

			//��ȡ����
			virtual void* getData() = 0;
			//�����ֽڳ���
			virtual size_t getByteLength() = 0;

			//�Ƿ�����ɲ���
			virtual bool isDone() = 0;
			//�����Ƿ���ȷ
			virtual bool isValid() = 0;
			//��ѯ������Ϣ
			virtual const char* getError()=0;
			//ͬ��������Դ
			virtual void load() = 0;
			//�첽������Դ
			virtual void loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args) = 0;
			//��ȡ���ؽ���
			virtual float getProgress() = 0;
			//֪ͨ�����ͷ���Դ���������ͷ�֮�����´�ʹ����Ҫ�ٴζ�ȡ��
			virtual void close() = 0;
		
			

		};


	}
}




#endif // !CFL_IGLDATARESOURCE_H
