#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib\include\Geometry\AABB.h"
#include <list>

class GameObject; 

class Quadtree {
	
	struct QuadtreeNode
	{
		QuadtreeNode() {}
		QuadtreeNode(AABB limits) : boundary(limits) {}
		~QuadtreeNode() { }

		bool Insert(GameObject *go_to_insert); 
		void QuadtreeNode::CreateChildren(); 
		void Clear(); 
		void DrawDebug(); 

		template<typename TYPE>
		inline void Intersect(std::vector<GameObject*>& intersections, const TYPE & primitive) const; 

		AABB boundary; 
		int capacity = 0; 
		QuadtreeNode *children = nullptr; 

		std::list<GameObject*> objects; 
		int bucket_space = 1; 
	};

public: 
	void Create(const AABB limits); 
	void Clear(); 
	void Insert(GameObject *go_to_insert);
	void Remove(GameObject *go_to_remove); 

	template<typename TYPE>
	inline void Intersect(std::vector<GameObject*>& intersections, const TYPE & primitive) const;

	void DebugDraw();

private: 
	QuadtreeNode *root_node = nullptr; 

};
#endif
