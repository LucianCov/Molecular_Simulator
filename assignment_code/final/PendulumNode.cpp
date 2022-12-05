#include "PendulumNode.hpp"

#include "gloo/utils.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/InputManager.hpp"

#include "glm/gtx/string_cast.hpp"

namespace GLOO {
  PendulumNode::PendulumNode(ParticleState initial_state, const std::vector<float>& masses, IntegratorType type, float step_size)
 : SceneNode() {
    step_ = step_size;
    time_ = 0.0;
    state_ = initial_state;
    initial_state_ = initial_state;
    integrator_ = IntegratorFactory::CreateIntegrator<PendulumSystem, ParticleState>(type);

    sphere_mesh_ = PrimitiveFactory::CreateSphere(0.1f, 25, 25);
    shader_ = std::make_shared<PhongShader>();
    
    system_.AddParticle(masses[0]);
    for (size_t i = 1; i < state_.positions.size(); i++){
        //need to initialize the system by adding masses and springs
        system_.AddParticle(masses[i]);
        system_.AddSpring(50.0, 0.6, i-1, i);
    }
    system_.FixParticle(0);

    Initialize();
}

void PendulumNode::Update(double delta_time) {
    // want to integrate as many times as the step allows for this delta
    // std::cout << delta_time << std::endl;
    for (int _ = 0; _ < std::max(int(delta_time/step_),1); _++){
        state_ = integrator_->Integrate(system_, state_, time_, step_);
        time_ += step_;
    }
    for (size_t i = 0; i < particle_nodes_.size(); i++){
        particle_nodes_[i]->GetTransform().SetPosition(state_.positions[i]);
    }

    static bool prev_released = true;
    if (InputManager::GetInstance().IsKeyPressed('R')) {
        if (prev_released) {
            state_ = initial_state_;
        }
        prev_released = false;
    } else {
        prev_released = true;
    }
    
}

void PendulumNode::Initialize(){
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

    // std::cout << "Trying to get the pointer" << std::endl;
}
}