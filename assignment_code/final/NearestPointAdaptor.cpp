// #include "NearestPointAdaptor.hpp"
// #include <iostream>

// // #include <vector>

// namespace GLOO {
// NNPoints::NNPoints() : m_verts_() { }
// NNPoints::NNPoints(const std::vector<glm::vec3>& verts) : m_verts_(verts) { }
// inline void NNPoints::addVertex(const glm::vec3& v) { m_verts_.push_back(v); }
// const glm::vec3& NNPoints::operator[](unsigned int i) const { return m_verts_[i]; }
// unsigned int NNPoints::size() const { return m_verts_.size(); }


// PointsAdaptor::PointsAdaptor(const NNPoints& pts) : m_pts_(pts) { }

// inline unsigned int PointsAdaptor::kdtree_get_point_count() const
// { return m_pts_.size(); }

// inline float PointsAdaptor::kdtree_distance(const float* p1, const unsigned int index_2,
//         unsigned int size) const
// {
//     return glm::length(glm::vec3(p1[0], p1[1], p1[2]) -
//             m_pts_[index_2]);
// }

// inline float PointsAdaptor::kdtree_get_pt(const unsigned int i, int dim) const
// {
//     if (dim == 0) return m_pts_[i].x;
//     else if (dim == 1) return m_pts_[i].y;
//     else return m_pts_[i].z;
// }

// // void kdtree_test()
// // {
// // 	float test_pt[3] = {1.5, 1, 0}; // [1.5, 1, 0]
// // 	Points f;
// // 	f.addVertex(glm::vec3(0, 1, 0));
// // 	f.addVertex(glm::vec3(-1, 0, 0));
// // 	f.addVertex(glm::vec3(1, 0, 0));

// // 	PointsAdaptor f_adaptor(f);

// // 	typedef nanoflann::KDTreeSingleIndexAdaptor<
// // 		nanoflann::L2_Simple_Adaptor<float, PointsAdaptor>,
// // 		PointsAdaptor, 3> Points_KD_Tree_t;
// // 	Points_KD_Tree_t index(2, f_adaptor,
// // 			nanoflann::KDTreeSingleIndexAdaptorParams(10));
// // 	index.buildIndex();

// // 	// Perform Knn Search
// // 	const unsigned int num_results = 1;
// // 	size_t ret_index;
// // 	float out_dist_sqr;
// // 	nanoflann::KNNResultSet<float> resultSet(num_results);
// // 	resultSet.init(&ret_index, &out_dist_sqr);
// // 	index.findNeighbors(resultSet, &test_pt[0],
// // 			nanoflann::SearchParams(10));

// // 	std::cout << "KnnSearch(nn="<<num_results<<"): \n";
// // 	std::cout << "ret_index=" << ret_index <<
// // 		" out_dist_sqr: " << out_dist_sqr << '\n';

// // 	std::cout << "Nearest Vertex: "
// // 	       	<< f[ret_index].x << ", "
// // 		<< f[ret_index].y << ", "
// // 		<< f[ret_index].z << '\n';

// // }
// }