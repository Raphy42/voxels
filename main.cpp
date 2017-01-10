#include <iostream>
#include <vector>
#include <glm/common.hpp>
#include "gl_utils.h"
#include "ChunkBuffer.h"
#include "Noise.h"

enum class block : int {
    WATER = 0,
    DIRT,
    GRASS,
    STONE,
    SAND,
    ORE
};

template<int seed>
struct noise_helper {
    static void _noise(buffer::ChunkBuffer::ChunkHdl buffer) {
        if (buffer->isInitialized())
            return;
        else
            buffer->setInitialized(true);
        for (int x = 0; x < chunk::detail::dimension::width; ++x) {
            for (int z = 0; z < chunk::detail::dimension::depth; ++z) {
                const float n = noise::noise2d(
                        (x + buffer->_x * chunk::detail::dimension::width) / 256.0,
                        (z + buffer->_z * chunk::detail::dimension::depth) / 256.0,
                        seed,
                        5, 0.8) * 4;
                const int h = n * 2;
                int y = 0;

                for (y = 0; y < chunk::detail::dimension::height; ++y) {
                    if (y + buffer->_y * chunk::detail::dimension::width >= h) {
                        if (y + buffer->_x * buffer->_y < 68) {
                            buffer->set(x, y, z, block::WATER);
                            continue;
                        } else {
                            //misc generation trees etc...
                        }//todo environmental control
                    }
                }

                float r = noise::noise3d_abs((x + buffer->_x * chunk::detail::dimension::width) / 16.0,
                                             (y + buffer->_y * chunk::detail::dimension::height) / 16.0,
                                             (z + buffer->_z * chunk::detail::dimension::depth) / 16.0, -seed, 2, 1);

                if (n + r * 5 < 4)
                    buffer->set(x, y, z, block::SAND);
                else if (n + r * 5 < 8)

                    buffer->set(x, y, z,
                                (h < 68 || (y + buffer->_y * chunk::detail::dimension::height) < h - 1) ? block::DIRT
                                                                                                          : block::GRASS);
                else if (r < 1.25)
                    buffer->set(x, y, z, block::STONE);
                else
                    buffer->set(x, y, z, block::ORE);
            }
        }
        buffer->setChanged(true);
    }

    static void apply(buffer::ChunkBuffer<block> &buffer) {
        buffer.apply(_noise);
    }
};

int main() {
    buffer::ChunkBuffer<block> buffer;

    GLFWwindow *window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1400, 1000, "test", nullptr, nullptr);
    std::cout << "Hello, World!" << std::endl;

    std::pair<int, int> framebuffer;
    glfwGetFramebufferSize(window, &framebuffer.first, &framebuffer.second);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, framebuffer.first, framebuffer.second);

    glfwSetKeyCallback(window, [](GLFWwindow *w, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(w, true);
    });

    noise_helper<1234>::apply(buffer);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glfwSwapBuffers(window);
    }

    return 0;
}