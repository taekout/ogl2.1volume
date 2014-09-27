#include "Camera.h"
#include "Light.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/swizzle.hpp>

Camera::Camera(const glm::vec3 & eyepos, const glm::vec3 & viewDir)
{
	Init(eyepos, viewDir);
}


Camera::~Camera(void)
{
}

void Camera::Init(const glm::vec3 & eyepos, const glm::vec3 & viewDir)
{
	if(glm::length(viewDir) == 0)
		throw "camera view direction cannot be 0.";
	fLeft = -30.f;
	fRight = 30.f;
	fBottom = -30.f;
	fTop = 30.f;
	fNear = 10.f;
	fFar = 500.f;
	//fov(45.0f), aspect(1.f), near(20.f), far(70.f)
	fFov = 45.f;
	fAspect = 1.f;
	fNear = 40.f;
	fFar = 500.f;

	glm::vec3 normalizedViewDir = glm::normalize(viewDir);
	fResetCamera = CameraData(eyepos, normalizedViewDir);
	fbCamInOrtho = false;
	//fProjMat = glm::ortho(fLeft, fRight, fBottom, fTop, fNear, fFar);
	fProjMat = glm::perspective(fFov, fAspect, fNear, fFar);
	fViewMat = glm::lookAt(eyepos, eyepos + normalizedViewDir, glm::vec3(0, 1, 0));
	fModelMat = glm::mat4(1);
}

glm::mat4 Camera::GetModel()
{
	return fModelMat;
}

glm::mat4 Camera::GetProj()
{
	return fProjMat;
}

glm::mat4 Camera::GetView()
{
	return fViewMat;
}

void Camera::SetView(const glm::mat4 & inViewMat)
{
	fViewMat = inViewMat;
}

void Camera::SetOrthoOrPerspective()
{
	if( fbCamInOrtho )
		fProjMat = glm::perspective(fFov, fAspect, fNear, fFar);
	else
		fProjMat = glm::ortho(fLeft, fRight, fBottom, fTop, fNear, fFar);
	fbCamInOrtho = !fbCamInOrtho;
}

glm::vec3 Camera::GetEyePos()
{
	return fpscam::GetEyePos(fViewMat);
}

glm::vec3 Camera::GetEyeDir()
{
	return fpscam::GetEyeDir(fViewMat);
}

void Camera::RevertCameraToResetPoint()
{
	Init(fResetCamera.fEyePos, fResetCamera.fViewDir);
}

void Camera::Print()
{
	return;
}

void Camera::Print(glm::vec3 eyePos, glm::vec2 angles)
{
	return;
	printf("Eye Pos : %f, %f, %f\n", eyePos.x, eyePos.y, eyePos.z);
	printf("Horizon Angle, Vertical Angle : %f, %f\n", angles.x, angles.y);
}

#define MOVE_SCALE  0.5f

void Camera::Move(EDirection dir)
{
	switch(dir) {

	case left: {
		fpscam::StrafeRight<float> (MOVE_SCALE, false, fViewMat);
		}
		break;

	case right: {
		fpscam::StrafeRight<float> (-MOVE_SCALE, false, fViewMat);
		}
		break;

	case forward: {
		fpscam::MoveForward<float> (MOVE_SCALE, false, fViewMat);
		}
		break;

	case backward: {
		fpscam::MoveForward<float>(-MOVE_SCALE, false, fViewMat);
		}
		break;
	case up: {
		fpscam::MoveUp<float> (-MOVE_SCALE, false, fViewMat); // false : upvector = (0,1,0).
		}
		break;
	case down: {
		fpscam::MoveUp<float> (MOVE_SCALE, false, fViewMat); // false : upvector = (0,1,0).
		}
		break;
	}
}

// Rotate view based on quaternion.
void Camera::Rotate(const glm::vec2 & degree)
{
	const float mouseSpeed = 0.5f;

	fpscam::LookUp<float> (mouseSpeed * degree.y, true, fViewMat);
	fpscam::LookRight<float> (mouseSpeed* degree.x, true, fViewMat);

	//fpscam::OrbitRight(glm::vec3(0, 0, 0), -degree.x, fViewMat);
	//fpscam::OrbitUp(glm::vec3(0, 0, 0), -degree.y, fViewMat);
	//fCurCamera.fVerticalAngle = fCurCamera.fVerticalAngle + degree.y * mouseSpeed;
	//fCurCamera.fHorizonAngle = fCurCamera.fHorizonAngle + degree.x * mouseSpeed;
	//glm::vec3 viewDir;
	//Camera::SphericalAnglesToViewDir(fCurCamera.fHorizonAngle, fCurCamera.fVerticalAngle, viewDir);
	////viewDir = glm::mat3(rotm) * viewDir;
	//SetCamera();
}

//void Camera::UpdateRenderMat(/*GraphicsEngine * ge*/)
//{
//}
//
//void Camera::SetMVPForDepth(Light * inLight) // Make it ILight.
//{
//	glm::vec3 viewDir(cos(fCurCamera.fVerticalAngle) * sin(fCurCamera.fHorizonAngle), sin(fCurCamera.fVerticalAngle), cos(fCurCamera.fVerticalAngle) * cos(fCurCamera.fHorizonAngle));
//	glm::vec3 lightInvDir = -viewDir; // ???
//
//	// Compute the MVP matrix from the light's point of view
//	glm::mat4 depthProjectionMatrix = glm::ortho(Camera::fLeft, Camera::fRight, Camera::fBottom, Camera::fTop, Camera::fNear, Camera::fFar);
//	glm::mat4 depthViewMatrix = glm::lookAt(inLight->GetLightPos(0), inLight->GetLightDir(0), glm::vec3(0,1,0));
//	glm::mat4 depthModelMatrix = glm::mat4(1.0);
//	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
//
//	// Send our transformation to the currently bound shader,
//	// in the "MVP" uniform
//	//glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0])
//}

void Camera::ViewDirToSphericalAngles(const glm::vec3 & viewDir, float & horizonAngle, float &verticalAngle)
{
	glm::vec3 normalizedViewDir = glm::normalize(viewDir);
	horizonAngle = atan2(normalizedViewDir.y, normalizedViewDir.x);
	verticalAngle = acos( normalizedViewDir.z / glm::length(normalizedViewDir) );
	
}

void Camera::SphericalAnglesToViewDir(float horizonAngle, float verticalAngle, glm::vec3 & viewDir)
{
	viewDir = glm::vec3(sin(verticalAngle) * cos(horizonAngle), sin(verticalAngle) * sin(horizonAngle), cos(verticalAngle));
}

glm::vec3 Camera::SphericalAnglesToViewDir(float horizonAngle, float verticalAngle)
{
	glm::vec3 viewDir;
	SphericalAnglesToViewDir(horizonAngle, verticalAngle, viewDir);
	return viewDir;
}

#if 0 

namespace VGM
{

	namespace CullFlags{
		enum EFlags
		{
			kCulled = 0,
			kFully = 1,
			kPartially = 2,
			kContained = 3

		};

	}

	// Borrowed from GLSpace.h.
	class Camera
	{
	public:
		Camera() : fProjectionType(0), fSceneGroupID(0), fOcclusionID(std::make_pair(0,0)), fClipShadows(true), fUseClipPlanesToCull(false) {}

		Camera( const glm::dvec3& eyeLoc, const glm::dvec3& ctrLoc, const glm::dvec3& upVec,
			short projectionType, Context sceneGroupID,
			const glm::dvec4& latBounds, const glm::dvec2& nearFar, bool isLeftHanded );


		//TODO: make more of this private.

		glm::dmat4					fViewMatrix;			// world to eye
		glm::dmat4					fProjectionMatrix;		// eye to clip; projection space

		std::vector<glm::dvec4>		fWorldClipPlanes;			// world space - for node culling
		std::vector<glm::vec4>		fEyeClipPlanes;			// cached in eye space

		//TODO: these two might be unused. 
		glm::ivec4					fViewportSize;			// glViewport(left, right, width, height)
		glm::dmat4					fViewportMatrix;		// normalized device coords to view space

		short						fProjectionType;		// ortho, perspective...	

		glm::dvec3					fCameraLocation;
		glm::dvec3					fViewDirection;
		glm::dvec3					fUpVector;
		glm::dvec3					fCenterLocation;
		glm::dvec3					fOrthoCenter;	

		glm::dvec4					fLatBounds;  //Left, Right, Bottom, Top
		glm::dvec2					fNearFarBounds; //Near, Far

		bool						fLeftHanded;

		Context						fSceneGroupID;			// 0 by default. 
		bool						fClipShadows;	//True by default. If false when using this camera to render shadows
		//the shadows will be generated as if the clip planes were not there.

		std::pair<unsigned int, unsigned int> fOcclusionID;	//So we can track occluded nodes from multiple nodes. 


		void ComputeWorldFrustums();
		void ComputeCameraOffset();
		void ComputeCameraProjection();
		void ComputeEyeClipPlanes();
		void ComputeEyeParameters();
		void ComputeNearFarBounds(glm::dvec3 wcenter, glm::dvec3 wextent);
		CullFlags::EFlags CullBoundingBox( glm::dmat4* worldTrans, glm::dvec3 ur, glm::dvec3 ll, bool applyAllCullingPlanes = false);
		glm::dmat4 ComputeAutoRotateCorrection(const glm::dvec3& centroid, const glm::dvec3& center,
			const glm::dvec3& normal, const glm::dmat4& local2world, bool& skipProp) const;

		// return true if point is not visible due to view frustum culling and other clip plane culling
		bool CullWorldPoint(const glm::dvec3 & inWorldPt);
		void CullWithClipPlanes(bool);

		bool operator ==(const Camera & camera) const;	

	private:
		glm::dvec4					fLeftFrustum;  
		glm::dvec4					fRightFrustum;  
		glm::dvec4					fTopFrustum;  
		glm::dvec4					fBottomFrustum;  
		glm::dvec4					fNearFrustum;  
		glm::dvec4					fFarFrustum;  

		//Precompute some absolute values for frustum culling. 
		glm::dvec4					fLeftFrustumAbs;  
		glm::dvec4					fRightFrustumAbs;  
		glm::dvec4					fTopFrustumAbs;  
		glm::dvec4					fBottomFrustumAbs;  
		glm::dvec4					fNearFrustumAbs;  
		glm::dvec4					fFarFrustumAbs;  

		bool						fUseClipPlanesToCull;
	};

} // namespace VGM



#endif