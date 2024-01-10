#pragma once
#include "Common.h"

class Shader
{   
    private:
        uint32 _shaderID;

    public :
        void        initialize(const char* vertexRelativePath,const char* fragmentRelativePath);
        void        use(void);
        ~Shader();
    public :
        uint32      _programId;
        void        setMat4(const std::string &name, glm::mat4 mat4) const;
        void        setUint(const std::string &name, unsigned int index) const;
        void        setVec4(const std::string &name, glm::vec4 vec4) const;
        void        setVec3(const std::string &name, glm::vec3 vec3) const;
        void        setFloat(const std::string &name, float index) const;
        void        attachShader(const char* shaderRelativePath);
};
