#pragma once
#define _USE_MATH_DEFINES
#include <Matrix4x4.h>
#include <Vector3.h>
#include <cassert>
#include <cmath>
#include <math.h>
#include <string>


Vector3 Add(Vector3 vector1, Vector3 vector2);
Vector3 Subtract(Vector3 vector1, Vector3 vector2);
Vector3 VectorMultiply(Vector3 vector1, Vector3 vector2);
Matrix4x4 Inverse(const Matrix4x4& m);

Vector3 Normalize(const Vector3& v);
float Length(Vector3 v);
float Dot(Vector3 vector1, Vector3 vector2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, Vector3& rotate, const Vector3& translate);
// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);