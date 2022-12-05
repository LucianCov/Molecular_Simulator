#include "ClothNode.hpp"

#include "gloo/utils.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/SimpleShader.hpp"
#include "gloo/InputManager.hpp"

#include "glm/gtx/string_cast.hpp"

namespace GLOO {
  ClothNode::ClothNode(ParticleState initial_state, const std::vector<float>& masses, int size, IntegratorType type, float step_size)
 : SceneNode() {
    step_ = step_size;
    time_ = 0.0;
    state_ = initial_state;
    initial_state_ = initial_state;
    size_ = size;
    integrator_ = IntegratorFactory::CreateIntegrator<PendulumSystem, ParticleState>(type);

    // sphere_mesh_ = PrimitiveFactory::CreateSphere(0.1f, 25, 25);
    spring_lines_ = std::make_shared<VertexObject>();
    shader_ = std::make_shared<SimpleShader>();

    SetUpSystem(masses);
    InitializeNode();
}

void ClothNode::Update(double delta_time) {
    // want to integrate as many times as the step allows for this delta
    // std::cout << delta_time << std::endl;
    for (int _ = 0; _ < std::max(int(delta_time/step_),1); _++){
        state_ = integrator_->Integrate(system_, state_, time_, step_);
        time_ += step_;
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
    auto positions = make_unique<PositionArray>(state_.positions);
    spring_lines_->UpdatePositions(std::move(positions));
}

void ClothNode::SetUpSystem(const std::vector<float>& masses){
    auto indices = make_unique<IndexArray>();
    auto positions = make_unique<PositionArray>(state_.positions);
    
    // system_.AddParticle(masses[0]);
    //need to initialize the system by adding masses and springs
    for (int row = 0; row < size_; row++){
        for (int col = 0; col < size_; col++){
            int cur_idx = IndexOf(row, col);
            system_.AddParticle(masses[cur_idx]);
            // fixing the top left and top right
            if (row == 0) {
                if (col == 0 || col == size_-1){
                    system_.FixParticle(cur_idx);
                }
            }
            // both of these conditions add structural springs
            if (row < size_ - 1) {
                system_.AddSpring(70.0, 0.6, cur_idx, IndexOf(row+1, col));
                indices->push_back(cur_idx);
                indices->push_back(IndexOf(row+1, col));
            }
            if (col < size_ - 1) {
                system_.AddSpring(70.0, 0.6, cur_idx, IndexOf(row, col+1));
                indices->push_back(cur_idx);
                indices->push_back(IndexOf(row, col+1));
            }
            if (row < size_ - 2) {
                system_.AddSpring(70.0, 1.2, cur_idx, IndexOf(row+2, col));
            }
            if (col < size_ - 2) {
                system_.AddSpring(70.0, 1.2, cur_idx, IndexOf(row, col+2));
            }
            if (row < size_ - 1 && col < size_ - 1) {
                system_.AddSpring(70.0, 0.848528, cur_idx, IndexOf(row+1, col+1));
            }
            if (row < size_ - 1 && col > 0) {
                system_.AddSpring(70.0, 0.848528, cur_idx, IndexOf(row+1, col-1));
            }
        }
    }
    spring_lines_->UpdatePositions(std::move(positions));
    spring_lines_->UpdateIndices(std::move(indices));
}

void ClothNode::InitializeNode(){
    auto line = make_unique<SceneNode>();

    line->CreateComponent<ShadingComponent>(shader_);
    auto rc = line->CreateComponent<RenderingComponent>(spring_lines_);
    rc.SetDrawMode(DrawMode::Lines);

    // auto material = std::make_shared<Material>(Material::GetDefault());
    glm::vec3 color(1.f,1.f,1.f);
    auto material = std::make_shared<Material>(color, color, color, 0);
    line->CreateComponent<MaterialComponent>(material);

    // line->GetTransform().SetPosition(state_.positions[i]);

    spring_node_ = line.get();
    AddChild(std::move(line));
}

int ClothNode::IndexOf(int row, int col){
    return row * size_ + col;
}

   // std::cout << "Trying to get the pointer" << std::endl;
}