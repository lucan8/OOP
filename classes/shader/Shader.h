#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "../../functions/functions.h"
#include "../exceptions/CompileShaderException.h"
#include "../exceptions/LinkShaderException.h"

class Shader{
private:
    GLuint id;
    //Creates and compiles a shader(helper function for the constructor)
    GLuint CompileShader(const std :: string& source, GLenum shader_type) const;

    std :: string getCompileErorrMessage(GLuint shader_id) const;
    std :: string getLinkErrorMessage() const;
public:
    //Loads and compiles the shaders into a single program
    Shader(const std :: string& vertex_file_path, const std :: string& fragment_file_path);
    ~Shader();
    void use() const{glUseProgram(this->id);}
};