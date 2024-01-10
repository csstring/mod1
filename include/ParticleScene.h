#include <utility>
#include "Common.h"
#include "ParticleManager.h"
#include "CLManager.h"
#include "Scene.h"
#include "EnumHeader.h"

class ParticleScene : public Scene
{
  public:
    ParticleManager _particleManager;
    CLManager _CLManager;
    bool _isGravityOn = false;
    bool _isGeneratorOn = false;
    PARTICLE_SHAPE particleShape = PARTICLE_SHAPE::QURD;
    int32 drawCount;
    int32 _pointSize = 4;

  public:
    ParticleScene() = delete;
    ParticleScene(uint32 particleCount)
    : _particleManager(particleCount){
    };
    ~ParticleScene(){};

    void initialize();
    void update(float dt);
    void draw();
};