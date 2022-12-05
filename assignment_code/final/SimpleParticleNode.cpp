#include "SimpleParticleNode.hpp"

#include "gloo/utils.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/PhongShader.hpp"

#include "glm/gtx/string_cast.hpp"

namespace GLOO {
  SimpleParticleNode::SimpleParticleNode(ParticleState initial_state, IntegratorType type, float step_size)
 : SceneNode() {
    step_ = step_size;
    time_ = 0.0;
    state_ = initial_state;
    integrator_ = IntegratorFactory::CreateIntegrator<SimpleParticleSystem, ParticleState>(type);

    sphere_mesh_ = PrimitiveFactory::CreateSphere(0.1f, 25, 25);
    shader_ = std::make_shared<PhongShader>();

    InitializeParticle();
}

void SimpleParticleNode::Update(double delta_time) {
    // want to integrate as many times as the step allows for this delta
    // std::cout << delta_time << std::endl;
    for (int _ = 0; _ < std::max(int(delta_time/step_),1); _++){
        // std::cout << "Integrating" << std::endl;
        state_ = integrator_->Integrate(system_, state_, time_, step_);
        // std::cout << "stepping" << std::endl;
        time_ += step_;
    }
    // std::cout << "updating position" << std::endl;
    // std::cout << glm::to_string(state_.positions[0]) << std::endl;
    particle_node_->GetTransform().SetPosition(state_.positions[0]);
    // std::cout << "successfully set position" << std::endl;
}

void SimpleParticleNode::InitializeParticle(){
    auto particle = make_unique<SceneNode>();

    particle->CreateComponent<ShadingComponent>(shader_);
    auto rc = particle->CreateComponent<RenderingComponent>(sphere_mesh_);
    rc.SetDrawMode(DrawMode::Triangles);

    auto material = std::make_shared<Material>(Material::GetDefault());
    auto mat_comp = particle->CreateComponent<MaterialComponent>(material);

    particle->GetTransform().SetPosition(state_.positions[0]);

    // std::cout << "Trying to get the pointer" << std::endl;
    particle_node_ = particle.get();

    AddChild(std::move(particle));
}
}