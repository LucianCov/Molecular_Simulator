#include "LennardJonesNode.hpp"

#include "gloo/utils.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/InputManager.hpp"

#include "glm/gtx/string_cast.hpp"

namespace GLOO {
  LennardJonesNode::LennardJonesNode(ParticleState initial_state, const std::vector<float>& masses, IntegratorType type, float step_size)
 : SceneNode() {
    sphere_mesh_ = PrimitiveFactory::CreateSphere(0.05f, 25, 25);
    shader_ = std::make_shared<PhongShader>();
    segments_ = std::make_shared<VertexObject>();
    reset_ = initial_state;
    state_ = initial_state;
    step_ = step_size;
    time_ = 0.0;
    integrator_ = IntegratorFactory::CreateIntegrator<LennardJonesSystem, ParticleState>(type);
    system_ = LennardJonesSystem();
    
    system_.AddParticle(masses[0]);
    for (size_t i = 1; i < state_.positions.size(); i++){
        //need to initialize the system by adding masses and springs
        system_.AddParticle(masses[i]);
        system_.AddSpring(50.0, 0.6, i-1, i);
    }
    system_.FixParticle(0);

    Initialize();
}

void LennardJonesNode::Update(double delta_time) {
    if (InputManager::GetInstance().IsKeyPressed('R')) {
        state_ = reset_; /* reset state to initial if R is pressed */
    }

    for (int _ = 0; _ < std::max(int(delta_time/step_),1); _++){
        state_ = integrator_->Integrate(system_, state_, time_, step_);
        time_ += step_;
    }
    for (size_t i = 0; i < particle_nodes_.size(); i++){
        particle_nodes_[i]->GetTransform().SetPosition(state_.positions[i]);
    }}

void LennardJonesNode::Initialize(){
    for (size_t i = 0; i < state_.positions.size(); i++){
        auto particle = make_unique<SceneNode>();

        particle->CreateComponent<ShadingComponent>(shader_);
        auto rc = particle->CreateComponent<RenderingComponent>(sphere_mesh_);
        rc.SetDrawMode(DrawMode::Triangles);

        auto material = std::make_shared<Material>(Material::GetDefault());
        auto mat_comp = particle->CreateComponent<MaterialComponent>(material);

        particle->GetTransform().SetPosition(state_.positions[i]);

        particle_nodes_.push_back(particle.get());
        AddChild(std::move(particle));
    }
}
}