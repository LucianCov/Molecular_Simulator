#ifndef SIMPLE_PARTICLE_NODE_H_
#define SIMPLE_PARTICLE_NODE_H_

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorFactory.hpp"
#include "SimpleParticleSystem.hpp"
#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"

// #include <string>
// #include <vector>

namespace GLOO {
class SimpleParticleNode : public SceneNode {
 public:
  SimpleParticleNode(ParticleState initial_state, IntegratorType type, float step_size);

  void Update(double delta_time) override;

 private:

    void InitializeParticle();

    SceneNode* particle_node_;

    std::shared_ptr<VertexObject> sphere_mesh_;
    std::shared_ptr<ShaderProgram> shader_;

    float step_;
    float time_;
    ParticleState state_;
    std::unique_ptr<IntegratorBase<SimpleParticleSystem, ParticleState>> integrator_;
    SimpleParticleSystem system_ = SimpleParticleSystem();

};
}  // namespace GLOO

#endif
