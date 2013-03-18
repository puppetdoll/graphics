#include "material.hpp"
#include <gtkglmm.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  glEnable(GL_COLOR_MATERIAL);
  GLfloat ks[] = {m_ks.R(), m_ks.G(), m_ks.B()};
  GLfloat kd[] = {m_kd.R(), m_kd.G(), m_kd.B()};
  GLfloat s[] = {m_shininess};
  glLightfv(GL_LIGHT0, GL_SPECULAR, ks);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, kd);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, s);
}
