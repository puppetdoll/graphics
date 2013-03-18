#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

Viewer::Viewer()
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);
  root = NULL;
  selected = NULL;
  zbuffer = false;
  backface = false;
  frontface = false;
  kpressed[0] = false;
  kpressed[1] = false;
  kpressed[2] = false;
  Matrix4x4 m;
  m_stack.push(m);
  pickName = "larm";
  mode = 0;

}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );
  glEnable(GL_DEPTH_TEST);
  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  if (backface && !frontface) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else if (frontface && !backface) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
  } else if (!frontface && !backface) {
    glDisable(GL_CULL_FACE);
  } else {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_AND_BACK);
  }

  std::cout << "begin drawing" << std::endl;
  std::cout << get_width() << "," << get_height() << std::endl;
  // Set up for perspective drawing 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);
  
  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (!m_stack.empty() && !kpressed[0] && !kpressed[1] && !kpressed[2]) {
    glMultMatrixd(m_stack.top().transpose().begin());
  }
  glMultMatrixd(transform.transpose().begin());
  // Clear framebuffer
  if (zbuffer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  } else {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  // Set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);

  // Draw stuff
  if (root != NULL) {
    root->walk_gl();
  }
   // draw_trackball_circle();

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();
  std::cout << "finish drawing" << std::endl;

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode
  
  glMatrixMode(GL_MODELVIEW);

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  kpressed[event->button-1] = true;
  last_pos[0] = event->x;
  last_pos[1] = event->y;
  transform = m_stack.top();
  selected = NULL;
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  kpressed[event->button-1] = false;
  m_stack.push(transform);
  transform = Matrix4x4();
  selected = NULL;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  mode = 2;
  if (mode == 1) {
    if (kpressed[0]) {
      transform = translation(Vector3D((event->x - last_pos[0])*0.1, (last_pos[1] - event->y)*0.1, 0)) * transform;
    }

    if (kpressed[2]) {
      if (event->x - last_pos[0] != 0) {
        transform = rotation((event->x - last_pos[0]) * 0.1, 'z')* transform;
      }
      if (event->y - last_pos[1] != 0) {
        transform = rotation((last_pos[1] - event->y) * 0.1, 'y') * transform;
      }
    }

    if (kpressed[1]) {
      transform = translation(Vector3D(0, 0, (last_pos[1] - event->y) * 0.1)) * transform;
    } 
  } else if (mode == 2) {
    if (selected == NULL) {
      selected = findNode(root, pickName);
    }
    if (pickName == "rarm" || pickName == "larm") {
      Matrix4x4 m = selected->get_transform();
      selected->set_transform(translation(Vector3D(0, -1.5, 0)) * m);
      selected->set_transform(rotation((last_pos[1] - event->y), 'x') * selected->get_transform());
      selected->set_transform(translation(Vector3D(0, 1.5, 0)) * selected->get_transform());
    } else if (pickName == "rleg" || pickName == "lleg") {
      Matrix4x4 m = selected->get_transform();
      selected->set_transform(translation(Vector3D(0, 0, 0)) * m);
      selected->set_transform(rotation((last_pos[1] - event->y), 'x') * selected->get_transform());
      selected->set_transform(translation(Vector3D(0, 0, 0)) * selected->get_transform());
    } else if (pickName == "rarmjmid" || pickName == "larmjmid`") {
      Matrix4x4 m = selected->get_transform();
      selected->set_transform(translation(Vector3D(0, 0, 0)) * m);
      selected->set_transform(rotation((last_pos[1] - event->y), 'x') * selected->get_transform());
      selected->set_transform(translation(Vector3D(0, 0, 0)) * selected->get_transform());
    }
  }

  last_pos[0] = event->x;
  last_pos[1] = event->y;
  invalidate();
  return true;
}

void Viewer::setPick(std::string name) {
  selected = NULL;
  pickName = name;
}

SceneNode* Viewer::findNode(SceneNode * n, std::string name) {
  if (n->isName(name)) {
    return n;
  }
  std::list<SceneNode*> children = n->getChildList();
  for (std::list<SceneNode*>::const_iterator n = children.begin(); n != children.end(); n++) {
    SceneNode * node = findNode((*n), name);
    if (node != NULL) {
      return node;
    }
  }

  return NULL;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();
  
  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  
  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball 
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ? 
    (float)current_width * 0.25 : (float)current_height * 0.25;

  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}

void Viewer::setRootNode(SceneNode * node) {
  root = node;
}

void Viewer::undo() {
  if (!m_stack.empty()) {
    r_stack.push(m_stack.top());
    m_stack.pop();
  }
}

void Viewer::redo() {
  if (!r_stack.empty()) {
    m_stack.push(r_stack.top());
    r_stack.pop();
  } 
}

void Viewer::resetPosition() {
  Matrix4x4 m;
  m_stack.push(m);
}

Matrix4x4 Viewer::rotation(double angle, char axis)
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
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 Viewer::translation(const Vector3D& displacement)
{
  double entries[16] = {
    1,  0,  0,  displacement[0],
    0,  1,  0,  displacement[1],
    0,  0,  1,  displacement[2],
    0,  0,  0,  1};
  return Matrix4x4((double*)&entries);
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 Viewer::scaling(const Vector3D& scale)
{
  double entries[16] = {
    scale[0],  0,        0,        0,
    0,        scale[1],  0,        0,
    0,        0,        scale[2],  0,
    0,        0,        0,        1};
  return Matrix4x4((double*)&entries);
}

