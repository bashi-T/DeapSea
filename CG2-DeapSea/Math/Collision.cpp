#include "Collision.h"

bool isCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x) && (aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y) && (aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z) && (aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	else {
		return false;
	}
}

bool isCollision(const AABB& aabb, const Sphere& sphere)
{
	Vector3 closestPoint
	{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};
	float distance = Length(Subtract(closestPoint, sphere.center));
	if (distance <= sphere.radius)
	{
		return true;
	}
	else {
		return false;
	}
}

bool isCollision(const OBB& obb, const Sphere& sphere)
{
	Matrix4x4 OBBWorldMatrix
	{
		obb.orientations[0].x,
		obb.orientations[0].y,
		obb.orientations[0].z,
		0.0f,

		obb.orientations[1].x,
		obb.orientations[1].y,
		obb.orientations[1].z,
		0.0f,

		obb.orientations[2].x,
		obb.orientations[2].y,
		obb.orientations[2].z,
		0.0f,

		obb.center.x,
		obb.center.y,
		obb.center.z,
		1.0f
	};

	Matrix4x4 OBBWorldMatrixInverce = Inverse(OBBWorldMatrix);
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, OBBWorldMatrixInverce);
	AABB aabbOBBLocal{ {-obb.size.x,-obb.size.y,-obb.size.z},obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace ,sphere.radius };

	return isCollision(aabbOBBLocal, sphereOBBLocal);
}

bool isCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	float distance = Length(Subtract(sphere2.center, sphere1.center));
	if (distance <= sphere1.radius + sphere2.radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
