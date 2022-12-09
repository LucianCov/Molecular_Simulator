#ifndef EAM_H_
#define EAM_H_

#include <glm/glm.hpp>

namespace GLOO {
class EAM {
  public:
    EAM();
    EAM(float rho, float phi, float embedding);
    float CalcForce(glm::vec3 pos1, glm::vec3 pos2) const;
  private:
    float rho_;
    float phi_;
    float embedding_;
};
}  // namespace GLOO

#endif