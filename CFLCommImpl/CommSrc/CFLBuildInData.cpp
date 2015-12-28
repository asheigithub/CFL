#include "Content/CFLBuildInData.h"

namespace cfl
{
	namespace content
	{
		
		BuildInData::BuildInData(void* data,const size_t bytes)
			:IGLDataResource(), data(data), bytes(bytes)
		{
			
		}

		bool BuildInData::isDone(){ return true; }

		bool BuildInData::isValid() { return data !=nullptr ; }

		const char* BuildInData::getError(){ return "\0"; }

		void BuildInData::loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args)
		{
			if (progress != nullptr)
			{
				progress(std::make_shared<BuildInData>(std::move(*this)), args);
			}
			if (callbacker != nullptr)
			{
				callbacker(std::make_shared<BuildInData>( std::move( *this)),args);
			}
		}

	}
}
