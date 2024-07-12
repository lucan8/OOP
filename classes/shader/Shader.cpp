#include "Shader.h"

Shader :: Shader(const std :: string& vertex_file_path, const std :: string& fragment_file_path){
    this->id =  glCreateProgram();
    //Reading the shaders source code
    std :: string VertexShaderSrc = ReadFile(vertex_file_path);
    std :: string FragmentShaderSrc = ReadFile(fragment_file_path);

    //Compiling the shaders
    GLuint v_shader_id = CompileShader(VertexShaderSrc, GL_VERTEX_SHADER);
    GLuint f_shader_id =  CompileShader(FragmentShaderSrc, GL_FRAGMENT_SHADER);

    //Attaching the shaders to the program
    glAttachShader(this->id, v_shader_id);
    glAttachShader(this->id, f_shader_id);

    //Linking the program
    glLinkProgram(this->id);
    glValidateProgram(this->id);

    GLint result;
    glGetProgramiv(this->id, GL_LINK_STATUS, &result);

    //Checking if the program linked successfully
    if (result == GL_FALSE)
        throw LinkShaderException(__func__, this->getLinkErrorMessage());

    //Deleting the shaders
    glDeleteShader(v_shader_id);
    glDeleteShader(f_shader_id);
}


GLuint Shader :: CompileShader(const std :: string& source, GLenum shader_type) const{
    GLuint shader_id = glCreateShader(shader_type);
    const char* src = source.c_str();

    //Setting the source code for the shader
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    //Checking if the shader compiled successfully
    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    //If the shader failed to compile, print the error message
    if (!result){
        throw CompileShaderException(__func__, this->getCompileErorrMessage(shader_id));
        return 0;
    }
    return shader_id;
}


std :: string Shader :: getCompileErorrMessage(GLuint shader_id) const{
    //Getting the length of the error message
    int length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

    //Allocating memory for the error message
    char* message = (char*)alloca(length * sizeof(char));
    //Getting the error message
    glGetShaderInfoLog(shader_id, length, &length, message);

    return message;
}


std :: string Shader :: getLinkErrorMessage() const{
    //Getting the length of the error message
    int length;
    glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &length);

    //Allocating memory for the error message
    char* message = (char*)alloca(length * sizeof(char));
    //Getting the error message
    glGetProgramInfoLog(this->id, length, &length, message);

    return message;
}

Shader :: ~Shader(){
    glDeleteProgram(this->id);
}