#version 330 core

layout(location = 0) out vec4 color;
//highp float pixelXPos = gl_FragCoord.x/1000;
//highp float pixelYPos = gl_FragCoord.y/1000;

uniform sampler2D myTextureSampler;
// Interpolated values from the vertex shaders

in vec2 UV;

void main() {
    color = texture(myTextureSampler, UV);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}