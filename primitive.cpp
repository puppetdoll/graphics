#include "primitive.hpp"

Primitive::~Primitive()
{
}



Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking) const {
  glNewList(1, GL_COMPILE_AND_EXECUTE);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    gluSphere(qobj, 1.0, 50, 50);
  glEndList();
}

void Cylinder::walk_gl(bool picking) const {
  glNewList(1, GL_COMPILE_AND_EXECUTE);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    gluCylinder(qobj, 1.0, 1.0, 1.0, 50, 50);
  glEndList();
}

