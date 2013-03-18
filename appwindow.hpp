#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>
#include "scene.hpp"
#include "viewer.hpp"

class AppWindow : public Gtk::Window {
public:
  AppWindow();
  void setRootNode(SceneNode* root);
  
protected:

private:
  // A "vertical box" which holds everything in our window
  Gtk::VBox m_vbox;

  // The menubar, with all the menus at the top of the window
  Gtk::MenuBar m_menubar;
  // Each menu itself
  Gtk::Menu m_menu_app;
  Gtk::Menu m_menu_mode;
  Gtk::Menu m_menu_edit;
  Gtk::Menu m_menu_options;
  Gtk::Menu m_menu_picking;

  // The main OpenGL area
  Viewer m_viewer;

  void resetPosition();
  void resetOrientation();
  void resetJoints();
  void resetAll();
  void setMode(int mode);
  void circle();
  void zbuffer();
  void backface();
  void frontface();
  void undo();
  void redo();
  void setPick(int slot);
};

#endif
