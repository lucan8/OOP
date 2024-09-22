#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    Shader(): id(0){}
    ~Shader(){};
    //Frees the memory and invalides the program
    void deleteProgram();
    
    void bind() const{glUseProgram(this->id);}
    void unbind() const{glUseProgram(0);}
    GLuint get() const{return this->id;}

    //Returns the location of a uniform variable
    GLint getUniformLocation(const std :: string& name);

    void setUniform1i(const std :: string& name, int value);
    void setUniform1f(const std :: string& name, float value);
    void setUniform2f(const std :: string& name, const glm :: vec2& values);
    void setUniform4f(const std :: string& name, const glm :: vec4& values);
    void setUniformMat4f(const std :: string& name, const glm :: mat4& values);

};