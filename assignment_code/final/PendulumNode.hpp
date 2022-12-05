#ifndef PENDULUM_NODE_H_
#define PENDULUM_NODE_H_

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorFactory.hpp"
#include "PendulumSystem.hpp"
#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"

// #include <string>
// #include <vector>

namespace GLOO {
class PendulumNode : public SceneNode {
 public:
  PendulumNode(ParticleState initial_state, const std::vector<float>& masses, IntegratorType type, float step_size);

  void Update(double delta_time) override;

 private:

    void Initialize();

    std::vector<SceneNode*> particle_nodes_;

    std::shared_ptr<VertexObject> sphere_mesh_;
    std::shared_ptr<ShaderProgram> shader_;

    float step_;
    float time_;
    ParticleState state_;
    ParticleState initial_state_;
    std::unique_ptr<IntegratorBase<PendulumSystem, ParticleState>> integrator_;
    PendulumSystem system_ = PendulumSystem();

};
}  // namespace GLOO

#endif
