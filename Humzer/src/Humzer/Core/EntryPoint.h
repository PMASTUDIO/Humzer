#pragma once

#ifdef HUM_PLATFORM_WINDOWS

	extern Humzer::Application* Humzer::CreateApplication();

	int main(int argc, char** argv) {

		Humzer::Log::Init();

		HUM_CORE_WARN("Log system initialized!");
		HUM_CORE_INFO("Hello Humzer!");

		auto app = Humzer::CreateApplication();
		app->Run();
		delete app;

	}

#else
	#error Humzel Only supports Windows x64!
#endif
