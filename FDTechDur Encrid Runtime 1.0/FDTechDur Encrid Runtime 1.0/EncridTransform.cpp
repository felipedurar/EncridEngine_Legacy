
#define DEVELOPMENT
#include "EncridTransform.h"
#include "EncridPosSize.h"
#include "EncridDevice.h"

#include <gl\GL.h>
#include <gl\GLU.h>

#define TESTDEV(a) if (_dev == NULL) { return(a); } else { _dev->MakeCurrent(); }
#define TESTDEV if (_dev == NULL) { return; } else { _dev->MakeCurrent(); }

TDE_Point _TDE_PropPos(TDE_Device* dev, TDE_Point pos)
{
	FLOAT nw = (2.0 * pos.X) / dev->W;
	FLOAT nh = (2.0 * pos.Y) / dev->H;
	//
	TDE_Point p; p.SetPoint(nw, nh);
	return p;
}

TDE_Size _TDE_PropSize(TDE_Device* dev, TDE_Size sz)
{
	FLOAT nw = (2.0 * sz.W) / dev->W;
	FLOAT nh = (2.0 * sz.H) / dev->H;
	//
	TDE_Size p; p.SetSize(nw, nh);
	return p;
}

TDE_Transform::TDE_Transform() { _dev = NULL; };

TDE_Transform::TDE_Transform(TDE_Device* dev)
{
	_dev = NULL;
	_dev = dev;
}

VOID TDE_Transform::Rotate2D(FLOAT Angle, TDE_Point Position)
{
	TESTDEV;
	TDE_Point ps = _TDE_PropPos(_dev, Position);
	glRotatef(Angle, ps.X, ps.Y, 0.0);
}

VOID TDE_Transform::Translate2D(TDE_Size size)
{
	TESTDEV;
	TDE_Size sz = _TDE_PropSize(_dev, size);
	glTranslatef(sz.W, sz.H, 0.0f);
}

VOID TDE_Transform::Scale2D(TDE_Size size)
{
	TESTDEV;
	TDE_Size sz = _TDE_PropSize(_dev, size);
	glTranslatef(sz.W, sz.H, 0.0f);
	glScalef(sz.W, sz.H, 0.0f);
}

VOID TDE_Transform::Rotate3D(FLOAT Angle, TDE_Coordinate Axis)
{
	TESTDEV;
	glRotatef(Angle, Axis.x, Axis.y, Axis.z);
}



