#include "Camera.h"


Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}


void Camera::SetCamera(glm::vec3 eyePos)
{
	fEyePos = eyePos;
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