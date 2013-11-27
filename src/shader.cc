#include <fstream>
#include <iostream>

#include "shader.hh"
#include "exceptions.hh"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
    for (GLuint shader: shaders_)
    {
        glDeleteShader(shader);
    }
}

GLuint ShaderManager::fromfile(const char* file, GLenum shaderType)
{
    std::ifstream in{file};

    if (in.good())
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();

        return fromstring(contents.c_str(), shaderType);
    }
    throw ShaderException();
}

GLuint ShaderManager::fromstring(const char* program, GLenum shaderType)
{
    // Compile shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &program, NULL);
    glCompileShader(shader);

    // Check compilation status
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        // Display error message
        std::cerr << "Error while compiling shader" << std::endl;
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cerr << buffer << std::endl;
        throw ShaderException();
    }

    // Add id to shader list
    shaders_.emplace_back(shader);

    return shader;
}


//
// GLProgram
//

GLProgram::GLProgram()
{
}

GLProgram::~GLProgram()
{
    glDeleteProgram(program_);
}

void GLProgram::reset()
{
    glDeleteProgram(program_);
    program_ = glCreateProgram();
    sm_ = ShaderManager();
}

void GLProgram::init()
{
    program_ = glCreateProgram();
}

void GLProgram::addShader(const char* file, GLenum type)
{
    GLuint shader = sm_.fromfile(file, type);
    glAttachShader(program_, shader);
}

void GLProgram::addUniform(const char* name, glm::vec2 vec)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniform2f(location, vec.x, vec.y);
}

void GLProgram::addUniform(const char* name, glm::vec3 vec)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void GLProgram::addUniform(const char* name, glm::vec4 vec)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void GLProgram::addUniform(const char* name, glm::mat4 mat)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void GLProgram::addUniform(const char* name, unsigned i)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniform1i(location, i);
}

void GLProgram::addUniform(const char* name, float f)
{
    GLuint location = glGetUniformLocation(program_, name);
    glUniform1f(location, f);
}

void GLProgram::addAttribute(
    const char* name,
    int size,
    GLenum type,
    bool normalize,
    int stride,
    const GLvoid* offset)
{
    GLint posAttrib = glGetAttribLocation(program_, name);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, size, type, normalize, stride, offset);
}

void GLProgram::finalize()
{
    glBindFragDataLocation(program_, 0, "color");
    glLinkProgram(program_);

    // Check linking
    GLint status;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        std::cerr << "Failed to link program" << std::endl;
        GLchar log[512];
        glGetProgramInfoLog(program_, 512, NULL, log);
        std::cerr << log << std::endl;
        throw ShaderException();
    }

    glUseProgram(program_);
}
