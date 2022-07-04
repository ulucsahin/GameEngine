#pragma once

#ifdef GENGINE_PLATFORM_WINDOWS

extern GEngine::Application* GEngine::CreateApplication();

int main(int argc, char** argv)
{
	printf("GEngine Starting!");
	auto app = GEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif