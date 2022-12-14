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
  LennardJonesNode::LennardJonesNode(ParticleState initial_state, const std::vector<float>& masses, const std::vector<glm::vec3>& colors, IntegratorType type, float step_size)
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
    integrator_type_ = type;
    colors_ = colors;

    system_.Set_Force_Model(0.1, 0.1);
    
    for (size_t i = 0; i < state_.positions.size(); i++){
        //need to initialize the system by adding masses and springs
        system_.AddParticle(masses[i]);
    }

    Initialize();
}

void LennardJonesNode::Update(double delta_time) {
    if (InputManager::GetInstance().IsKeyPressed('R')) {
        state_ = reset_; /* reset state to initial if R is pressed */
    }

    auto prev_state = state_;
    for (int _ = 0; _ < std::max(int(delta_time/step_),1); _++){
        state_ = integrator_->Integrate(system_, state_, time_, step_);
        // if (integrator_type_ == IntegratorType::Euler) {
        //     for (int i = 0; i < state_.positions.size(); i++) {
        //         state_.positions[i] = state_.positions[i] - prev_state.positions[i];
        //     }
        // prev_state = state_;
        // }
        time_ += step_;
        // prev_state = state_;
    }
    for (size_t i = 0; i < particle_nodes_.size(); i++){
        particle_nodes_[i]->GetTransform().SetPosition(state_.positions[i]);
    }
    }

void LennardJonesNode::Initialize(){
    for (size_t i = 0; i < state_.positions.size(); i++){
        auto particle = make_unique<SceneNode>();

        particle->CreateComponent<ShadingComponent>(shader_);
        auto rc = particle->CreateComponent<RenderingComponent>(sphere_mesh_);
        rc.SetDrawMode(DrawMode::Triangles);

        auto material = std::make_shared<Material>(Material::GetDefault());
        material->SetDiffuseColor(colors_[i]);
        auto mat_comp = particle->CreateComponent<MaterialComponent>(material);

        particle->GetTransform().SetPosition(state_.positions[i]);

        particle_nodes_.push_back(particle.get());
        AddChild(std::move(particle));
    }
}
}