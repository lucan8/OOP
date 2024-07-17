#include "Renderer.h"

void Renderer :: draw(const VAO& vao, const IBO& ibo, const Shader& shader){
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}