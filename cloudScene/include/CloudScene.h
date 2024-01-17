#include <utility>
#include "Common.h"
#include "CloudManager.h"
#include "CloudCLManager.h"
#include "Scene.h"
#include "EnumHeader.h"
#include "CloudShader.h"

class CloudScene : public Scene
{
  public:
    CloudManager _cloudManager;
    CloudCLManager _cloudCLManager;
    CloudShader _cloudShader;
    glm::vec3 _lightDir;
    glm::vec3 _lightColor;
    bool _isGravityOn = false;
    bool _isGeneratorOn = false;

  public:
    CloudScene(){}
    ~CloudScene(){}

    void initialize();
    void guiRender();
    void update(float dt);
    void draw();
};