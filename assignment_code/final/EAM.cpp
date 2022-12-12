#include "EAM.hpp"

namespace GLOO {
    EAM::EAM() {
        // A_ = 641.3; // units are eV
        // B_ = 12.22; // units are eV
        // n_ = 0.6626; // units are eV

        A_ = 1; // units are eV
        B_ = 1; // units are eV
        n_ = 0.5; // units are eV
    }

    EAM::EAM(float A, float B, float n) :
     A_(A), B_(B), n_(n) {
    }

    float EAM::CalcPotential(glm::vec3 pos1, glm::vec3 pos2) const {
        float r = glm::distance(pos1, pos2);
        return (-1.f)*(-r)*(A_)*exp(-r); // should be -alpha*A_*exp(-alpha*r)
    }

    float EAM::CalcEmbedding(glm::vec3 pos1, glm::vec3 pos2) const {
        float r = glm::distance(pos1, pos2);
        // will approximate force for now
        // b = 1.364 A^-1
        return (-1.f)*(-B_)*(-1.364)*(n_)*(pow(exp(-1.364*r), n_-1)); // should be derivative of sum, not sum of derivatives, which is current implementation
    }
}

    