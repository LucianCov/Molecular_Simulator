#ifndef EAM_NODE_H_
#define EAM_NODE_H_

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorFactory.hpp"
#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"

#include "EAMSystem.hpp"

// #include <string>
// #include <vector>

namespace GLOO {
class EAMNode : public SceneNode {
 public:
  EAMNode(ParticleState initial_state, const std::vector<float>& masses, const std::vector<glm::vec3>& colors, IntegratorType type, float step_size);

  void Update(double delta_time) override;

 private:

    void Initialize();

    std::vector<SceneNode*> particle_nodes_;
    std::shared_ptr<VertexObject> sphere_mesh_;  
    std::shared_ptr<VertexObject> segments_;
    std::shared_ptr<ShaderProgram> shader_;

    ParticleState reset_;
    ParticleState state_;
    float step_;
    float time_;
    std::unique_ptr<IntegratorBase<EAMSystem, ParticleState>> integrator_;
    IntegratorType integrator_type_;
    EAMSystem system_;
    std::vector<glm::vec3> colors_;

};
}  // namespace GLOO

#endif
