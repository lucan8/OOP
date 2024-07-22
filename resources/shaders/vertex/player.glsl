#version 330 core
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;
uniform mat4 u_MVP;

void main(){
    gl_Position = vertexPosition * u_MVP;
    v_texCoords = texCoords;
}