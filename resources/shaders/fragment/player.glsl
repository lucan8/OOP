#version 330 core

out vec4 fragColor;
in vec2 v_texCoords;

//Coordsinates for the center of the screen
uniform vec2 u_center;
uniform sampler2D u_Texture;
//Units relative to the screen resolution and player coordinates
uniform vec2 u_units;
uniform vec2 u_player_coords;
uniform float u_player_radius;

vec4 yellow = vec4(1.0, 1.0, 0.0, 0.5);
vec4 white = vec4(1.0, 1.0, 1.0, 0.5);

vec4 getPixelColor(float dist, float radius){
    //If pixel is inside the player's circle, return texture color if not white, else return yellow
    if (dist < u_player_radius){
        vec4 tex_color = texture(u_Texture, v_texCoords);
        if (tex_color == white)
            return yellow;//(decomment if using player's aura circle)
            //discard;

        return tex_color;
    }
    //Decomment this block if using player's aura circle

    //if pixel is outside the player's circle but inside the player's aura circle, return yellow
    if (dist < u_player_radius * 2)
        return yellow;
    discard;
           
}
void main(){
    //Calculate the center of the player's circle in screen coordinates
    vec2 player_center = vec2(u_center.x + u_units.x * u_player_coords.x + 0.5,
                              u_center.y + u_units.y * u_player_coords.y + 0.5);

    //If pixel is outside the player's circle, discard it
    float dist = distance(player_center, gl_FragCoord.xy);
    fragColor = getPixelColor(dist, u_player_radius);
}