#include "Content/CFLMemoryDataSource.h"

namespace cfl
{
	namespace content
	{
		MemoryDataSource::MemoryDataSource(std::shared_ptr<char> data, const size_t bytes)
			:IGLDataResource(), data(data), bytes(bytes)
		{
			
		}

		bool MemoryDataSource::isDone(){ return true; }

		bool MemoryDataSource::isValid() { return data != nullptr; }

		const char* MemoryDataSource::getError(){ return "\0"; }

		void MemoryDataSource::loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args)
		{
			if (progress != nullptr)
			{
				progress(std::make_shared<MemoryDataSource>(std::move(*this)), args);
			}
			if (callbacker != nullptr)
			{
				callbacker(std::make_shared<MemoryDataSource>(std::move(*this)), args);
			}
		}

	}
}
