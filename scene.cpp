#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
  if (m_name == "root") {
    m_id = 1;
  } else if (m_name == "ntorso") {
    m_id = 1;
  } else if (m_name == "nshoulder") {
    m_id = 1;
  } else if (m_name == "nhips") {
    m_id = 2;
  } else if (m_name == "torso") {
    m_id = 3;
  } else if (m_name == "nneck") {
    m_id = 1;
  } else if (m_name == "jneck") {
    m_id = 1;
  } else if (m_name == "neck") {
    m_id = 1;
  } else if (m_name == "nhead") {
    m_id = 2;
  } else if (m_name == "head") {
    m_id = 1;
  } else if (m_name == "larm") {
    m_id = 1;
  } else if (m_name == "rarm") {
    m_id = 2;
  } else if (m_name == "lleg") {
    m_id = 1;
  } else if (m_name == "rleg") {
    m_id = 2;
  } else if (m_name == "nlimb") {
    m_id = 1;
  } else if (m_name == "upperlimb") {
    m_id = 1;
  } else if (m_name == "jmid") {
    m_id = 1;
  } else if (m_name == "nmid") {
    m_id = 1;
  }
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  glPushName(m_id);
  // Apply same transformation to all children
  for (ChildList::const_iterator n = m_children.begin(); n != m_children.end(); n++) {
    (*n)->set_transform(m_trans * (*n)->get_transform());
    (*n)->walk_gl(picking);
    (*n)->set_transform(m_invtrans * (*n)->get_transform());
  }
  glPopName();
}

void SceneNode::rotate(char axis, double angle)
{
  Matrix4x4 r;
  angle = M_PI * (angle / 180); 
  switch(axis) {
    case 'z':{
      double entriesz[16] = {
        cos(angle),   -sin(angle),  0.0,          0.0,
        sin(angle),   cos(angle),   0.0,          0.0,
        0.0,          0.0,          1.0,          0.0,
        0.0,          0.0,          0.0,          1.0};
      r = Matrix4x4((double*)&entriesz);
     break;
    }
    case 'x':{
      double entriesx[16] = {
        1,            0,            0,            0,
        0,            cos(angle),   -sin(angle),  0,
        0,            sin(angle),   cos(angle),   0,
        0,            0,            0,            1};
      r = Matrix4x4((double*)&entriesx);
      break;
    }
    case 'y':{
      double entriesy[16] = {
        cos(angle),   0,            sin(angle),   0,
        0,            1,            0,            0,
        -sin(angle),  0,            cos(angle),   0,
        0,            0,            0,            1};
      r = Matrix4x4((double*)&entriesy);
      break;
    }
  }
  set_transform(m_trans * r);
}

void SceneNode::scale(const Vector3D& amount)
{
  double entries[16] = {
    amount[0],  0,        0,        0,
    0,        amount[1],  0,        0,
    0,        0,        amount[2],  0,
    0,        0,        0,        1};
  set_transform(m_trans * Matrix4x4((double*)&entries));
}

void SceneNode::translate(const Vector3D& amount)
{
  double entries[16] = {
    1,  0,  0,  amount[0],
    0,  1,  0,  amount[1],
    0,  0,  1,  amount[2],
    0,  0,  0,  1};
  set_transform(m_trans * Matrix4x4((double*)&entries));
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  glPushName(m_id);
  for (ChildList::const_iterator n = m_children.begin(); n != m_children.end(); n++) {
    (*n)->set_transform(m_trans * (*n)->get_transform());
    (*n)->walk_gl(picking);
    (*n)->set_transform(m_invtrans * (*n)->get_transform());
  }
  glPopName();
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
  glPushName(m_id);
  glPushMatrix();
  glMultMatrixd(m_trans.transpose().begin());

  m_material->apply_gl();
  m_primitive->walk_gl(picking); 
  glPopMatrix();
  glPopName();
}
