#pragma once
#include"Math/COllision.h"
#include"Systems/WindowApp.h"
namespace MyEngine

{
	class Camera
	{
	private:
		EulerTransform transformMatrix;
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projectionMatrix;
		Matrix4x4 viewProjectionMatrix;
		float fovY;
		float aspectRetio;
		float nearClip;
		float farClip;
		static inline Camera* instance;

	public:
		Camera();
		~Camera();
		void Initialize();
		void Update();
		void SetRotate(const Vector3& rotate) { transformMatrix.rotate = rotate; }
		void SetTranslate(const Vector3& translate) { transformMatrix.translate = translate; }
		void SetFovY(const float& FovY) { fovY = FovY; }
		void SetAspectretio(const float& AspectRetio) { aspectRetio = AspectRetio; }
		void SetNearClip(const float& NearClip) { nearClip = NearClip; }
		void SetFarClip(const float& FarClip) { farClip = FarClip; }
		const Vector3& GetRotate()const { return transformMatrix.rotate; }
		const Vector3& GetTranslate()const { return transformMatrix.translate; }
		const Matrix4x4& GetWorldMatrix() { return worldMatrix; }
		const Matrix4x4& GetViewMatrix() { return viewMatrix; }
		const Matrix4x4& GetProjectionMatrix() { return projectionMatrix; }
		const Matrix4x4& GetViewProjectionMatrix() { return viewProjectionMatrix; }
		static Camera* GetInstance();
		static void DeleteInstance();
	};
}