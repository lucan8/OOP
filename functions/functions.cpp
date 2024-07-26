#include "functions.h"
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
        throw FileOpenException(__func__, file_path);

    std :: string line;
    while (std :: getline(fin, line))
        content += line + '\n';

    return content;
}


//Returns the positions for the square canvas(center is x, y and 2 * radius is the side of the square)
//left bottom, left top, right top, right bottom
glm :: mat4x2 getCanvasPositions(Coordinates coords, float radius) {
    return glm :: mat4x2(
        coords.x - radius, coords.y - radius, 
        coords.x - radius, coords.y + radius,
        coords.x + radius, coords.y + radius, 
        coords.x + radius, coords.y - radius

    );
}