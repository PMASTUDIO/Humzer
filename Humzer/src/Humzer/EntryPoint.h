#pragma once

#ifdef HUM_PLATFORM_WINDOWS

	extern Humzer::Application* Humzer::CreateApplication();

	int main(int argc, char** argv) {

		auto app = Humzer::CreateApplication();
		app->Run();
		delete app;

	}

#else
	#error Humzel Only supports Windows x64!
#endif
