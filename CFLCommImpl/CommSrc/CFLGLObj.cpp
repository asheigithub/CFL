#include "Graphic/CFLGLObj.h"
#include "CFLEntry.h"
namespace cfl
{
	namespace render
	{


		void IGLObj::pushMainLooperCommand(IdrawCommand* cmd)
		{
			CFLContext::getCurrentInstance()->renderLooper->pushCommand(cmd);
		};

		

	}
}

