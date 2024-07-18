#version 330 core
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;
void main(){
    gl_Position = vertexPosition;
    v_texCoords = texCoords;
}