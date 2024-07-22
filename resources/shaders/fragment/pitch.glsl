#version 330 core

out vec4 fragColor;
in vec2 v_texCoords;

uniform sampler2D u_Texture;

void main(){
    vec4 texColor = texture(u_Texture, v_texCoords);
    fragColor = texColor;
}