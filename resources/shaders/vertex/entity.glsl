#version 330 core
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;
uniform mat4 u_projection;
//uniform mat4 u_model;
void main(){
    gl_Position = u_projection * vertexPosition;
    v_texCoords = texCoords;
}