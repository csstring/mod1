#include "Common.h"
#include <memory>
#include "EnumHeader.h"

class Shader;
class ShaderManager
{
  private:
    std::unique_ptr<Shader> circleShader;
    std::unique_ptr<Shader> qurdShader;

    uint32      _curProgramId;
  public:
    ShaderManager(){};
    ~ShaderManager(){};

    void        initialize();
    void        use(PARTICLE_SHAPE input);
    void        setMat4(const std::string &name, glm::mat4 mat4) const;
    void        setUint(const std::string &name, unsigned int index) const;
    void        setVec4(const std::string &name, glm::vec4 vec4) const;
    void        setVec3(const std::string &name, glm::vec3 vec3) const;
    void        setFloat(const std::string &name, float index) const;
};
