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
void main(){
    //Calculate the center of the player's circle in screen coordinates
    vec2 player_center = vec2(u_center.x + u_units.x * u_player_coords.x + 0.5,
                              u_center.y + u_units.y * u_player_coords.y + 0.5);

    //If pixel is outside the player's circle, discard it
    if (distance(player_center, gl_FragCoord.xy) > u_player_radius) {
        discard;
    }
    vec4 texColor = texture(u_Texture, v_texCoords);
    //Removing white pixels from texture
    // if (texColor == vec4(1.0, 1.0, 1.0, 1.0)) {
    //     discard;
    // }
    fragColor = texColor;
}