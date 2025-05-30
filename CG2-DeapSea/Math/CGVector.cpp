#include "CGVector.h"

Matrix2x2 Add(Matrix2x2 a, Matrix2x2 b)
{
	Matrix2x2 add;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			add.m[x][y] = a.m[x][y] + b.m[x][y];
		}
	}
	return add;
}

Matrix2x2 Subtract(Matrix2x2 a, Matrix2x2 b) 
{
	Matrix2x2 subtract;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++) 
		{
			subtract.m[x][y] = a.m[x][y] - b.m[x][y];
		}
	}
	return subtract;
}

Matrix2x2 Multiply(Matrix2x2 a, Matrix2x2 b)
{
	Matrix2x2 multiply;
	multiply.m[0][0] = (a.m[0][0] * b.m[0][0]) + (a.m[0][1] * b.m[1][0]);
	multiply.m[0][1] = (a.m[0][0] * b.m[0][1]) + (a.m[0][1] * b.m[1][1]);
	multiply.m[1][0] = (a.m[1][0] * b.m[0][0]) + (a.m[1][1] * b.m[1][0]);
	multiply.m[1][1] = (a.m[1][0] * b.m[0][1]) + (a.m[1][1] * b.m[1][1]);

	return multiply;
}

Vector2 Multiply(Vector2 v, Matrix2x2 b)
{
	Vector2 multiply;
	multiply.x = (v.x * b.m[0][0]) + (v.y * b.m[1][0]);
	multiply.y = (v.x * b.m[0][1]) + (v.y * b.m[1][1]);
	return multiply;
}

Matrix3x3 Multiply(Matrix3x3 a, Matrix3x3 b)
{
	Matrix3x3 multiply;
	multiply.m[0][0] = (a.m[0][0] * b.m[0][0]) + (a.m[0][1] * b.m[1][0]);
	multiply.m[0][1] = (a.m[0][0] * b.m[0][1]) + (a.m[0][1] * b.m[1][1]);
	multiply.m[0][2] = (a.m[0][0] * b.m[0][1]) + (a.m[0][1] * b.m[1][1]);
	multiply.m[1][0] = (a.m[1][0] * b.m[0][0]) + (a.m[1][1] * b.m[1][0]);
	multiply.m[1][1] = (a.m[1][0] * b.m[0][1]) + (a.m[1][1] * b.m[1][1]);
	multiply.m[1][2] = (a.m[1][0] * b.m[0][1]) + (a.m[1][1] * b.m[1][1]);
	multiply.m[2][0] = (a.m[1][0] * b.m[0][0]) + (a.m[1][1] * b.m[1][0]);
	multiply.m[2][1] = (a.m[1][0] * b.m[0][1]) + (a.m[1][1] * b.m[1][1]);
	multiply.m[2][2] = (a.m[1][0] * b.m[0][1]) + (a.m[1][1] * b.m[1][1]);

	return multiply;
}

Matrix2x2 MakeRotateMatrix(float theta)
{
	Matrix2x2 rotate;
	rotate.m[0][0] = std::cos(theta);
	rotate.m[0][1] = std::sin(theta);
	rotate.m[1][0] = -std::sin(theta);
	rotate.m[1][1] = std::cos(theta);

	return rotate;
}
Matrix3x3 MakeRotateMatrix3x3(float theta)
{
	Matrix3x3 rotate;
	rotate.m[0][0] = std::cos(theta);
	rotate.m[1][0] = -std::sin(theta);
	rotate.m[2][0] = 0;
	rotate.m[0][1] = std::sin(theta);
	rotate.m[1][1] = std::cos(theta);
	rotate.m[2][1] = 0;
	rotate.m[0][2] = 0;
	rotate.m[1][2] = 0;
	rotate.m[2][2] = 1;

	return rotate;
}
Matrix3x3 MakeTranslateMatrix(Vector2 translate)
{
	Matrix3x3 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1.0f;

	return result;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix)
{
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Matrix2x2 Inverse(Matrix2x2 a) 
{
	Matrix2x2 result = {0};
	float x = a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0];
	if (x != 0)
	{
		result.m[0][0] = 1 / x * a.m[1][1];
		result.m[0][1] = 1 / x * -(a.m[0][1]);
		result.m[1][0] = 1 / x * -(a.m[1][0]);
		result.m[1][1] = 1 / x * a.m[0][0];
	}
	return result;
}

Matrix3x3 Inverse(Matrix3x3 a)
{
	Matrix3x3 result;
	float x = a.m[0][0] * a.m[1][1] * a.m[2][2] + a.m[0][1] * a.m[1][2] * a.m[2][1] + a.m[0][2] * a.m[1][0] * a.m[2][1] - 
		a.m[2][0] * a.m[1][1] * a.m[2][0] - a.m[0][1] * a.m[2][1] * a.m[2][2] - a.m[0][0] * a.m[1][0] * a.m[2][1];
	if (x != 0)
	{
		result.m[0][0] = 1 / x * (a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1]);
		result.m[0][1] = 1 / x * -(a.m[0][1] * a.m[2][2] - a.m[0][2] * a.m[2][1]);
		result.m[0][2] = 1 / x * (a.m[0][1] * a.m[1][2] - a.m[0][2] * a.m[1][1]);
		result.m[1][0] = 1 / x * -(a.m[1][0] * a.m[2][2] - a.m[1][2] * a.m[2][0]);
		result.m[1][1] = 1 / x * (a.m[0][0] * a.m[2][2] - a.m[0][2] * a.m[2][0]);
		result.m[1][2] = 1 / x * -(a.m[0][0] * a.m[1][2] - a.m[0][2] * a.m[1][0]);
		result.m[2][0] = 1 / x * (a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0]);
		result.m[2][1] = 1 / x * -(a.m[0][0] * a.m[2][1] - a.m[0][1] * a.m[2][0]);
		result.m[2][2] = 1 / x * (a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0]);
	}
	return result;
}

Matrix2x2 Transpose(Matrix2x2 a)
{
	Matrix2x2 result;
	result.m[0][0] = a.m[0][0];
	result.m[1][1] = a.m[1][1];
	result.m[0][1] = a.m[1][0];
	result.m[1][0] = a.m[0][1];
	return result;
}

Matrix3x3 Transpose(Matrix3x3 a)
{
	Matrix3x3 result;
	result.m[0][0] = a.m[0][0];
	result.m[1][0] = a.m[0][1];
	result.m[2][0] = a.m[0][2];
	result.m[0][1] = a.m[1][0];
	result.m[1][1] = a.m[1][1];
	result.m[2][1] = a.m[1][2];
	result.m[0][2] = a.m[2][0];
	result.m[1][2] = a.m[2][1];
	result.m[2][2] = a.m[2][2];
	return result;
}

Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom) // 正射影
{
	Matrix3x3 result;
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][0] = 0;
	result.m[2][0] = (left + right) / (left - right);
	result.m[0][1] = 0;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][1] = (top + bottom) / (bottom - top);
	result.m[0][2] = 0;
	result.m[1][2] = 0;
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height)// ビューポート
{ 
	Matrix3x3 result;
	result.m[0][0] = width / 2;
	result.m[1][0] = 0;
	result.m[2][0] = left + width / 2;
	result.m[0][1] = 0;
	result.m[1][1] = -height / 2;
	result.m[2][1] = top + height / 2;
	result.m[0][2] = 0;
	result.m[1][2] = 0;
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 Inverse3x3(Matrix3x3 a)
{
	Matrix3x3 result;
	result.m[2][0] = -a.m[2][0];
	result.m[2][1] = -a.m[2][1];
	return result;
};

Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
	Vector2 f;
	f.x = t * b.x + (1.0f - t) * a.x;
	f.y = t * b.y + (1.0f - t) * a.y;
	return f;
}

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t)
{
	Vector2 p0p1 = Lerp(p0, p1, t);
	Vector2 p1p2 = Lerp(p1, p2, t);
	Vector2 p = Lerp(p0p1, p1p2, t);
	return p;
}

Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
{
	Vector3 f;
	f.x = t * b.x + (1.0f - t) * a.x;
	f.y = t * b.y + (1.0f - t) * a.y;
	f.z = t * b.z + (1.0f - t) * a.z;
	return f;
}

float Lerp(const float& a, const float& b, float t)
{
	float result{};

	result = a + b * t;

	return result;
}

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 v3 = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return v3;
};
Vector3 Subtract(const Vector3& v1, const Vector3& v2) 
{
	Vector3 v3 = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return v3;
};

Vector3 Multiply(float scalar, const Vector3& v) 
{
	Vector3 v3 = {v.x * scalar, v.y * scalar, v.z * scalar};
	return v3;
}

float Dot(const Vector3& v1, const Vector3& v2) 
{
	float dot;
	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return dot;
}

float Length(const Vector3& v)
{
	float length;
	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return length;
}

Vector3 Normalize(const Vector3& v)
{
	float length;
	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	Vector3 normalize
	{
	    v.x / length,
	    v.y / length,
	    v.z / length,
	};
	return normalize;
}

Matrix4x4 Add(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 add;
	add.m[0][0] = a.m[0][0] + b.m[0][0];
	add.m[0][1] = a.m[0][1] + b.m[0][1];
	add.m[0][2] = a.m[0][2] + b.m[0][2];
	add.m[0][3] = a.m[0][3] + b.m[0][3];
	add.m[1][0] = a.m[1][0] + b.m[1][0];
	add.m[1][1] = a.m[1][1] + b.m[1][1];
	add.m[1][2] = a.m[1][2] + b.m[1][2];
	add.m[1][3] = a.m[1][3] + b.m[1][3];
	add.m[2][0] = a.m[2][0] + b.m[2][0];
	add.m[2][1] = a.m[2][1] + b.m[2][1];
	add.m[2][2] = a.m[2][2] + b.m[2][2];
	add.m[2][3] = a.m[2][3] + b.m[2][3];
	add.m[3][0] = a.m[3][0] + b.m[3][0];
	add.m[3][1] = a.m[3][1] + b.m[3][1];
	add.m[3][2] = a.m[3][2] + b.m[3][2];
	add.m[3][3] = a.m[3][3] + b.m[3][3];
	return add;
}

Matrix4x4 Subtract(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 subtract;
	subtract.m[0][1] = a.m[0][1] - b.m[0][1];
	subtract.m[0][0] = a.m[0][0] - b.m[0][0];
	subtract.m[0][2] = a.m[0][2] - b.m[0][2];
	subtract.m[0][3] = a.m[0][3] - b.m[0][3];
	subtract.m[1][0] = a.m[1][0] - b.m[1][0];
	subtract.m[1][1] = a.m[1][1] - b.m[1][1];
	subtract.m[1][2] = a.m[1][2] - b.m[1][2];
	subtract.m[1][3] = a.m[1][3] - b.m[1][3];
	subtract.m[2][0] = a.m[2][0] - b.m[2][0];
	subtract.m[2][1] = a.m[2][1] - b.m[2][1];
	subtract.m[2][2] = a.m[2][2] - b.m[2][2];
	subtract.m[2][3] = a.m[2][3] - b.m[2][3];
	subtract.m[3][0] = a.m[3][0] - b.m[3][0];
	subtract.m[3][1] = a.m[3][1] - b.m[3][1];
	subtract.m[3][2] = a.m[3][2] - b.m[3][2];
	subtract.m[3][3] = a.m[3][3] - b.m[3][3];
	return subtract;
}

Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 multiply;
	multiply.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + 
	                   a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
	multiply.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + 
	                   a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
	multiply.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + 
	                   a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
	multiply.m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + 
	                   a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];
	multiply.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + 
	                   a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
	multiply.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + 
	                   a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
	multiply.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + 
	                   a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
	multiply.m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + 
	                   a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];
	multiply.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + 
	                   a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
	multiply.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + 
	                   a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
	multiply.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + 
	                   a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
	multiply.m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + 
	                   a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];
	multiply.m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + 
	                   a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
	multiply.m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + 
	                   a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
	multiply.m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + 
	                   a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
	multiply.m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + 
	                   a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

	return multiply;
}

Matrix4x4 Inverse(Matrix4x4 a)
{
	Matrix4x4 result = {0};
	float x = a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3] +
	          a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1] +
	          a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2] -
	          a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1] -
	          a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3] -
	          a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2] -
	          a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3] -
	          a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1] -
	          a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2] +
	          a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1] +
	          a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3] +
	          a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2] +
	          a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3] +
	          a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1] +
	          a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2] -
	          a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1] -
	          a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3] -
	          a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2] -
	          a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0] -
	          a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0] -
	          a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0] +
	          a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0] +
	          a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0] +
	          a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0];

	if (x != 0)
	{
		result.m[0][0] = 1.0f / x *
		                 (a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][1] +
		                  a.m[1][3] * a.m[2][1] * a.m[3][2] - a.m[1][3] * a.m[2][2] * a.m[3][1] -
		                  a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[1][1] * a.m[2][3] * a.m[3][2]);

		result.m[0][1] = 1.0f / x *
		                 (-a.m[0][1] * a.m[2][2] * a.m[3][3] - a.m[0][2] * a.m[2][3] * a.m[3][1] -
		                  a.m[0][3] * a.m[2][1] * a.m[3][2] + a.m[0][3] * a.m[2][2] * a.m[3][1] +
		                  a.m[0][2] * a.m[2][1] * a.m[3][3] + a.m[0][1] * a.m[2][3] * a.m[3][2]);

		result.m[0][2] = 1.0f / x *
		                 (a.m[0][1] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][1] +
		                  a.m[0][3] * a.m[1][1] * a.m[3][2] - a.m[0][3] * a.m[1][2] * a.m[3][1] -
		                  a.m[0][2] * a.m[1][1] * a.m[3][3] - a.m[0][1] * a.m[1][3] * a.m[3][2]);

		result.m[0][3] = 1.0f / x *
		                 (-a.m[0][1] * a.m[1][2] * a.m[2][3] - a.m[0][2] * a.m[1][3] * a.m[2][1] -
		                  a.m[0][3] * a.m[1][1] * a.m[2][2] + a.m[0][3] * a.m[1][2] * a.m[2][1] +
		                  a.m[0][2] * a.m[1][1] * a.m[2][3] + a.m[0][1] * a.m[1][3] * a.m[2][2]);

		result.m[1][0] = 1.0f / x *
		                 (-a.m[1][0] * a.m[2][2] * a.m[3][3] - a.m[1][2] * a.m[2][3] * a.m[3][0] -
		                  a.m[1][3] * a.m[2][0] * a.m[3][2] + a.m[1][3] * a.m[2][2] * a.m[3][0] +
		                  a.m[1][2] * a.m[2][0] * a.m[3][3] + a.m[1][0] * a.m[2][3] * a.m[3][2]);

		result.m[1][1] = 1.0f / x *
		                 (a.m[0][0] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][0] +
		                  a.m[0][3] * a.m[2][0] * a.m[3][2] - a.m[0][3] * a.m[2][2] * a.m[3][0] -
		                  a.m[0][2] * a.m[2][0] * a.m[3][3] - a.m[0][0] * a.m[2][3] * a.m[3][2]);

		result.m[1][2] = 1.0f / x *
		                 (-a.m[0][0] * a.m[1][2] * a.m[3][3] - a.m[0][2] * a.m[1][3] * a.m[3][0] -
		                  a.m[0][3] * a.m[1][0] * a.m[3][2] + a.m[0][3] * a.m[1][2] * a.m[3][0] +
		                  a.m[0][2] * a.m[1][0] * a.m[3][3] + a.m[0][0] * a.m[1][3] * a.m[3][2]);

		result.m[1][3] = 1.0f / x *
		                 (a.m[0][0] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] +
		                  a.m[0][3] * a.m[1][0] * a.m[2][2] - a.m[0][3] * a.m[1][2] * a.m[2][0] -
		                  a.m[0][2] * a.m[1][0] * a.m[2][3] - a.m[0][0] * a.m[1][3] * a.m[2][2]);

		result.m[2][0] = 1.0f / x *
		                 (a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[1][1] * a.m[2][3] * a.m[3][0] +
		                  a.m[1][3] * a.m[2][0] * a.m[3][1] - a.m[1][3] * a.m[2][1] * a.m[3][0] -
		                  a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[1][0] * a.m[2][3] * a.m[3][1]);
		
		result.m[2][1] = 1.0f / x *
		                 (-a.m[0][0] * a.m[2][1] * a.m[3][3] - a.m[0][1] * a.m[2][3] * a.m[3][0] -
		                  a.m[0][3] * a.m[2][0] * a.m[3][1] + a.m[0][3] * a.m[2][1] * a.m[3][0] +
		                  a.m[0][1] * a.m[2][0] * a.m[3][3] + a.m[0][0] * a.m[2][3] * a.m[3][1]);

		result.m[2][2] = 1.0f / x *
		                 (a.m[0][0] * a.m[1][1] * a.m[3][3] + a.m[0][1] * a.m[1][3] * a.m[3][0] +
		                  a.m[0][3] * a.m[1][0] * a.m[3][1] - a.m[0][3] * a.m[1][1] * a.m[3][0] -
		                  a.m[0][1] * a.m[1][0] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[3][1]);

		result.m[2][3] = 1.0f / x *
		                 (-a.m[0][0] * a.m[1][1] * a.m[2][3] - a.m[0][1] * a.m[1][3] * a.m[2][0] -
		                  a.m[0][3] * a.m[1][0] * a.m[2][1] + a.m[0][3] * a.m[1][1] * a.m[2][0] +
		                  a.m[0][1] * a.m[1][0] * a.m[2][3] + a.m[0][0] * a.m[1][3] * a.m[2][1]);

		result.m[3][0] = 1.0f / x *
		                 (-a.m[1][0] * a.m[2][1] * a.m[3][2] - a.m[1][1] * a.m[2][2] * a.m[3][0] -
		                  a.m[1][2] * a.m[2][0] * a.m[3][1] + a.m[1][2] * a.m[2][1] * a.m[3][0] +
		                  a.m[1][1] * a.m[2][0] * a.m[3][2] + a.m[1][0] * a.m[2][2] * a.m[3][1]);

		result.m[3][1] = 1.0f / x *
		                 (a.m[0][0] * a.m[2][1] * a.m[3][2] + a.m[0][1] * a.m[2][2] * a.m[3][0] +
		                  a.m[0][2] * a.m[2][0] * a.m[3][1] - a.m[0][2] * a.m[2][1] * a.m[3][0] -
		                  a.m[0][1] * a.m[2][0] * a.m[3][2] - a.m[0][0] * a.m[2][2] * a.m[3][1]);

		result.m[3][2] = 1.0f / x *
		                 (-a.m[0][0] * a.m[1][1] * a.m[3][2] - a.m[0][1] * a.m[1][2] * a.m[3][0] -
		                  a.m[0][2] * a.m[1][0] * a.m[3][1] + a.m[0][2] * a.m[1][1] * a.m[3][0] +
		                  a.m[0][1] * a.m[1][0] * a.m[3][2] + a.m[0][0] * a.m[1][2] * a.m[3][1]);

		result.m[3][3] = 1.0f / x *
		                 (a.m[0][0] * a.m[1][1] * a.m[2][2] + a.m[0][1] * a.m[1][2] * a.m[2][0] +
		                  a.m[0][2] * a.m[1][0] * a.m[2][1] - a.m[0][2] * a.m[1][1] * a.m[2][0] -
		                  a.m[0][1] * a.m[1][0] * a.m[2][2] - a.m[0][0] * a.m[1][2] * a.m[2][1]);
	}
	return result;
}

Matrix4x4 Transpose(Matrix4x4 a)
{
	Matrix4x4 result;
	result.m[0][0] = a.m[0][0];
	result.m[1][0] = a.m[0][1];
	result.m[2][0] = a.m[0][2];
	result.m[3][0] = a.m[0][3];
	result.m[0][1] = a.m[1][0];
	result.m[1][1] = a.m[1][1];
	result.m[2][1] = a.m[1][2];
	result.m[3][1] = a.m[1][3];
	result.m[0][2] = a.m[2][0];
	result.m[1][2] = a.m[2][1];
	result.m[2][2] = a.m[2][2];
	result.m[3][2] = a.m[2][3];
	result.m[0][3] = a.m[3][0];
	result.m[1][3] = a.m[3][1];
	result.m[2][3] = a.m[3][2];
	result.m[3][3] = a.m[3][3];
	return result;
}

Matrix4x4 MakeIdentity4x4() 
{
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 rotate;
	rotate.m[0][0] = 1.0f;
	rotate.m[0][1] = 0.0f;
	rotate.m[0][2] = 0.0f;
	rotate.m[0][3] = 0.0f;
	rotate.m[1][0] = 0.0f;
	rotate.m[1][1] = std::cos(radian);
	rotate.m[1][2] = std::sin(radian);
	rotate.m[1][3] = 0.0f;
	rotate.m[2][0] = 0.0f;
	rotate.m[2][1] = -std::sin(radian);
	rotate.m[2][2] = std::cos(radian);
	rotate.m[2][3] = 0.0f;
	rotate.m[3][0] = 0.0f;
	rotate.m[3][1] = 0.0f;
	rotate.m[3][2] = 0.0f;
	rotate.m[3][3] = 1.0f;

	return rotate;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 rotate;
	rotate.m[0][0] = std::cos(radian);
	rotate.m[0][1] = 0.0f;
	rotate.m[0][2] = -std::sin(radian);
	rotate.m[0][3] = 0.0f;
	rotate.m[1][0] = 0.0f;
	rotate.m[1][1] = 1.0f;
	rotate.m[1][2] = 0.0f;
	rotate.m[1][3] = 0.0f;
	rotate.m[2][0] = std::sin(radian);
	rotate.m[2][1] = 0.0f;
	rotate.m[2][2] = std::cos(radian);
	rotate.m[2][3] = 0.0f;
	rotate.m[3][0] = 0.0f;
	rotate.m[3][1] = 0.0f;
	rotate.m[3][2] = 0.0f;
	rotate.m[3][3] = 1.0f;

	return rotate;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 rotate;
	rotate.m[0][0] = std::cos(radian);
	rotate.m[0][1] = std::sin(radian);
	rotate.m[0][2] = 0.0f;
	rotate.m[0][3] = 0.0f;
	rotate.m[1][0] = -std::sin(radian);
	rotate.m[1][1] = std::cos(radian);
	rotate.m[1][2] = 0.0f;
	rotate.m[1][3] = 0.0f;
	rotate.m[2][0] = 0.0f;
	rotate.m[2][1] = 0.0f;
	rotate.m[2][2] = 1.0f;
	rotate.m[2][3] = 0.0f;
	rotate.m[3][0] = 0.0f;
	rotate.m[3][1] = 0.0f;
	rotate.m[3][2] = 0.0f;
	rotate.m[3][3] = 1.0f;

	return rotate;
}

Matrix4x4 MakeRotateMatrix(Vector3 rotate)
{
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 ROTATE = Multiply(rotateX, Multiply(rotateY, rotateZ));
	return ROTATE;
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate)
{
	Matrix4x4 ROTATE = MakeRotateMatrix(rotate);
	Matrix4x4 SCALE = MakeScaleMatrix(scale);
	Matrix4x4 TRANSLATE = MakeTranslateMatrix(translate);
	Matrix4x4 result= Multiply(SCALE, Multiply(ROTATE, TRANSLATE));
	result.m[0][0] = scale.x * ROTATE.m[0][0];
	result.m[0][1] = scale.x * ROTATE.m[0][1];
	result.m[0][2] = scale.x * ROTATE.m[0][2];
	result.m[0][3] = 0.0f;
	result.m[1][0] = scale.y * ROTATE.m[1][0];
	result.m[1][1] = scale.y * ROTATE.m[1][1];
	result.m[1][2] = scale.y * ROTATE.m[1][2];
	result.m[1][3] = 0.0f;
	result.m[2][0] = scale.z * ROTATE.m[2][0];
	result.m[2][1] = scale.z * ROTATE.m[2][1];
	result.m[2][2] = scale.z * ROTATE.m[2][2];
	result.m[2][3] = 0.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;


	return result;
};

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result;

	result.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2));
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = (1.0f / std::tan(fovY / 2));
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;
}

Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result;

	result.m[0][0] = 2.0f / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1.0f / (nearClip - farClip);
	result.m[2][3] = 0;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result;

	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = -height / 2.0f;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) // 行ベクトル
{
	Matrix4x4 result;
	result.m[0][0] = axis.x * axis.x * (1 - cos(angle)) + cos(angle);
	result.m[0][1] = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
	result.m[0][2] = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
	result.m[0][3] = 0.0f;
	result.m[1][0] = axis.y * axis.x * (1 - cos(angle)) - axis.z * sin(angle);
	result.m[1][1] = axis.y * axis.y * (1 - cos(angle)) + cos(angle);
	result.m[1][2] = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);
	result.m[1][3] = 0.0f;
	result.m[2][0] = axis.z * axis.x * (1 - cos(angle)) + axis.y * sin(angle);
	result.m[2][1] = axis.z * axis.y * (1 - cos(angle)) - axis.x * sin(angle);
	result.m[2][2] = axis.z * axis.x * (1 - cos(angle)) + cos(angle);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 DirectionTodirection(const Vector3& from, const Vector3& to)
{
	Matrix4x4 result;
	Vector3 n = Normalize(Cross(from, to));
	float cosT = Dot(from, to);
	float sinT = sqrt(Dot(Cross(from, to), Cross(from, to)));
	if (cosT == -1.0f)
	{
		if (from.x != 0 || from.y != 0)
		{
			n = { from.y,-from.x,0.0f };
		}
		else if (from.x != 0 || from.z != 0)
		{
			n = { from.z,0.0f,-from.x };
		}
	}
	result.m[0][0] = n.x * n.x * (1 - cosT) + cosT;
	result.m[0][1] = n.x * n.y * (1 - cosT) + n.z * sinT;
	result.m[0][2] = n.x * n.z * (1 - cosT) - n.y * sinT;
	result.m[0][3] = 0.0f;
	result.m[1][0] = n.y * n.x * (1 - cosT) - n.z * sinT;
	result.m[1][1] = n.y * n.y * (1 - cosT) + cosT;
	result.m[1][2] = n.y * n.z * (1 - cosT) + n.x * sinT;
	result.m[1][3] = 0.0f;
	result.m[2][0] = n.z * n.x * (1 - cosT) + n.y * sinT;
	result.m[2][1] = n.z * n.y * (1 - cosT) - n.x * sinT;
	result.m[2][2] = n.z * n.z * (1 - cosT) + cosT;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	result.w = lhs.w * rhs.w - Dot({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });
	Vector3 resultV =
		Add(Cross({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z }),
			Add(Multiply(rhs.w, { lhs.x,lhs.y,lhs.z }),
				Multiply(lhs.w, { rhs.x,rhs.y,rhs.z })));
	result.x = resultV.x;
	result.y = resultV.y;
	result.z = resultV.z;
	return result;
}

Quaternion identityQuaternion()
{
	Quaternion result{ 0,0,0,1 };
	return result;
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	Quaternion result
	{
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};
	return result;
}

float Norm(const Quaternion& quaternion)
{
	float result = sqrt(
		quaternion.w * quaternion.w +
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z);
	return result;
}

Quaternion Normalize(const Quaternion& quaternion)//正規化・単位Quaternion
{
	float norm = Norm(quaternion);

	Quaternion result
	{
		quaternion.x / norm,
		quaternion.y / norm,
		quaternion.z / norm,
		quaternion.w / norm,
	};
	return result;
}

Quaternion Inverse(const Quaternion& quaternion)
{
	float norm = Norm(quaternion) * Norm(quaternion);
	Quaternion conjugation = Conjugate(quaternion);
	Quaternion result =
	{
		conjugation.x / norm,
		conjugation.y / norm,
		conjugation.z / norm,
		conjugation.w / norm
	};
	return result;
}

Quaternion MakerotateAxisQuaternion(const Vector3& axis, float angle)
{
	Quaternion result;
	Vector3 axisN;
	axisN.x = Normalize(axis).x;
	axisN.y = Normalize(axis).y;
	axisN.z = Normalize(axis).z;
	result =
	{
		std::sin(axisN.x * angle / 2),
		std::sin(axisN.y * angle / 2),
		std::sin(axisN.z * angle / 2),
		std::cos(angle / 2)
	};
	return Normalize(result);
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion quatV = { vector.x,vector.y,vector.z,0.0f };
	Vector3 result =
	{
		Multiply(quaternion,Multiply(quatV, Inverse(quaternion))).x,
		Multiply(quaternion,Multiply(quatV, Inverse(quaternion))).y,
		Multiply(quaternion,Multiply(quatV, Inverse(quaternion))).z,
	};

	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;
	result.m[0][0] = (quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) -
		(quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[0][1] = 2.0f * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z));
	result.m[0][2] = 2.0f * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	result.m[0][3] = 0;

	result.m[1][0] = 2.0f * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	result.m[1][1] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) +
		(quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[1][2] = 2.0f * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x));
	result.m[1][3] = 0;

	result.m[2][0] = 2.0f * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y));
	result.m[2][1] = 2.0f * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	result.m[2][2] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) -
		(quaternion.y * quaternion.y) + (quaternion.z * quaternion.z);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion result;
	float dot = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
	if (dot < 0)
	{
		const Quaternion& q2 = { -q0.x,-q0.y,-q0.z,-q0.w };
		dot = -dot;

		if (dot >= 1.0f - epsilon|| dot <= 1.0f + epsilon)
		{
			result.x = t * q1.x + (1.0f - t) * q2.x;
			result.y = t * q1.y + (1.0f - t) * q2.y;
			result.z = t * q1.z + (1.0f - t) * q2.z;
			result.w = t * q1.w + (1.0f - t) * q2.w;
			return result;
		}
		else
		{
			float theta = std::acos(dot);

			float scale0 = std::sin((1 - t) * theta) / std::sin(theta);
			float scale1 = std::sin(t * theta) / std::sin(theta);

			result.x = scale0 * q2.x + scale1 * q1.x;
			result.y = scale0 * q2.y + scale1 * q1.y;
			result.z = scale0 * q2.z + scale1 * q1.z;
			result.w = scale0 * q2.w + scale1 * q1.w;
			return result;
		}
	}
	else
	{
		if (dot >= 1.0f - epsilon || dot <= 1.0f + epsilon)
		{
			result.x = t * q1.x + (1.0f - t) * q0.x;
			result.y = t * q1.y + (1.0f - t) * q0.y;
			result.z = t * q1.z + (1.0f - t) * q0.z;
			result.w = t * q1.w + (1.0f - t) * q0.w;
			return result;
		}else
		{
			float theta = std::acos(dot);

			float scale0 = std::sin((1 - t) * theta) / std::sin(theta);
			float scale1 = std::sin(t * theta) / std::sin(theta);

			result.x = scale0 * q0.x + scale1 * q1.x;
			result.y = scale0 * q0.y + scale1 * q1.y;
			result.z = scale0 * q0.z + scale1 * q1.z;
			result.w = scale0 * q0.w + scale1 * q1.w;
			return result;
		}
	}
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Quaternion rotate, Vector3 translate)
{
	Matrix4x4 SCALE = MakeScaleMatrix(scale);
	Matrix4x4 ROTATE = MakeRotateMatrix(rotate);
	Matrix4x4 TRANSLATE = MakeTranslateMatrix(translate);
	Matrix4x4 result
	{
		result.m[0][0] = scale.x * ROTATE.m[0][0],
		result.m[0][1] = scale.x * ROTATE.m[0][1],
		result.m[0][2] = scale.x * ROTATE.m[0][2],
		result.m[0][3] = 0.0f,
		result.m[1][0] = scale.y * ROTATE.m[1][0],
		result.m[1][1] = scale.y * ROTATE.m[1][1],
		result.m[1][2] = scale.y * ROTATE.m[1][2],
		result.m[1][3] = 0.0f,
		result.m[2][0] = scale.z * ROTATE.m[2][0],
		result.m[2][1] = scale.z * ROTATE.m[2][1],
		result.m[2][2] = scale.z * ROTATE.m[2][2],
		result.m[2][3] = 0.0f,
		result.m[3][0] = translate.x,
		result.m[3][1] = translate.y,
		result.m[3][2] = translate.z,
		result.m[3][3] = 1.0f
	};
	return result;
}

float LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < 2 * (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= 2 * std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= (float)std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a, diff, t);
}

Vector3 Revolution(Vector3 distance, Vector3 tergetTranslate, Vector3 rotate)//distance.xは0,distance.zはマイナス
{
	Matrix4x4 RotateMatrix = MakeRotateMatrix(rotate);
	Vector3 offset = TransformNormal(distance, RotateMatrix);
	Vector3 translate = Add(tergetTranslate, offset);
	return translate;
}

