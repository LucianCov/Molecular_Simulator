#ifndef EAM_SYSTEM_H_
#define EAM_SYSTEM_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"
#include "EAM.hpp"

namespace GLOO {
class EAMSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;
    float k_ = .05;

    void Set_Force_Model(float rho, float phi, float embedding) {
        force_model_ = EAM(rho, phi, embedding);
    }

    void AddParticle(float mass){
        masses_.emplace_back(mass);
        // fixed_.push_back(false);
    }

    void AddSpring(float spring_const, float rest_length, int point1, int point2){
        Spring new_spring = Spring();
        new_spring.point_1_idx = point1;
        new_spring.point_2_idx = point2;
        new_spring.rest_length = rest_length;
        new_spring.spring_constant = spring_const;
        springs_.push_back(new_spring);
    }

    void FixParticle(int index){
        fixed_[index] = true;
    }

    ParticleState ComputeTimeDerivative(const ParticleState& state, float time) const{
        ParticleState derivative = ParticleState();
        derivative.positions = state.velocities;        
        std::vector<glm::vec3> accelerations;
        for (int i = 0; i < state.positions.size(); i++) {
            // for each particle initialize its total force to 0
            glm::vec3 force = glm::vec3(0.f);
            force += k_ * state.velocities[i] * -1.f;
            for (int j = 0; j < state.positions.size(); j++) {
                if (i != j) {
                    // don't want to use the point on itself
                    force += glm::normalize(state.positions[j] - state.positions[i]) * -1.f * force_model_.CalcForce(state.positions[i], state.positions[j]);

                }
            }
            accelerations.push_back(force/masses_[i]);
        }
        derivative.velocities = accelerations;
        return derivative;
    }
 private:
    EAM force_model_ = EAM();
};
}  // namespace GLOO

#endif