//
// Created by Raphaël Dantzer on 10/01/17.
//

#ifndef VOXELS_GL_UTILS_H
#define VOXELS_GL_UTILS_H

#define GLFW_INCLUDE_GLCOREARB

#include <glfw/glfw3.h>
#include <OpenGL/gl3.h>
#include <string>
#include <iostream>
#include "utils.h"

namespace gl_utils {
inline GLuint createFromFile(GLenum type, const char *filename) {
    const std::string source = utils::readFile(filename);
    const GLchar *s = source.c_str();
    int status{0};
    GLuint shader;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &s, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLchar log[512];

        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << log << std::endl;
    }
    return shader;
}

inline GLuint createProgramFromFiles(const char *vs, const char *fs) {
    GLuint program;
    
    program = glCreateProgram();
    glAttachShader(program, createFromFile(GL_VERTEX_SHADER, vs));
    glAttachShader(program, createFromFile(GL_FRAGMENT_SHADER, fs));
    
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        char log[512];

        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << log << std::endl;
    }
    return program;
}
}

#endif //VOXELS_GL_UTILS_H
