#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// output data from vertex shader into the fragment shader
out vec2 v_TexCoord;

uniform mat4 u_MVP; // model view projection matrix

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
        
// get value outputted from vertex shader
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

//highp float pixelXPos = gl_FragCoord.x/1000;
//highp float pixelYPos = gl_FragCoord.y/1000;

void main(){
    // color = vec4(0.0, pixelXPos, pixelYPos, 1.0);
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * 0.9 + u_Color * 0.1;
}
