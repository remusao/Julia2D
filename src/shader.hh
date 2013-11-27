#ifndef SHADER_HH_
# define SHADER_HH_

# include <vector>
# include <GL/glew.h>
# include <glm/glm.hpp>

class ShaderManager
{
    public:
        ShaderManager();
        ~ShaderManager();

        ShaderManager(const ShaderManager&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        ShaderManager(ShaderManager&&) = default;
        ShaderManager& operator=(ShaderManager&&) = default;

        GLuint fromfile(const char* file, GLenum shaderType);
        GLuint fromstring(const GLchar* shader, GLenum shaderType);

    private:
        std::vector<GLuint> shaders_;
};


class GLProgram
{
    public:
        GLProgram();
        ~GLProgram();

        void reset();
        void init();
        void addShader(const char* file, GLenum type);
        void addUniform(const char* name, glm::mat4 mat);
        void addUniform(const char* name, glm::vec2 mat);
        void addUniform(const char* name, glm::vec3 mat);
        void addUniform(const char* name, glm::vec4 mat);
        void addUniform(const char* name, unsigned i);
        void addUniform(const char* name, float f);
        void addAttribute(const char* name, int, GLenum, bool, int, const GLvoid*);
        void finalize();

    private:
        GLuint          program_;
        ShaderManager   sm_;
};

#endif /* !SHADER_HH_ */
