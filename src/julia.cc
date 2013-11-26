#include <iostream>
#include <string>

#include "julia.hh"
#include "exceptions.hh"

namespace
{
    void print_opengl_info()
    {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        std::cout << "Renderer: " << renderer << std::endl;
        std::cout << "OpenGL version: " << version << std::endl;
    }
}


Julia::Julia(const char* name, int width, int height, float x, float y)
    : window_(sf::VideoMode(width, height), name, sf::Style::Default, sf::ContextSettings(32)),
      modelMatrix_(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))),
      mu_(x, y),
      iter_(10)
{
    // Window options
    window_.setVerticalSyncEnabled(true);

    // Init Glew
    glewExperimental = GL_TRUE;
    glewInit();

    // Print infos about OpenGL version
    print_opengl_info();

    // Init buffers and shaders
    program_.init();
    init_buffers();
    init_shaders();

    // Init camera
    camera_.setPosition(glm::vec3(0, 0, 5));
    camera_.setViewportAspectRatio(window_.getSize().x / window_.getSize().y);
}


Julia::~Julia()
{
    // Delete buffers
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Julia::loop()
{
    sf::Clock clock;
    sf::Time elapsed = clock.getElapsedTime();
    bool running = true;

    while (running)
    {
        sf::Time time = clock.getElapsedTime();

        // handle events
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
                break;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    running = false;
                    break;
                }
            }
        }

        // Process input
        processInput(elapsed.asSeconds());

        // frame per second
        float fps = 1.f / (time.asSeconds() - elapsed.asSeconds());
        std::cout << fps << std::endl;

        // Update elapsed time
        elapsed = time;

        // Update model view projection matrix
        program_.addUniform("mvp", camera_.matrix() * modelMatrix_);
        program_.addUniform("mu", mu_);
        program_.addUniform("iter", iter_);

        // Draw
        glClearColor( 0.2f, 0.2f, 0.3f, 0.0f );
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a fullscreen Quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers
        window_.display();
    }
}

void Julia::processInput(float elapsed)
{
    const float moveSpeed = 0.01;
    const float rotationSpeed = 0.1;

    // Z-axis
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        camera_.offsetPosition(elapsed * moveSpeed * -camera_.forward());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        camera_.offsetPosition(elapsed * moveSpeed * camera_.forward());

    // Y-axis
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        camera_.offsetPosition(elapsed * moveSpeed * -camera_.right());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        camera_.offsetPosition(elapsed * moveSpeed * camera_.right());

    // X-axis
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        camera_.offsetPosition(elapsed * moveSpeed * -glm::vec3(0, 1, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        camera_.offsetPosition(elapsed * moveSpeed * glm::vec3(0, 1, 0));

    // Rotation arround X-axis
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        camera_.offsetOrientation(-rotationSpeed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        camera_.offsetOrientation(rotationSpeed, 0);

    // Rotation arround Y-axis
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        camera_.offsetOrientation(0, -rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        camera_.offsetOrientation(0, rotationSpeed);

    // Fractal stuff
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        ++iter_;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && iter_ > 1)
        --iter_;
}


void Julia::init_buffers()
{
    float quad[] =
    {
        -1.0f,  1.0f, // Top-left
         1.0f,  1.0f, // Top-right
         1.0f, -1.0f, // Bottom-right
        -1.0f, -1.0f  // Bottom-left
    };


    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // Create VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Create VBO
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Bind quad to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof (quad), quad, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    // Bind elements to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}


void Julia::init_shaders()
{
    program_.addShader("shaders/vertex.shader", GL_VERTEX_SHADER);
    program_.addShader("shaders/fragment.shader", GL_FRAGMENT_SHADER);

    program_.finalize();

    program_.addAttribute("position", 2, GL_FLOAT, GL_FALSE, 0, 0);
}
