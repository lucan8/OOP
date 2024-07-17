#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "../../functions/functions.h"
#include "../exceptions/CompileShaderException.h"
#include "../exceptions/LinkShaderException.h"
#include <unordered_map>

class Shader{
private:
    GLuint id;
    std :: unordered_map<std :: string, GLint> uniform_names_cache;
    
    //Creates and compiles a shader(helper function for the constructor)
    GLuint CompileShader(const std :: string& source, GLenum shader_type) const;
    
    std :: string getCompileErorrMessage(GLuint shader_id) const;
    std :: string getLinkErrorMessage() const;
public:
    //Loads and compiles the shaders into a single program
    Shader(const std :: string& vertex_file_path, const std :: string& fragment_file_path);
    ~Shader();
    void bind() const{glUseProgram(this->id);}
    void unbind() const{glUseProgram(0);}
    GLuint get() const{return this->id;}

    //Returns the location of a uniform variable
    GLint getUniformLocation(const std :: string& name);

};