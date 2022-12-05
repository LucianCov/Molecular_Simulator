#ifndef CLOTH_NODE_H_
#define CLOTH_NODE_H_

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorFactory.hpp"
#include "PendulumSystem.hpp"
#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"


namespace GLOO {
class ClothNode : public SceneNode {
 public:
  ClothNode(ParticleState initial_state, const std::vector<float>& masses, int size, IntegratorType type, float step_size);

  void Update(double delta_time) override;

 private:
    void SetUpSystem(const std::vector<float>& masses);
    void InitializeNode();

    int IndexOf(int row, int col);

    SceneNode* spring_node_;
    std::shared_ptr<VertexObject> spring_lines_;

    std::shared_ptr<ShaderProgram> shader_;

    float step_;
    float time_;
    int size_;
    ParticleState state_;
    ParticleState initial_state_;
    std::unique_ptr<IntegratorBase<PendulumSystem, ParticleState>> integrator_;
    PendulumSystem system_ = PendulumSystem();

};
}  // namespace GLOO

#endif
