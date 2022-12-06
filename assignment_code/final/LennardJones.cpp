#include "LennardJones.hpp"

namespace GLOO {
    LennardJones::LennardJones() {
        epsilon_ = 0.2;
        r_min_ = 0.9;
    }

    LennardJones::LennardJones(float epsilon, float r_min) :
     epsilon_(epsilon), r_min_(r_min) {
    }

    float LennardJones::CalcForce(glm::vec3 pos1, glm::vec3 pos2) const {
        // using the negated derivative of the lennard jones potential equation to get force
        float r = glm::distance(pos1, pos2);
        // NOTE: Values can get really small at higher distances, might make sense to truncate
        return 12 * epsilon_ * r_min_/(r*r) * (pow(r_min_/r, 11) - pow(r_min_/r, 5));
    }
}