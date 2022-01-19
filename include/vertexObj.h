//
// Created by fesvh on 2022/1/11.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef CGCHUYAN_VERTEXOBJ_H
#define CGCHUYAN_VERTEXOBJ_H


class VAO{
private:
public:
    unsigned int ID;
    VAO() {
        glGenVertexArrays(1, &ID);
    }
    void VAOBind(){
        glBindVertexArray(ID);

    }

};
class VBO{
private:
public:
    unsigned int ID;
    VBO(){
        glGenBuffers(1, &ID);
    }
    void VBOBind(){
        glBindBuffer(ID);
    }
};

#endif //CGCHUYAN_VERTEXOBJ_H
