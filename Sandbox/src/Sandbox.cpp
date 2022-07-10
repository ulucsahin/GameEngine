#include <GEngine.h>

// #include <iostream>

class Sandbox : public GEngine::Application
{
public:
	Sandbox()
	{
		printf("Sandbox started!\n");
	}

	~Sandbox()
	{

	}

};

GEngine::Application* GEngine::CreateApplication()
{
	return new Sandbox();
}