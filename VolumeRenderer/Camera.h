#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtx/vector_angle.hpp>
#include "Defs.h"



class Light;

class Camera
{
public:
	Camera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle);
	Camera(const glm::vec3 & eyepos, const glm::vec3 & viewDir);
	~Camera(void);

	glm::mat4 GetModel();
	glm::mat4 GetProj();
	glm::mat4 GetView();
	glm::vec3 GetEyePos();
	void GetSphericalAngles(float & horizonAngle, float & verticalAngle);

	void SetCamera();
	void SetCamera(glm::vec3 eyePos, float horizonAngle, float VerticalAngle);
	void SetCamera(glm::vec3 eyePos, glm::vec3 viewDir);
	void RevertCameraToResetPoint();

	void Print();
	void Print(glm::vec3 eyePos, glm::vec2 angles);

	void Move(EDirection dir);
	void Rotate(const glm::vec2 & degree);

	void UpdateRenderMat(/*GraphicsEngine * ge*/);
	void SetMVPForDepth(Light * inLight);

	const float fLeft;
	const float fRight;
	const float fBottom;
	const float fTop;
	const float fNear;
	const float fFar;

	static void ViewDirToSphericalAngles(const glm::vec3 & viewDir, float & horizonAngle, float &verticalAngle);
	static void SphericalAnglesToViewDir(float horizonAngle, float verticalAngle, glm::vec3 & viewDir);
	static glm::vec3 SphericalAnglesToViewDir(float horizonAngle, float verticalAngle);

protected:

	

	// camera position
	struct CameraData
	{
		glm::vec3 fEyePos;
		float fHorizonAngle;
		float fVerticalAngle;
		CameraData() : fEyePos(0.f, 0.f, 0.f), fHorizonAngle(0.f), fVerticalAngle(0.f) {}
		CameraData(const glm::vec3 & eyePos, float horizonAngle, float verticalAngle) : fEyePos(eyePos), fHorizonAngle(horizonAngle), fVerticalAngle(verticalAngle) {}
	};
	CameraData fCurCamera;
	CameraData fResetCamera;

	// transformation matrices
	glm::mat4 fViewMat;
	glm::mat4 fProjMat;
	glm::mat4 fModelMat;

};

//float Camera::fov = 45.0f;
//float Camera::aspect = 1.0f;
//float Camera::near = 0.1f;
//float Camera::far = 500.0f;




///////////////////////////////////////////////////////////////////////////////////////
// Camera Utilities
///////////////////////////////////////////////////////////////////////////////////////
namespace fpscam {

#define mat4_type glm::detail::tmat4x4<T>
	//template<typename T>
	//using mat4_type = glm::detail::tmat4x4<T, glm::highp>;

#define vec4_type glm::detail::tvec4<T>
	//template<typename T>
	//using vec4_type = glm::detail::tvec4<T, glm::highp>;

#define vec3_type glm::detail::tvec3<T>
	//template<typename T>
	//using vec3_type = glm::detail::tvec3<T, glm::highp>;

	template<typename T>
	void MoveForward(T amount, bool inParallelWithGround, mat4_type &viewMatrix) // If you want to move along on the ground, parallelWithGround = true.
	{
		if (inParallelWithGround)
		{
			vec4_type moveVec = vec4_type(viewMatrix[0][2], viewMatrix[1][2], 0, 0);

			// if looking straight down or up such that you can't move forward, do something smart
			if( glm::dot(moveVec, moveVec) < T(FLT_EPSILON) )
				moveVec = -vec4_type(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0);

			viewMatrix[3] += viewMatrix * (glm::normalize(moveVec) * amount);
		}
		else
		{
			viewMatrix[3].z += amount;
		}
	}

	template<typename T>
	void StrafeRight(T amount, bool inParallelWithGround, mat4_type & viewMatrix)
	{
		if (inParallelWithGround)
		{
			vec4_type moveVec = -vec4_type(viewMatrix[0][0], viewMatrix[1][0], 0, 0);

			// if rolled 90 degrees or equivalent such that you can't move right, do something smart
			if( glm::dot(moveVec, moveVec) < T(FLT_EPSILON) )
				moveVec = -vec4_type(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0);

			viewMatrix[3] += viewMatrix * (glm::normalize(moveVec) * amount);
		}
		else
		{
			viewMatrix[3].x += amount;
		}
	}

	template<typename T>
	void MoveUp(T amount, bool inWorldUp, mat4_type & viewMatrix) // inWorldUp = true : 
	{
		vec4_type moveVec = inWorldUp ? -amount * viewMatrix[2] : vec4_type(0, amount, 0, 0);
		viewMatrix[3] += moveVec;
	}

	template<typename T>
	void LookRight(T degrees, bool rotateAboutWorldUp, mat4_type & viewMatrix) // rotateAboutWorldUp = true : camera will rotate around the camera up vector, not world up vector.
	{
		vec4_type rotVec = rotateAboutWorldUp ? viewMatrix[2] : vec4_type(0, 0, 1, 0);
		viewMatrix = glm::rotate(mat4_type(1), degrees, vec3_type(rotVec)) * viewMatrix;
	}

	template<typename T>
	void LookUp(T degrees, bool inAllowUpsideDown, mat4_type & viewMatrix)
	{
		// camera's up vector in world space.
		vec3_type up = vec3_type(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

		mat4_type rotMat = glm::rotate(mat4_type(1), -degrees, vec3_type(1, 0, 0));

		// the result of looking up, which could lead to an upside down view.
		mat4_type rotatedViewMatrix = rotMat * viewMatrix;

		if ( !inAllowUpsideDown && rotatedViewMatrix[2][1] < 0)
		{
			// Result of lookup would be upside down.
			// Recalculate the rotation angle so that the camera does not flip upside down.

			// up vector projected on the the world XY plane
			vec3_type projectedUp = glm::normalize(vec3_type(up.x, up.y, 0));;

			// camera's right vector in world space coords.
			vec3_type right = vec3_type(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);

			// complicated math..
			vec3_type cross = glm::cross(vec3_type(0, 0, 1), right);
			T sign = glm::sign(up.z) * ((glm::dot(cross, up) > 0) ? 1 : -1);

			// Rebuild rotatedViewMatrix with a better 'degrees'
			degrees = glm::degrees(glm::angle(up, projectedUp));

			rotMat = glm::rotate(mat4_type(1), sign*degrees, vec3_type(1,0,0));
			rotatedViewMatrix = rotMat * viewMatrix;
		}

		viewMatrix = rotatedViewMatrix;
	}

	template<typename T>
	void OrbitRight(const vec3_type & inOrigin, T inDegree, mat4_type & inOutViewMatrix)
	{
		mat4_type cameraMat = glm::affineInverse(inOutViewMatrix);
		mat4_type translateMat = glm::translate(mat4_type(1), inOrigin);
		mat4_type rotMat = glm::rotate(mat4_type(1), inDegree, vec3_type(0, 0, 1));
		cameraMat = translateMat * rotMat * glm::affineInverse(translateMat) * cameraMat;

		inOutViewMatrix = glm::affineInverse(cameraMat);
	}

	template<typename T>
	void OrbitUp(const vec3_type & inOrigin, T inDegree, mat4_type & inOutViewMatrix)
	{
		mat4_type cameraMat = glm::affineInverse(inOutViewMatrix);
		mat4_type translateMat = glm::translate(mat4_type(1), inOrigin);
		mat4_type rotMat = glm::rotate(mat4_type(1), inDegree, vec3_type(cameraMat[0]));
		cameraMat = translateMat * rotMat * glm::affineInverse(translateMat) * cameraMat;

		inOutViewMatrix = glm::affineInverse(cameraMat);
	}
} // end namespace fpscam


