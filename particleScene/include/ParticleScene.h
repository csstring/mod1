#include <utility>
#include "Common.h"
#include "ParticleManager.h"
#include "ParticleCLManager.h"
#include "Scene.h"
#include "EnumHeader.h"
#include "ParticleShader.h"

class ParticleScene : public Scene
{
  public:
    ParticleManager _particleManager;
    ParticleCLManager _ParticleCLManager;
    ParticleShader _particleShader;

    bool _isGravityOn = false;
    bool _isGeneratorOn = false;
    PARTICLE_SHAPE particleShape = PARTICLE_SHAPE::QURD;
    int32 drawCount = 0;
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