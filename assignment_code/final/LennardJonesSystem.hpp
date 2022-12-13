#ifndef LENNARD_JONES_SYSTEM_H_
#define LENNARD_JONES_SYSTEM_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"
#include "LennardJones.hpp"
#include <fstream>

namespace GLOO {
class LennardJonesSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;
    float k_ = .05;

    void Set_Force_Model(float epsilon, float r_min) {
        force_model_ = LennardJones(epsilon, r_min);
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
        float energy = 0;        
        std::vector<glm::vec3> accelerations;
        for (int i = 0; i < state.positions.size(); i++) {
            // for each particle initialize its total force to 0
            glm::vec3 force = glm::vec3(0.f);
            force += k_ * state.velocities[i] * -1.f;
            for (int j = 0; j < state.positions.size(); j++) {
                if (i != j) {
                    // don't want to use the point on itself
                    force += glm::normalize(state.positions[j] - state.positions[i]) * -1.f * force_model_.CalcForce(state.positions[i], state.positions[j]);
                    energy += force_model_.CalcEnergy(state.positions[i], state.positions[j]);
                }
            }
            energy += (0.5)*(0.5)*(pow(glm::length(state.velocities[i]), 2));
            accelerations.push_back(force/masses_[i]);
        }
        // std::cout << energy << std::endl;
        std::ofstream File("energy_1.csv", std::ios::app);
        File << energy << "\n";
        File.close();
        derivative.velocities = accelerations;
        return derivative;
    }
 private:
    LennardJones force_model_ = LennardJones();
};
}  // namespace GLOO

#endif
