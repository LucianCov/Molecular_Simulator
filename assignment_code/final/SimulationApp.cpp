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

  ParticleState initial_state_molecular = ParticleState();
  std::vector<float> molecule_masses;
  std::vector<glm::vec3> colors;
  // initial_state_molecular.positions.push_back(glm::vec3(1.f, 0.f, 0.f));
  // initial_state_molecular.positions.push_back(glm::vec3(1.2f, 0.f, 0.f));
  // initial_state_molecular.velocities.push_back(glm::vec3(0.f));
  // initial_state_molecular.velocities.push_back(glm::vec3(0.f));
  // molecule_masses.push_back(0.5);
  // molecule_masses.push_back(0.5);
  for (float i = 1.0; i < 1.2; i += 0.1){
    for (float j = 0.9; j < 1.2; j += 0.1) {
      for (float k = 0.9; k < 1.2; k += 0.1) {
        initial_state_molecular.positions.push_back(glm::vec3(i, j, k));
        initial_state_molecular.velocities.push_back(glm::vec3(0.f));
        molecule_masses.push_back(0.5);
        colors.push_back(glm::vec3(0.9f, 0.2f, 0.1f));
      }
    }
  }

  for (float i = -2.2; i < -2.0; i += 0.1){
    for (float j = 1.0; j < 1.2; j += 0.1) {
      for (float k = 1.0; k < 1.2; k += 0.1) {
        initial_state_molecular.positions.push_back(glm::vec3(i, j-0.05, k-0.05));
        initial_state_molecular.velocities.push_back(glm::vec3(10.5f, 0.f, 0.f));
        molecule_masses.push_back(0.5);
        colors.push_back(glm::vec3(0.1f, 0.4f, 0.9f));
      }
    }
  }

  // initial_state_molecular.positions.push_back(glm::vec3(-1.f, 1.05f, 1.05f));
  // initial_state_molecular.velocities.push_back(glm::vec3(2.f, 0.f, 0.f));
  // molecule_masses.push_back(0.5);

  auto m_sim = make_unique<LennardJonesNode>(initial_state_molecular, molecule_masses, colors, integrator_type_, integration_step_);
  m_sim_node_ = m_sim.get();
  root.AddChild(std::move(m_sim));
}
}  // namespace GLOO
