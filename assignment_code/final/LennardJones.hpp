#ifndef LENNARD_JONES_H_
#define LENNARD_JONES_H_

#include <glm/glm.hpp>

namespace GLOO {
class LennardJones {
  public:
    LennardJones(float epsilon, float r_min);
    float CalcForce(glm::vec3 pos1, glm::vec3 pos2) const;
  private:
    float epsilon_;
    float r_min_;
};
}  // namespace GLOO

#endif