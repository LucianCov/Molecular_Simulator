#ifndef LENNARD_JONES_SYSTEM_H_
#define LENNARD_JONES_SYSTEM_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"
#include "LennardJones.hpp"

namespace GLOO {
class LennardJonesSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;

    void Set_Force_Model(float epsilon, float r_min) {
        force_model_ = LennardJones(epsilon, r_min);
    }

    void AddParticle(float mass){
        masses_.emplace_back(mass);
        fixed_.push_back(false);
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
        derivative.velocities = state.velocities;

        // force can be retrieved with force_model_.CalcForce(glm::vec3 pos1, glm::vec3 pos2)

        for (int i = 0; i < state.positions.size(); i++) {
            glm::vec3 force;
            for (int j = 0; j < state.positions.size(); j++) {
                force += glm::normalize(state.positions[j] - state.positions[i]) * force_model_.CalcForce(state.positions[i], state.positions[j]);
            }
            derivative.velocities[i] = force/masses_[i];
        }
        return derivative;
    }
 private:
    LennardJones force_model_ = LennardJones();
};
}  // namespace GLOO

#endif
