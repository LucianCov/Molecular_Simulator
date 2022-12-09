#include "EAM.hpp"

namespace GLOO {
    EAM::EAM() {
        rho_ = 0.2;
        phi_ = 0.9;
        embedding_ = 0.5;
    }

    EAM::EAM(float rho, float phi, float embedding) :
     rho_(rho), phi_(phi), embedding_(embedding) {
    }

    float EAM::CalcForce(glm::vec3 pos1, glm::vec3 pos2) const {
        float r = glm::distance(pos1, pos2);
        return embedding_*rho_*r + (0.5f)*phi_*r;
    }
}