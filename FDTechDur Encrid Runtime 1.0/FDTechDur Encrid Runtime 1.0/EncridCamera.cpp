
#define DEVELOPMENT

#include "EncridCamera.h"

TDE_Camera::TDE_Camera()
{
	CameraMode = TDE_CAMERA_ORTHOGRAPHIC;
	FOV = 60.0000f;
	Near = 1.0000f;
	Far = 1000.0000f;
	AspectRatio = 0.0000f;
	CameraPosition = new TDE_Coordinate(0.0000f, 0.0000f, 0.0000f);
	LookAt = new TDE_Coordinate(0.0000f, 0.0000f, 1.0000f);
	UpVector = new TDE_Coordinate(0.0000f, 1.0000f, 0.0000f);
}

