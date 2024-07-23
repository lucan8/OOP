#version 330 core

out vec4 fragColor;
in vec2 v_texCoords;

//Coordinates for the center of the screen
uniform vec2 u_center;
uniform sampler2D u_Texture;
//Units relative to the screen resolution and player coordinates
uniform vec2 u_units;
//Player coordinates and radius in pitch coords
uniform vec2 u_player_coords;
uniform float u_player_radius;

vec4 yellow = vec4(1.0, 1.0, 0.0, 0.5);
vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

//Used for drawing the player's aura circle and texture circle
vec4 getPixelColor(vec2 curr_pixel, vec2 center,  float radius){
    //Calculate the distance between the current pixel and the center
    float dist = distance(curr_pixel, center);
    //If pixel is inside circle, return texture color if not white, else return yellow
    if (dist < radius){
        vec4 tex_color = texture(u_Texture, v_texCoords);
        if (tex_color == white)
            return yellow;

        return tex_color;
    }
    discard;
           
}
void main(){
    //Calculate the center of the player's circle in screen coordinates
    vec2 player_center = vec2(u_center.x + u_units.x * u_player_coords.x + 0.5,
                              u_center.y + u_units.y * u_player_coords.y + 0.5);

    //Getting the color of the pixel based on its distance to the player's center
    fragColor = getPixelColor(gl_FragCoord.xy, player_center, u_player_radius * u_units.x);
}