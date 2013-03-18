#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "scene.hpp"
#include <stack>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>


// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();
  void setZBuffer() {zbuffer = !zbuffer;}
  void setBackFace() {backface = !backface;}
  void setFrontFace() {frontface = !frontface;}
  void setCircle() {circle = !circle;}
  void setRootNode(SceneNode * node);
  void undo();
  void redo();
  void resetPosition();
  void setPick(std::string name);

protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

  // Draw a circle for the trackball, with OpenGL commands.
  // Assumes the context for the viewer is active.
  void draw_trackball_circle();
  
private:
  int mode;
  bool zbuffer;
  bool backface;
  bool frontface;
  bool circle;
  SceneNode * selected;
  SceneNode * root;
  bool kpressed[3];
  Point2D last_pos;
  std::stack<Matrix4x4> m_stack;
  std::stack<Matrix4x4> r_stack;
  std::string pickName;
  Matrix4x4 transform;
  SceneNode * findNode(SceneNode * n, std::string name);
  Matrix4x4 rotation(double angle, char axis);
  Matrix4x4 translation(const Vector3D& displacement);
  Matrix4x4 scaling(const Vector3D& scale);
};

#endif
