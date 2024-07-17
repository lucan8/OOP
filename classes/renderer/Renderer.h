#pragma once
#include "../VAO/VAO.h"
#include "../IBO/IBO.h"
#include "../shader/Shader.h"

class Renderer{
public:
    static void draw(const VAO& vao, const IBO& ibo, const Shader& shader);
};