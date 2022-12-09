#ifndef POINT_ADAPTOR_H_
#define POINT_ADAPTOR_H_

#include <vector>
#include <iostream>
#include "glm/glm.hpp"

namespace GLOO {
// class NNPoints {
//  public:
// 	NNPoints();
// 	NNPoints(const std::vector<glm::vec3>& verts);
// 	inline void addVertex(const glm::vec3& v);
// 	const glm::vec3& operator[](unsigned int i) const;
// 	unsigned int size() const;
//  private:
// 	std::vector<glm::vec3> m_verts_;
// };

// class PointsAdaptor {
//  public:
// 	PointsAdaptor(const NNPoints& pts);

// 	inline unsigned int kdtree_get_point_count() const;

// 	inline float kdtree_distance(const float* p1, const unsigned int index_2,
// 			unsigned int size) const;

// 	inline float kdtree_get_pt(const unsigned int i, int dim) const;

// 	template <class BBOX>
// 	bool kdtree_get_bbox(BBOX&) const { return false; }

//  private:
// 	const NNPoints& m_pts_;
// };
class NNPoints
{
public:
	NNPoints() : m_verts() { }
	NNPoints(const std::vector<glm::vec3>& verts) : m_verts(verts) { }
	inline void addVertex(const glm::vec3& v) { m_verts.push_back(v); }
	const glm::vec3& operator[](unsigned int i) const { return m_verts[i]; }
	unsigned int size() const { return m_verts.size(); }
private:
	std::vector<glm::vec3> m_verts;
};

class PointsAdaptor
{
public:
	PointsAdaptor(const NNPoints& pts) : m_pts(pts) { }

	inline unsigned int kdtree_get_point_count() const
	{ return m_pts.size(); }

	inline float kdtree_distance(const float* p1, const unsigned int index_2,
			unsigned int size) const
	{
		return glm::length(glm::vec3(p1[0], p1[1], p1[2]) -
				m_pts[index_2]);
	}

	inline float kdtree_get_pt(const unsigned int i, int dim) const
	{
		if (dim == 0) return m_pts[i].x;
		else if (dim == 1) return m_pts[i].y;
		else return m_pts[i].z;
	}


	template <class BBOX>
	bool kdtree_get_bbox(BBOX&) const { return false; }

private:
	const NNPoints& m_pts;
};
}

#endif