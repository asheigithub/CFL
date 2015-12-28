#include "Graphic/CFLGameImage.h"

namespace cfl
{
	
	namespace graphic
	{
		GameImage::GameImage(const std::shared_ptr<GameImageData> data) :imgdata(data)
		{
			if (data)
			{
				data->refTexture->addOneRef();
			}
			
		}

		GameImage::~GameImage()
		{
			if (imgdata != nullptr)
			{
				imgdata->refTexture->minusOneRef();
			}
		}

		GameImage::GameImage(const GameImage& rhs) :imgdata(rhs.imgdata)
		{
			if (imgdata != nullptr)
			{
				imgdata->refTexture->addOneRef();
			}
		}

		GameImage&
			GameImage::operator=(const GameImage& rhs)
		{
			if (rhs.imgdata)
			{
				rhs.imgdata->refTexture->addOneRef();
			}

			if (imgdata)
			{
				imgdata->refTexture->minusOneRef();
			}

			imgdata = rhs.imgdata;

			return *this;
		}

	}
}