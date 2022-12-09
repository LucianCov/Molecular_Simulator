#ifndef LENNARD_JONES_SYSTEM_H_
#define LENNARD_JONES_SYSTEM_H_

#include "ParticleSystemBase.hpp"
#include "Spring.hpp"
#include "LennardJones.hpp"
#include "NearestPointAdaptor.hpp"
#include "/usr/local/include/nanoflann.hpp"
#include "glm/gtx/string_cast.hpp"
#include <cmath>

namespace GLOO {
class LennardJonesSystem : ParticleSystemBase {
 public:
    std::vector<float> masses_;
    std::vector<Spring> springs_;
    std::vector<bool> fixed_;
    float k_ = .05;

    void Set_Force_Model(float epsilon, float r_min) {
        force_model_ = LennardJones(epsilon, r_min);
    }

    void AddParticle(float mass){
        masses_.emplace_back(mass);
        // fixed_.push_back(false);
    }

    void AddSpring(float spring_const, float rest_length, int point1, int point2){
        Spring new_spring = Spring();
        new_spring.point_1_idx = point1;
        new_spring.point_2_idx = point2;
        new_spring.rest_length = rest_length;
        new_spring.spring_constant = spring_const;
        springs_.push_back(new_spring);
    }

    void FixParticle(int index){
        fixed_[index] = true;
    }

    ParticleState ComputeTimeDerivative(const ParticleState& state, float time) const{
        ParticleState derivative = ParticleState();
        derivative.positions = state.velocities;        
        std::vector<glm::vec3> accelerations;

        NNPoints finder_points(state.positions);
        // NearestPointFinder finder(finder_points);
        PointsAdaptor adaptor(finder_points);

        typedef nanoflann::KDTreeSingleIndexAdaptor<
		nanoflann::L2_Simple_Adaptor<float, PointsAdaptor>,
		PointsAdaptor, 3> Points_KD_Tree_t;

	    Points_KD_Tree_t index(2, adaptor,
			nanoflann::KDTreeSingleIndexAdaptorParams(10));
	    index.buildIndex();

        const unsigned int num_results = 1;
        size_t ret_index;
        float out_dist_sqr;
        nanoflann::KNNResultSet<float> resultSet(num_results);
        resultSet.init(&ret_index, &out_dist_sqr);

        std::cout<< "asdf" << std::endl;

        for (int i = 0; i < state.positions.size(); i++) {
            // for each particle initialize its total force to 0
            float point_pos[3] = {state.positions[i].x, state.positions[i].y, state.positions[i].z};

            glm::vec3 force = glm::vec3(0.f);
            force += k_ * state.velocities[i] * -1.f;
            // for (int j = 0; j < state.positions.size(); j++) {
            //     if (i != j) {
            //         // don't want to use the point on itself
            //         force += glm::normalize(state.positions[j] - state.positions[i]) * -1.f * force_model_.CalcForce(state.positions[i], state.positions[j]);

            //     }
            // }
            // std::cout << "about to call the finder" << std::endl;
            index.findNeighbors(resultSet, &point_pos[0],
			                    nanoflann::SearchParams(10));
            // const unsigned int num_nbrs = 1;
            // std::cout << "found" << std::endl;

            std::cout << glm::to_string(finder_points[ret_index]) << std::endl;

            if (isnan(finder_points[ret_index].x) || isnan(finder_points[ret_index].y) || isnan(finder_points[ret_index].z)) {
                std::cout << "caught a nan val" << std::endl;
            }
            else {
                force += glm::normalize(finder_points[ret_index] - state.positions[i]) * -1.f
                        * force_model_.CalcForce(state.positions[i], finder_points[ret_index]);
            }
            // for (glm::vec3& point : resultSet) {
            //     // only going to iterate over 6 points! yay
            //     force += glm::normalize(point - state.positions[i]) * -1.f
            //              * force_model_.CalcForce(state.positions[i], point);
            // }
            accelerations.push_back(force/masses_[i]);
        }
        std::cout << "about to return" << std::endl;
        derivative.velocities = accelerations;
        return derivative;
    }
 private:
    LennardJones force_model_ = LennardJones();
};
}  // namespace GLOO

#endif
