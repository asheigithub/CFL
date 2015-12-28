#include "CFLDrawCommand.h"
#include <memory>
#include <deque>
#include <mutex>

#include "CFLEntry.h"

namespace cfl
{
	namespace render
	{

		class DrawQueue
		{
			friend class RenderThreadLooper;

		private:
			
			std::deque<IdrawCommand*> deque;
			void pushCommand(IdrawCommand* cmd)
			{
				deque.emplace_back(cmd);
			};

			void executeQueue(CFLContext* context)
			{
				auto end = deque.end();
				auto begin = deque.begin();

				while (begin !=end)
				{
					auto summary = (*begin)->cmdSummary;
					//LOGI("begin do%s\n", summary);

					(*begin)->execute(context);

					(*begin)->over();
					//LOGI("end do%s\n", summary);
					
					begin++;
				}

				deque.clear();
			};

			
		};

		
		RenderThreadLooper::RenderThreadLooper() :backQueue(new DrawQueue()), 
			renderQueue(new DrawQueue())
		{	
		}

		RenderThreadLooper::~RenderThreadLooper()
		{
			delete backQueue; delete renderQueue; 
		}

		void RenderThreadLooper::pushCommand(IdrawCommand* cmd)
		{
			backQueue->pushCommand(cmd);
		}

		void RenderThreadLooper::render(CFLContext* context)
		{
			renderQueue->executeQueue(context);

		}
		void RenderThreadLooper::swapQueue(CFLContext* context)
		{
			context->graphic->onBeforRenderQueueSwap(context);
			std::swap(renderQueue, backQueue);
			context->graphic->onRednerQueueSwaped(context);
		}



	}
}