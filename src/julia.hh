#ifndef VOXEL_ENGINE_HH_
# define VOXEL_ENGINE_HH_

# include <GL/glew.h>
# include <SFML/Window.hpp>
# include <SFML/Graphics.hpp>
# include <SFML/OpenGL.hpp>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>

# include "camera.hh"
# include "shader.hh"

class Julia
{
    public:
        Julia(const char* name,
                    int width,
                    int height,
                    float x,
                    float y);
        ~Julia();

        void loop();

    private:

        void processInput(float elapsed);
        void init_buffers();
        void init_shaders();

        // Window
        sf::Window  window_;

        // Camera
        Camera camera_;
        glm::mat4 modelMatrix_; // Store the projection matrix

        // Buffers
        GLuint vao_;
        GLuint vbo_;
        GLuint ebo_;

        // Shaders
        GLProgram program_;

        // Julia fractal
        glm::vec2 mu_;
        GLuint    iter_;
};

#endif /* !VOXEL_ENGINE_HH_ */
