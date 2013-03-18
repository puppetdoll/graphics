#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <gtkglmm.h>
#include "algebra.hpp"
#include <gtkmm.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(bool picking) const = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(bool picking) const;
};

class Cylinder: public Primitive {
public: 
  virtual ~Cylinder();
  
  // Draws cylinder with height 1 and radius 1
  virtual void walk_gl(bool picking) const;
};
#endif
