#include "SimulationApp.hpp"

#include "glm/gtx/string_cast.hpp"

#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/CameraComponent.hpp"
#include "gloo/components/LightComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/MeshLoader.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/lights/AmbientLight.hpp"
#include "gloo/cameras/ArcBallCameraNode.hpp"
#include "gloo/debug/AxisNode.hpp"


namespace GLOO {
SimulationApp::SimulationApp(const std::string& app_name,
                             glm::ivec2 window_size,
                             IntegratorType integrator_type,
                             float integration_step)
    : Application(app_name, window_size),
      integrator_type_(integrator_type),
      integration_step_(integration_step) {
  // TODO: remove the following two lines and use integrator type and step to
  // create integrators; the lines below exist only to suppress compiler
  // warnings.
  // UNUSED(integrator_type_);
  // UNUSED(integration_step_);

}

void SimulationApp::SetupScene() {
  SceneNode& root = scene_->GetRootNode();

  auto camera_node = make_unique<ArcBallCameraNode>(45.f, 0.75f, 5.0f);
  scene_->ActivateCamera(camera_node->GetComponentPtr<CameraComponent>());
  root.AddChild(std::move(camera_node));

  root.AddChild(make_unique<AxisNode>('A'));

  auto ambient_light = std::make_shared<AmbientLight>();
  ambient_light->SetAmbientColor(glm::vec3(0.2f));
  root.CreateComponent<LightComponent>(ambient_light);

  auto point_light = std::make_shared<PointLight>();
  point_light->SetDiffuseColor(glm::vec3(0.8f, 0.8f, 0.8f));
  point_light->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
  point_light->SetAttenuation(glm::vec3(1.0f, 0.09f, 0.032f));
  auto point_light_node = make_unique<SceneNode>();
  point_light_node->CreateComponent<LightComponent>(point_light);
  point_light_node->GetTransform().SetPosition(glm::vec3(0.0f, 2.0f, 4.f));
  root.AddChild(std::move(point_light_node));

  ParticleState initial_state_simple = ParticleState();
  initial_state_simple.positions.emplace_back(glm::vec3(1.f,0.f,0.f));
  initial_state_simple.velocities.emplace_back(glm::vec3(0.f));
  auto simple_system = make_unique<SimpleParticleNode>(initial_state_simple, integrator_type_, integration_step_);
  simple_particle_node_ = simple_system.get();
  root.AddChild(std::move(simple_system));

  ParticleState initial_state_pendulum = ParticleState();
  initial_state_pendulum.positions.emplace_back(glm::vec3(1.5f, 1.f, 0.f));
  initial_state_pendulum.positions.emplace_back(glm::vec3(2.f, 1.f, 0.f));
  initial_state_pendulum.positions.emplace_back(glm::vec3(2.5f, 1.f, 0.f));
  initial_state_pendulum.positions.emplace_back(glm::vec3(3.f, 1.f, 0.f));
  initial_state_pendulum.velocities.emplace_back(glm::vec3(0.f));
  initial_state_pendulum.velocities.emplace_back(glm::vec3(0.f));
  initial_state_pendulum.velocities.emplace_back(glm::vec3(0.f));
  initial_state_pendulum.velocities.emplace_back(glm::vec3(0.f));

  std::vector<float> masses;
  masses.push_back(1.0);
  masses.push_back(1.0);
  masses.push_back(1.0);
  masses.push_back(1.0);
  auto pendulum_system = make_unique<PendulumNode>(initial_state_pendulum, masses, integrator_type_, integration_step_);
  pendulum_node_ = pendulum_system.get();
  root.AddChild(std::move(pendulum_system));

  ParticleState initial_state_cloth = ParticleState();
  std::vector<float> cloth_masses;
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      initial_state_cloth.positions.push_back(glm::vec3(1 - j * .6f, 1 - i * .6f, 0.f));
      initial_state_cloth.velocities.push_back(glm::vec3(0.f));
      cloth_masses.push_back(0.5);
    }
  }

  auto cloth_system = make_unique<ClothNode>(initial_state_cloth, cloth_masses, 8, integrator_type_, integration_step_);
  cloth_node_ = cloth_system.get();
  root.AddChild(std::move(cloth_system));
}
}  // namespace GLOO
