
#define DEVELOPMENT
#include "EncridShapes.h"
#include "EncridIDGen.h"

//

#include <GL\GL.h>
#include <GL\GLU.h>

TDE_Model* TDE_Shapes::CreatePlane(FLOAT w, FLOAT h)
{
	TDE_Model* Plane = new TDE_Model();
	Plane->UseGLListIs = true;
	Plane->GLListId = TDE_GetID();
	//
	glNewList(Plane->GLListId, GL_COMPILE);
	//
	//glFrontFace(GL_CW);
	glBegin(GL_QUADS);
	    glColor3f(1.0, 1.0, 1.0); glVertex3f(0, 0, 0);
		glColor3f(1.0, 1.0, 1.0); glVertex3f(w, 0, 0);
		glColor3f(1.0, 1.0, 1.0); glVertex3f(w, h, 0);
		glColor3f(1.0, 1.0, 1.0); glVertex3f(0, h, 0);
	glEnd();
	//
	glEndList();
	Plane->lists.push_back(Plane->GLListId);
	return Plane;
}


