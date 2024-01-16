#include <utility>
#include "Common.h"
#include "StableFluidsManager.h"
#include "StableFluidsCLManager.h"
#include "Scene.h"
#include "EnumHeader.h"
#include "StableFluidsShader.h"

class StableFluidsScene : public Scene
{
  public:
    StableFluidsManager _stableFluidsManager;
    StableFluidsCLManager _stableFluidsCLManager;
    StableFluidsShader _stableFluidsShader;

    bool _isGravityOn = false;
    bool _isGeneratorOn = false;

  public:
    StableFluidsScene(){}
    ~StableFluidsScene(){}

    void initialize();
    void guiRender();
    void update(float dt);
    void draw();
};