#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include <memory>

class Texture2D;
class Sprite;

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader& shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void SetShader(glm::vec3 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
    void DrawTexture(Texture2D& texture, glm::vec3 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
    void BindTexture(Texture2D& texture);

    // Set view matrix to sprite renderer shader
    void SetViewMatrix(glm::mat4 viewMatrix);

    unsigned int quadVAO;

private:
    Shader shader;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif