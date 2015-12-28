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

		//包装对Texture2D的引用
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

			
			//获取如何处理透明通道。（如果不是isDone()状态则获取的值可能是不正确的）
			content::CTFAlphaType getAlphaType() const;
			//获取纹理是否含有透明值。（不是isDone()状态则获取的值可能是不正确的）
			content::CTFHasAlpha  getHasAlpha() const;


			//引用的2D纹理是否已经初始成功
			bool isValid() const; 

			//获取是否已经提交GPU。（提交并不代表初始化成功，需要到本帧完成后，才能知道是否成功初始化。但是已经可以发送试用此纹理的绘图指令了）
			bool isDone() const;

			//引用的2D纹理是否资源存在错误。
			bool isFailed() const;

			//上传2D纹理。多次调用只会执行一次。
			void upload(bool async=false); 

			//获取引用的纹理(如果不是isDone()或isValid()状态,则有可能为空!)
			render::textures::Texture2D* getTexture2D() const;

			//获取附加的透明纹理.如果不是附加透明纹理则为空，否则如果不是isDone()或isValid(),则有可能为空
			render::textures::Texture2D* getSeparateTexture() const;


			//当在绘图时被引用时更新此数字
			void setLastRefCounter(size_t frame);




			/**
			* 被批绘制引用到的最后批次
			*/
			int __useinbatchCounter() const;
			void __useinbatchCounter(int v);
			/**
			* 在被引用的那个批次的第一个序号
			*/
			int __useinbatchStIdx() const;
			void __useinbatchStIdx(int v);

		private:
			Texture2DRef(const file::DirType dir, const CFLString& path );
			
			void addOneRef(); //追加一个对Texture2D的引用
			void minusOneRef(); //减少一个对Texture2D的引用


			std::shared_ptr< tex2dRef_indata> texdata;
		};

		

		/*
		用于在Graphic中绘图的Image数据。包含与纹理（或纹理图集）对应的原始图片信息
		*/
		class GameImageData
		{
		public:
			
			int width;          //原始图片宽
			int height;			//原始图片高
			int clipWidth;		//裁剪掉透明区域后的宽度
			int clipHeight;		//裁剪掉透明区域后的高度

			int padLeft;		//从左向右扫描到第一个非透明像素的位置
			int padTop;			//从上向下扫描到第一个非透明像素的位置

			float leftTopU;		//左上角在纹理的U值   
			float leftTopV;		//左上角在纹理的V值   

			
			float rightBottomU;	//右下角在纹理的U值	
			float rightBottomV;	//右下角在纹理的V值	

			std::shared_ptr<Texture2DRef> refTexture; //引用的2D纹理
		};

		/*
		用于在Graphic中绘图的Image。
		对引用的纹理进行自动引用计数管理
		*/
		class GameImage
		{
		public:
			GameImage(const std::shared_ptr<GameImageData>);
			~GameImage();

			GameImage(const GameImage& rhs);    
			GameImage&
				operator=(const GameImage& rhs);

			//获取图片数据是否为空
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
