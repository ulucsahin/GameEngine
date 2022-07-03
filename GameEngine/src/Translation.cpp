#include "Translation.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

Translation::Translation()
{
}

Translation::~Translation()
{
}

void Translation::Rotate(glm::mat4& input, Vec3 rotation)
{
    /*
    * rotation in degrees
    */

    float cosA = std::cos(rotation.x * toRadians);
    float cosB = std::cos(rotation.y * toRadians);
    float cosC = std::cos(rotation.z * toRadians);

    float sinA = std::sin(rotation.x * toRadians);
    float sinB = std::sin(rotation.y * toRadians);
    float sinC = std::sin(rotation.z * toRadians);

    float rot[16] = {
        cosB * cosC,     sinA * sinB * cosC - cosA * sinC,        cosA * sinB * cosC + sinA * sinC,     0,
        cosB * sinC,     sinA * sinB * sinC + cosA * cosC,        cosA * sinB * sinC - sinA * cosC,     0,
        -sinB,           sinA * cosB,                             cosA * cosB,                          0,
        0,               0,                                       0,                                    1
    };

    glm::mat4 rotationMatrix = glm::make_mat4(rot);

    input = input * rotationMatrix;
}