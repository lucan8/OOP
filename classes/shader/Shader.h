#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>

class Shader;
typedef std :: unique_ptr<Shader> unique_shader;

class Shader{
private:
    GLuint id;
    std :: unordered_map<std :: string, GLint> uniform_names_cache;
    
    //Creates and compiles a shader(helper function for the constructor)
    GLuint CompileShader(const std :: string& source, GLenum shader_type) const;
public:
    //Loads and compiles the shaders into a single program
    Shader(const std :: string& vertex_file_path, const std :: string& fragment_file_path);
    Shader(): id(0){}
    //Two shaders can't have the same id(if one gets deleted, the other will be invalid)
    Shader(const Shader& other) = delete;
    //Moving means we now have a shader with the same id as the other, which is invalid
    Shader(Shader&& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    //Deletes the program
    ~Shader();
    
    void bind() const{glUseProgram(this->id);}
    void unbind() const{glUseProgram(0);}
    GLuint get() const{return this->id;}

    //Returns the location of a uniform variable
    GLint getUniformLocation(const std :: string& name);

    void setUniform1i(const std :: string& name, int value);
    void setUniform1f(const std :: string& name, float value);
    void setUniform2f(const std :: string& name, const glm :: vec2& values);
    void setUniform3f(const std :: string& name, const glm :: vec3& values);
    void setUniform4f(const std :: string& name, const glm :: vec4& values);
    void setUniformMat4f(const std :: string& name, const glm :: mat4& values);

};