#include "functions.h"
#include <sstream>
#include <fstream>
#include "../classes/exceptions/FileOpenException.h"
#include "../classes/constants/Constants.h"

std :: vector<std :: string> split(const std :: string& str, char sep){
    std :: istringstream str_stream(str);
    std :: string split_elem;
    std :: vector<std :: string> split_string;

    while (getline(str_stream, split_elem, sep))
        split_string.push_back(split_elem);

    return split_string;
}

//Reads file content and returns it as a string
std :: string readFile(const std :: string& file_path, bool is_binary){
    std :: string content;
    std :: ifstream fin(file_path, is_binary ? std :: ios :: binary : std :: ios :: in);

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


void setTextureCoords(glm :: mat4& entity_vertices){
    //Getting the texture coordinates
    float max_x = Constants :: getVal("TEXTURE_MAX_X"), max_y = Constants :: getVal("TEXTURE_MAX_Y");
    float min_x = Constants :: getVal("TEXTURE_MIN_X"), min_y = Constants :: getVal("TEXTURE_MIN_Y");

    //Setting the texture coordinates
    entity_vertices[0][2] = min_x, entity_vertices[0][3] = min_y,
    entity_vertices[1][2] = min_x, entity_vertices[1][3] = max_y,
    entity_vertices[2][2] = max_x, entity_vertices[2][3] = max_y,
    entity_vertices[3][2] = max_x, entity_vertices[3][3] = min_y;
}


bool isBetween(float val, float min_val, float max_val){
    return val >= min_val && val <= max_val;
}


glm :: vec4 convertCoords(const glm :: vec2& from_coords, const glm :: mat4& from_proj,
                          const glm :: mat4& to_proj){
    return glm :: inverse(to_proj) * from_proj * glm :: vec4(from_coords, 0.0f, 1.0f);
}


float changeUnit(float value, float nr_units_from, float nr_units_to){
    return value * nr_units_to / nr_units_from;
}


glm :: vec4 normalizeColor(glm :: vec4 color){
    const float max_color_val = 255;
    return glm :: vec4(color.r / max_color_val, color.g / max_color_val,
                       color.b / max_color_val, color.a / max_color_val);
}