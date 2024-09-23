#version 330 core

out vec4 fragColor;
in vec2 v_texCoords;

//Coordinates for the center of the screen
uniform vec2 u_screen_center;
uniform sampler2D u_texture;

//Units relative to the screen resolution and player coordinates
uniform vec2 u_screen_units;

//Entity coordinates and radius in pitch coords
uniform vec2 u_entity_coords;
uniform float u_entity_radius;

uniform int u_entity_type;
uniform vec4 u_aura_color;
uniform vec3 u_font_color;

vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
//Entity types
const int pitch = 0;
const int ball = 1;
const int player = 2;
const int score = 3;
const int team_crest = 4;
const int letter = 5;


//If pixel is withing the entity's circle, return the texture color, else discard
vec4 getPixelColor(vec2 curr_pixel, vec2 center,  float radius){
    //Calculate the distance between the current pixel and the center
    float dist = distance(curr_pixel, center);
    //If pixel is inside circle, return texture color
    if (dist < radius)
        return texture(u_texture, v_texCoords);
    discard; 
}

//Returns the player's aura color if outside player radius(but inside aura radius)
//or the texture color if inside player radius
vec4 getPlayerPixelColor(vec2 curr_pixel, vec2 center,  float radius){
    vec4 color = getPixelColor(curr_pixel, center, radius);
    
    //Color is white, means we are inside the canvas, so we return the player's aura color
    if (color == white)
        return u_aura_color;
    else
        return color;
}


vec4 getBallPixelColor(vec2 curr_pixel, vec2 center,  float radius){
    return getPixelColor(curr_pixel, center, radius);
}


vec4 getPitchPixelColor(){
    return texture(u_texture, v_texCoords);
}

vec4 getScorePixelColor(){
    return texture(u_texture, v_texCoords);
}


vec4 getTeamCrestPixelColor(){
    vec4 color = texture(u_texture, v_texCoords);
    if (color == white)
        discard;
    else
        return color;
}


vec4 getLetterPixelColor(){
    vec4 color = texture(u_texture, v_texCoords);
    return vec4(u_font_color, color.x);
}


void main(){
    //Calculate the center of the entity's circle in screen coordinates
    vec2 entity_center = vec2(u_screen_center.x + u_screen_units.x * u_entity_coords.x + 0.5,
                              u_screen_center.y + u_screen_units.y * u_entity_coords.y + 0.5);

    switch (u_entity_type){
        case pitch:
            fragColor = getPitchPixelColor();
            break;
        case ball:
            fragColor = getBallPixelColor(gl_FragCoord.xy, entity_center, u_entity_radius * u_screen_units.x);
            break;
        case player:
            fragColor = getPlayerPixelColor(gl_FragCoord.xy, entity_center, u_entity_radius * u_screen_units.x);
            break;
        case score:
            fragColor = getScorePixelColor();
            break;
        case team_crest:
            fragColor = getTeamCrestPixelColor();
            break;
        case letter:
            fragColor = getLetterPixelColor();
            break;
        default:
            discard;
    }
}