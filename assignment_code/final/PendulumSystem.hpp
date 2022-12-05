#ifndef PENDULUM_SYSTEM_BASE_H_
#define PENDULUM_SYSTEM_BASE_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"

namespace GLOO {
class PendulumSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;
    const float gravity_ = 9.8;
    const float k_ = .85;
    // PendulumSystem() {
         
    // }

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

        // derivative.positions.emplace_back(glm::vec3(0.f));
        // derivative.velocities.emplace_back(glm::vec3(0.f));

        // grav force is the mass vector multiplied by the gravity scalar
        // drag force is the velocities vector multiplied by the k_ scalar
        // spring force is going to need to be calculated by iterating over all points, taking distance and pos between,
        // and adding forces to a force vector

        // std::cout<< "transferring velocities to deriv" << std::endl;

        derivative.positions = state.velocities;

        std::vector<glm::vec3> forces;

        for (size_t i = 0; i < state.velocities.size(); i++){
            // calc drag force and add in gravity to the z component
            forces.push_back((k_ * state.velocities[i] * -1.f) - glm::vec3(0.f,masses_[i] * gravity_, 0.f));
        }
        // std::cout << "forces initialized" << std::endl;

        for (int i = 0; i < springs_.size(); i++){
            // adding spring forces to each particle by iterating through springs
            // f = -k*(mag(d)-r) * d/mag(d)
            glm::vec3 pos(state.positions[springs_[i].point_1_idx] - state.positions[springs_[i].point_2_idx]);
            float distance = glm::length(pos);

            // add spring force to one
            forces[springs_[i].point_1_idx] += -1.f * springs_[i].spring_constant * (distance - springs_[i].rest_length) * pos / distance;
            // add negative of spring force to the other
            forces[springs_[i].point_2_idx] += springs_[i].spring_constant * (distance - springs_[i].rest_length) * pos / distance;
        }

        // std::cout << "spring forces added" << std::endl;
        // at this point, forces is populated with the sum of all forces for each point
        // just need to divide by mass and we get the accelleration

        for (int i = 0; i < forces.size(); i++){
            if (fixed_[i]) {
                derivative.velocities.push_back(glm::vec3(0.f));
            }
            else{
                derivative.velocities.push_back(forces[i] / masses_[i]);
            }
        }
        // std::cout << "acceleration calculated" << std::endl;

        return derivative;
    }
};
}  // namespace GLOO

#endif
