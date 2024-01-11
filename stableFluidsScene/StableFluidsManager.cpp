#include "StableFluidsManager.h"
#include "Camera.h"
#include "Particle.h"
#include "DDSLoder.h" 
struct fluid
{
  glm::vec4 _position;
  glm::vec2 _uv;
};

StableFluidsManager::~StableFluidsManager()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);
}

void StableFluidsManager::initialize()
{
  DDSLoder textureLoder(std::filesystem::canonical("./stableFluidsScene/uvmap.dds"));
  
  std::vector<fluid> fluids;
  fluids.push_back({glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}); // Bottom-left
  fluids.push_back({glm::vec4( 1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right

    // Triangle 2
  fluids.push_back({glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}); // Top-left
  fluids.push_back({glm::vec4( 1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}); // Bottom-right
  fluids.push_back({glm::vec4( 1.0f,  1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}); // Top-right
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);
  _textureID = textureLoder.loadDDS();
  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, fluids.size() * sizeof(fluid), fluids.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fluid), (void*)offsetof(fluid, _position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fluid), (void*)offsetof(fluid, _uv));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void StableFluidsManager::draw(uint32 drawCount)
{
  glBindVertexArray(_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
