#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "MyStruct.h"

/// <summary>
/// Renderingクラスの作成
/// </summary>
class Rendering
{
public://メンバ関数の作成

#pragma region 4x4行列メンバ関数の作成

	/// <summary>
	/// 行列の積
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	/// <summary>
	/// 拡大縮小行列
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/// <summary>
	/// X軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(float radian);

	/// <summary>
	/// Y軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(float radian);

	/// <summary>
	/// Z軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(float radian);

	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateMatrix(const Vector3& radian);

	/// <summary>
	/// 平行移動行列
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	/// <summary>
	/// アフィン変換行列
	/// </summary>
	/// <param name="affine"></param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// トランスフォーム
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="matrix"></param>
	/// <returns></returns>
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

#pragma endregion

	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

};

