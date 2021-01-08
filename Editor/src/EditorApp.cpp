#include <Humzer/Humzer.h>
#include <Humzer/Core/EntryPoint.h>
#include "EditorRuntime.h"

//#include "EditorRuntime.h"

namespace Humzer {

	class HumzerEditor : public Application {
	public:
		HumzerEditor()  : runtime(){

		}

		~HumzerEditor() 
		{
			runtime.OnShutdown();
		}

		void ClientOnStart() {
			runtime.OnStart();
		}

		void ClientUpdate(Humzer::Timestep dt) {
			runtime.OnUpdate(dt);
		}
	private:
		EditorRuntime runtime;
	};

	Application* CreateApplication() {
		return new HumzerEditor();
	}


}
