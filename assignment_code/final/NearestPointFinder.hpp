#ifndef POINT_FINDER_H_
#define POINT_FINDER_H_

#include "NearestPointAdaptor.hpp"
#include "/usr/local/include/nanoflann.hpp"

namespace GLOO {
class NearestPointFinder {
 public:
    NearestPointFinder(const NNPoints& points);
    std::vector<glm::vec3> Find(const float* p, const unsigned int num_nbrs);
 private:
    typedef nanoflann::KDTreeSingleIndexAdaptor<
	    nanoflann::L2_Simple_Adaptor<float, PointsAdaptor>,
	    PointsAdaptor, 3> Points_KD_Tree_t;
	Points_KD_Tree_t index_;
    std::unique_ptr<NNPoints> points_ptr_;
};
}

#endif