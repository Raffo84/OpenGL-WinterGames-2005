#include "SlalomGate.h"

SlalomGate::SlalomGate(bool isActive) {
	timeOut = SLALOM_GATE_TIMEOUT_VALUE;
	activate(isActive);
	generateDL();
	indicatorDL = 0;
	radius = SLALOM_GATE_WIDTH / 4.0f;
	gateCol =		Color(0.4f, 0.2f, 0.0f);
	indicatorCol =	Color(0.9f, 0.1f, 0.0f);
}

SlalomGate::~SlalomGate(void) {
	glDeleteLists(myDL, 1);
#ifdef _DEBUG
	std::cout << "Loesche Gate von ";
	pos.print();
	std::cout << "\n";
#endif
}

void SlalomGate::setColor(Color col, bool isGateCol) {
	if(isGateCol) {
		gateCol = col;
		generateDL();
	}
	else
		indicatorCol = col;
}

void __fastcall SlalomGate::activate(bool newActive) {
	active = newActive;
	if(newActive)
		timeOut = SLALOM_GATE_TIMEOUT_VALUE;
}

int __fastcall SlalomGate::isActive() const {
	int result = SLALOM_GATE_IS_ACTIVE;
	if(!active)
		result |= SLALOM_GATE_DEACTIVATED;
	if(timeOut <= 0.0f)
		result |= SLALOM_GATE_TIMED_OUT;

	return result;
}

void __fastcall SlalomGate::update(float timeDiff) {
	if(isActive() == SLALOM_GATE_IS_ACTIVE) {
		timeOut -= timeDiff;
		if(timeOut <= 0.0f) {
			timeOut = 0.0f;
		}
	}
}

void SlalomGate::render() const {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

	if(glIsList(myDL))
		glCallList(myDL);

	if(active && indicatorDL) {
		// render indicator for current gate
		glTranslatef(0.0f, SLALOM_GATE_HEIGHT * 1.5f, 0.0f);
		glColor3f(indicatorCol.r, indicatorCol.g, indicatorCol.b);
		glCallList(indicatorDL);
	}

	glPopMatrix();
}

void SlalomGate::generateDL(){
	if(glIsList(myDL))
		glDeleteLists(myDL, 1);
	else {
		myDL = glGenLists(1);
		if(myDL == 0) {
#ifdef _DEBUG
			std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	glNewList(myDL, GL_COMPILE);
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor3f(gateCol.r, gateCol.g, gateCol.b);
		glTranslatef(-SLALOM_GATE_WIDTH / 2.0f, 0.0f, 0.0f);	// y was SLALOM_GATE_HEIGHT / 2.0f
		renderHelpCylinder(2.0f * SLALOM_GATE_HEIGHT);	// double height, to ensure, the gate...
		glTranslatef(SLALOM_GATE_WIDTH, 0.0f, 0.0f);
		renderHelpCylinder(2.0f * SLALOM_GATE_HEIGHT);	// ... is always connected to the ground
		glTranslatef(-SLALOM_GATE_WIDTH / 2.0f, SLALOM_GATE_HEIGHT - 0.1, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		renderHelpCylinder(SLALOM_GATE_WIDTH + 0.2f);
		glTranslatef(-0.4f, 0.0f, 0.0f);
		renderHelpCylinder(SLALOM_GATE_WIDTH + 0.2f);
		glPopMatrix();
		if(isEnTex) glEnable(GL_TEXTURE_2D);
	glEndList();
}

void SlalomGate::renderHelpCylinder(float height) const {
	float lod = GLOBAL_LEVEL_OF_DETAIL;
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= lod; i++) {	// lod + 1 Durchgänge, um den Zylinder zu schließen.
		float x, z;
		x = 0.1f * cos(i * (2.0f*PI / lod));
		z = 0.1f * sin(i * (2.0f*PI / lod));
		glNormal3f(0.5f*x,  0.0f, 0.5f*z);
		glVertex3f(x, -height / 2.0f, z);
		glVertex3f(x,  height / 2.0f, z);
	}
	glEnd();
}

bool __fastcall SlalomGate::operator &&(Actor *other) {
	if(other == this)
		return true;
	if(other == NULL)
		return false;

	Vector3D distVec = other->getPosition() - pos;
	float dist = distVec.length();
	dist -= radius;
	dist -= other->getRadius();
	if(dist >= 0.0f) return false;

	Matrix3D mat;
	mat.createRotationMatrix(0.0f, -rot.y, 0.0f);
	distVec = mat * distVec;
	if(distVec.z <= 0.1) return true;
	return false;
}