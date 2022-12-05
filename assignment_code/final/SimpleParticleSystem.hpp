#ifndef SIMPLE_PARTICLE_SYSTEM_BASE_H_
#define SIMPLE_PARTICLE_SYSTEM_BASE_H_

#include "ParticleState.hpp"

#include <iostream>

namespace GLOO {
class SimpleParticleSystem : ParticleSystemBase {
 public:
//   ParticleSystemBase() {
//   }

    ParticleState ComputeTimeDerivative(const ParticleState& state,float time) const{
        ParticleState derivative = ParticleState();

        derivative.positions.emplace_back(glm::vec3(0.f));
        derivative.velocities.emplace_back(glm::vec3(0.f));

        derivative.positions[0].x = -1 * state.positions[0].y;
        derivative.positions[0].y = state.positions[0].x;
        derivative.positions[0].z = 0;

        return derivative;
    }
};
}  // namespace GLOO

#endif
