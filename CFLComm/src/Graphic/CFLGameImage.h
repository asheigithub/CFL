#ifndef CFL_GAMEIMAGE_H
#define CFL_GAMEIMAGE_H

#include "CFLTexture.h"
#include "../Content/CFLBinaryReader.h"
#include "../Content/CFLCTF.h"

namespace cfl
{
	namespace content
	{
		class Content;
	}

	namespace graphic
	{
		
		
		class tex2dRef_indata;

		//��װ��Texture2D������
		class Texture2DRef
		{
			friend class cfl::content::Content;
			friend class GameImage;
		public:
			Texture2DRef(const std::shared_ptr<render::textures::Texture2D> texture2d,
				const std::shared_ptr<render::textures::Texture2D> separate = nullptr,
				content::CTFAlphaType alphatype= content::CTFAlphaType::alphachannal,
				content::CTFHasAlpha  hasalpha = content::CTFHasAlpha::notransparent
				);
			~Texture2DRef();

			bool equals(const Texture2DRef* rhs) const;

			
			//��ȡ��δ���͸��ͨ�������������isDone()״̬���ȡ��ֵ�����ǲ���ȷ�ģ�
			content::CTFAlphaType getAlphaType() const;
			//��ȡ�����Ƿ���͸��ֵ��������isDone()״̬���ȡ��ֵ�����ǲ���ȷ�ģ�
			content::CTFHasAlpha  getHasAlpha() const;


			//���õ�2D�����Ƿ��Ѿ���ʼ�ɹ�
			bool isValid() const; 

			//��ȡ�Ƿ��Ѿ��ύGPU�����ύ���������ʼ���ɹ�����Ҫ����֡��ɺ󣬲���֪���Ƿ�ɹ���ʼ���������Ѿ����Է������ô�����Ļ�ͼָ���ˣ�
			bool isDone() const;

			//���õ�2D�����Ƿ���Դ���ڴ���
			bool isFailed() const;

			//�ϴ�2D������ε���ֻ��ִ��һ�Ρ�
			void upload(bool async=false); 

			//��ȡ���õ�����(�������isDone()��isValid()״̬,���п���Ϊ��!)
			render::textures::Texture2D* getTexture2D() const;

			//��ȡ���ӵ�͸������.������Ǹ���͸��������Ϊ�գ������������isDone()��isValid(),���п���Ϊ��
			render::textures::Texture2D* getSeparateTexture() const;


			//���ڻ�ͼʱ������ʱ���´�����
			void setLastRefCounter(size_t frame);




			/**
			* �����������õ����������
			*/
			int __useinbatchCounter() const;
			void __useinbatchCounter(int v);
			/**
			* �ڱ����õ��Ǹ����εĵ�һ�����
			*/
			int __useinbatchStIdx() const;
			void __useinbatchStIdx(int v);

		private:
			Texture2DRef(const file::DirType dir, const CFLString& path );
			
			void addOneRef(); //׷��һ����Texture2D������
			void minusOneRef(); //����һ����Texture2D������


			std::shared_ptr< tex2dRef_indata> texdata;
		};

		

		/*
		������Graphic�л�ͼ��Image���ݡ�����������������ͼ������Ӧ��ԭʼͼƬ��Ϣ
		*/
		class GameImageData
		{
		public:
			
			int width;          //ԭʼͼƬ��
			int height;			//ԭʼͼƬ��
			int clipWidth;		//�ü���͸�������Ŀ��
			int clipHeight;		//�ü���͸�������ĸ߶�

			int padLeft;		//��������ɨ�赽��һ����͸�����ص�λ��
			int padTop;			//��������ɨ�赽��һ����͸�����ص�λ��

			float leftTopU;		//���Ͻ��������Uֵ   
			float leftTopV;		//���Ͻ��������Vֵ   

			
			float rightBottomU;	//���½��������Uֵ	
			float rightBottomV;	//���½��������Vֵ	

			std::shared_ptr<Texture2DRef> refTexture; //���õ�2D����
		};

		/*
		������Graphic�л�ͼ��Image��
		�����õ���������Զ����ü�������
		*/
		class GameImage
		{
		public:
			GameImage(const std::shared_ptr<GameImageData>);
			~GameImage();

			GameImage(const GameImage& rhs);    
			GameImage&
				operator=(const GameImage& rhs);

			//��ȡͼƬ�����Ƿ�Ϊ��
			bool isNull() const{ return imgdata == nullptr; };


			GameImageData& operator*() const
			{
				return *imgdata;
			}

			GameImageData* operator->() const
			{
				return imgdata.get();
			}


		private:
			std::shared_ptr<GameImageData> imgdata;
		};

		


	}
}


#endif // !CFL_GAMEIMAGE_H
