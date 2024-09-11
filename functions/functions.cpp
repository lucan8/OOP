#include "functions.h"
#include "../classes/exceptions/FileOpenException.h"

std :: vector<std :: string> split(const std :: string& str, char sep){
    std :: istringstream str_stream(str);
    std :: string split_elem;
    std :: vector<std :: string> split_string;

    while (getline(str_stream, split_elem, sep))
        split_string.push_back(split_elem);

    return split_string;
}

//Reads file content and returns it as a string
std :: string ReadFile(const std :: string& file_path){
    std :: string content;
    std :: ifstream fin(file_path);

    if (!fin)
        throw FileOpenException(__FILE__, __func__, __LINE__, file_path);

    std :: string line;
    while (std :: getline(fin, line))
        content += line + '\n';

    return content;
}


glm :: mat4x2 getCanvasPositions(glm :: vec2 coords, float radius){
    return glm :: mat4x2(
        coords + glm :: vec2(-radius, -radius), 
        coords + glm :: vec2(-radius, radius),
        coords + glm :: vec2(radius, radius),
        coords + glm :: vec2(radius, -radius)

    );
}


glm :: mat4x2 getCanvasPositions(glm :: vec2 coords, float max_x, float max_y){
    return glm :: mat4x2(
        coords + glm :: vec2(-max_x, -max_y), 
        coords + glm :: vec2(-max_x, max_y),
        coords + glm :: vec2(max_x, max_y),
        coords + glm :: vec2(max_x, -max_y)

    );
}


bool isBetween(float val, float min_val, float max_val){
    return val >= min_val && val <= max_val;
}


glm :: vec4 normalizeColor(glm :: vec4 color){
    const float max_color_val = 255;
    return glm :: vec4(color.r / max_color_val, color.g / max_color_val,
                       color.b / max_color_val, color.a / max_color_val);
}