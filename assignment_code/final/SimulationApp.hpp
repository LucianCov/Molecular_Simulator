#ifndef SIMULATION_APP_H_
#define SIMULATION_APP_H_

#include "gloo/Application.hpp"

#include "IntegratorType.hpp"
#include "IntegratorBase.hpp"
#include "SimpleParticleNode.hpp"
#include "PendulumNode.hpp"
#include "ClothNode.hpp"

namespace GLOO {
class SimulationApp : public Application {
 public:
  SimulationApp(const std::string& app_name,
                glm::ivec2 window_size,
                IntegratorType integrator_type,
                float integration_step);
  void SetupScene() override;

 private:
  IntegratorType integrator_type_;
  float integration_step_;
  SimpleParticleNode* simple_particle_node_;
  PendulumNode* pendulum_node_;
  ClothNode* cloth_node_;
};
}  // namespace GLOO

#endif
