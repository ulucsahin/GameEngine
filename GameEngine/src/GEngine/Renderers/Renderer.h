#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

	virtual void Start() = 0;
	virtual bool WindowShouldClose() = 0;

};

