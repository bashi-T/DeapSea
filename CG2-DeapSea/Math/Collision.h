#pragma once
#include<algorithm>
#include "CGvector.h"

//bool IsCollision(const Segment&segment,const Plane&plane)
//{
//	float dot = Dot(plane.normal, line.diff);
//	if (dot == 0.0f)
//	{
//		return false;
//	}
//
//	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
//
//}

bool isCollision(const AABB& aabb1, const AABB& aabb2);

bool isCollision(const AABB& aabb, const Sphere& sphere);

bool isCollision(const OBB& obb, const Sphere& sphere);

//bool isCollision(const OBB& obb1, const OBB& obb2)
//{
//	float sumSpan;
//}

bool isCollision(const Sphere& sphere1, const Sphere& sphere2);
