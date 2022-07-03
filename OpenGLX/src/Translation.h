#pragma once

#include <glm/glm.hpp>

#define toRadians (3.141592653589793238/180.0)

struct Vec3
{
	// rotation angles
	float x, y, z;
};

class Translation
{
public:
	static void Rotate(glm::mat4& input, Vec3 rotation);

private:
	Translation();
	~Translation();

};

