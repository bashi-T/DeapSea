#pragma once
#include <assert.h>
#include <cmath>
#include <math.h>
#include<stdio.h>
#include<limits>
#include<numbers>

typedef struct Matrix4x4 {
	float m[4][4];
} Matrix4x4;

typedef struct Matrix3x3 {
	float m[3][3];
} Matrix3x3;

typedef struct Matrix2x2 {
	float m[2][2];
} Matrix2x2;

typedef struct Vector2 {
	float x;
	float y;
} Vector2;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

struct Vector4 final {
	float x;
	float y;
	float z;
	float a;
};

struct Line//直線
{
	Vector3 origin;//始点
	Vector3 diff;//終点
};

struct Ray//半直線
{
	Vector3 origin;
	Vector3 diff;
};

struct Segment//線分
{
	Vector3 origin;
	Vector3 diff;
};

struct Plane {//平面
	Vector3 normal;//法線
	float distance;//距離
};

struct Triangle
{
	Vector3 vertices[3];
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;//中心点
	Vector3 orientations[3];//座標軸　正規化・直交必須 0=x 1=y 2=z
	Vector3 size;//中心から面までの距離　座標軸方向の長さの半分
};

struct EulerTransform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

const float radianPerFrequency = 0.0174533f;
const float theta_ = 3.14159265359f;

constexpr double epsilon = std::numeric_limits<double>::epsilon();

Matrix2x2 Add(Matrix2x2 a, Matrix2x2 b);
Matrix2x2 Subtract(Matrix2x2 a, Matrix2x2 b);
Matrix2x2 Multiply(Matrix2x2 a, Matrix2x2 b);

Vector2 Multiply(Vector2 v, Matrix2x2 b);

Matrix3x3 Multiply(Matrix3x3 a, Matrix3x3 b);


Matrix2x2 MakeRotateMatrix(float theta);

Matrix3x3 MakeRotateMatrix3x3(float theta);

Matrix3x3 MakeTranslateMatrix(Vector2 translate);

Vector2 Transform(Vector2 vector, Matrix3x3 matrix);

Matrix2x2 Inverse(Matrix2x2 a);

Matrix3x3 Inverse(Matrix3x3 a);

Matrix2x2 Transpose(Matrix2x2 a);

Matrix3x3 Transpose(Matrix3x3 a);

Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);

Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height);

Matrix3x3 Inverse3x3(Matrix3x3 a);

Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t);

Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);
//内積
float Dot(const Vector3& v1, const Vector3& v2);
//ベクトルの長さ
float Length(const Vector3& v);
//正規化
Vector3 Normalize(const Vector3& v);

Matrix4x4 Add(Matrix4x4 a, Matrix4x4 b);

Matrix4x4 Subtract(Matrix4x4 a, Matrix4x4 b);

Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b);

Matrix4x4 Inverse(Matrix4x4 a);

Matrix4x4 Transpose(Matrix4x4 a);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateMatrix(Vector3 rotate);

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);
//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//正射影
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

Matrix4x4 DirectionTodirection(const Vector3& from, const Vector3& to);

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

Quaternion identityQuaternion();

Quaternion Conjugate(const Quaternion& quaternion);

float Norm(const Quaternion& quaternion);

Quaternion Normalize(const Quaternion& quaternion);//正規化・単位Quaternion

Quaternion Inverse(const Quaternion& quaternion);

Quaternion MakerotateAxisQuaternion(const Vector3& axis, float angle);

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

Matrix4x4 MakeAffineMatrix(Vector3 scale, Quaternion rotate, Vector3 translate);

float Lerp(const float& a, const float& b, float t);

float LerpShortAngle(float a, float b, float t);
//公転
Vector3 Revolution(Vector3 distance, Vector3 tergetTranslate,Vector3 rotate);