#include "Common.h"
#include <memory>
#include "EnumHeader.h"

class Shader;
class ShaderManager
{
  private:
    std::unique_ptr<Shader> circleShader;
    std::unique_ptr<Shader> qurdShader;

  public:
    ShaderManager(){};
    ~ShaderManager(){};

    void        initialize();
    void        use(PARTICLE_SHAPE input);
};
