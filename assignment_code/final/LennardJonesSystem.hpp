#ifndef LENNARD_JONES_SYSTEM_H_
#define LENNARD_JONES_SYSTEM_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"

namespace GLOO {
class LennardJonesSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;


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

        // derivative = CalcForce(state);

        return derivative;
    }
};
}  // namespace GLOO

#endif
