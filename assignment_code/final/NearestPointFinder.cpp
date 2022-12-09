#include "NearestPointFinder.hpp"
#include "gloo/utils.hpp"

namespace GLOO {
NearestPointFinder::NearestPointFinder(const NNPoints& points): index_(2, 
                                                                PointsAdaptor(points),
                                                                nanoflann::KDTreeSingleIndexAdaptorParams(10)) {
	index_.buildIndex();
}

std::vector<glm::vec3> NearestPointFinder::Find(const float p[3], const unsigned int num_nbrs) {
    std::cout << "starting find func" << std::endl;
    size_t ret_index;
    float out_dist_sqr;
    nanoflann::KNNResultSet<float> resultSet(num_nbrs);
    std::cout << "result set created" << std::endl;
    resultSet.init(&ret_index, &out_dist_sqr);
    std::cout << "res set init" << std::endl;
    index_.findNeighbors(resultSet, &p[0], nanoflann::SearchParams(10));
    std::cout << "neighbors added to res set" << std::endl;
    std::vector<glm::vec3> output;
    NNPoints point_set = *points_ptr_;
    for (size_t i = 0; i < resultSet.size(); ++i) {
        std::cout << "attempting to access the points" << std::endl;
        output.push_back(point_set[i]);
    }
    return output;
    // float test_pt[3] = {1.5, 1, 0}; // [1.5, 1, 0]
	// NNPoints f;
	// f.addVertex(glm::vec3(0, 1, 0));
	// f.addVertex(glm::vec3(-1, 0, 0));
	// f.addVertex(glm::vec3(1, 0, 0));

	// PointsAdaptor f_adaptor(f);

	// // typedef nanoflann::KDTreeSingleIndexAdaptor<
	// // 	nanoflann::L2_Simple_Adaptor<float, PointsAdaptor>,
	// // 	PointsAdaptor, 3> Points_KD_Tree_t;
	// // Points_KD_Tree_t index(2, f_adaptor,
	// // 		nanoflann::KDTreeSingleIndexAdaptorParams(10));
	// // index.buildIndex();
    // index_ = Points_KD_Tree_t(2, f_adaptor, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    // index_.buildIndex();

	// // Perform Knn Search
	// // const unsigned int num_results = 1;
	// size_t ret_index;
	// float out_dist_sqr;
	// nanoflann::KNNResultSet<float> resultSet(num_nbrs);
	// resultSet.init(&ret_index, &out_dist_sqr);
	// index_.findNeighbors(resultSet, &test_pt[0],
	// 		nanoflann::SearchParams(10));
    // // std::cout << "didd it" << std::endl;

    // return std::vector<glm::vec3>();
}

}