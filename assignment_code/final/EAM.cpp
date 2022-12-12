#include "EAM.hpp"

namespace GLOO {
    EAM::EAM() {
        // A_ = 0.001; // units are eV
        // B_ = 0.001; // units are eV
        // n_ = 0.6626; // units are eV

        A_ = 0.6413; // units are eV
        B_ = 0.01222; // units are eV
        n_ = 0.6626; // units are eV
    }

    EAM::EAM(float A, float B, float n) :
     A_(A), B_(B), n_(n) {
    }

    float EAM::CalcPotential(glm::vec3 pos1, glm::vec3 pos2) const {
        float r = glm::distance(pos1, pos2);
        return (-1.f)*(-1.81)*(A_)*exp(-1.81*r); 
    }

    float EAM::CalcEmbedding(glm::vec3 pos1, glm::vec3 pos2) const {
        float r = glm::distance(pos1, pos2);
        // will approximate force for now
        // b = 1.364 A^-1
        return (-0.5)*(-B_)*(-0.001364)*(n_)*(pow(exp(-0.001364*r), n_-1)); // should be derivative of sum, not sum of derivatives, which is current implementation
    }
}

    