#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("Advanced Ergonomics Laboratory");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
 
  m_menu_app.items().push_back(MenuElem("_Reset Position", Gtk::AccelKey("i"),
    sigc::mem_fun(*this, &AppWindow::resetPosition)));
  
  m_menu_app.items().push_back(MenuElem("_Reset Orientation", Gtk::AccelKey("o"),
    sigc::mem_fun(*this, &AppWindow::resetOrientation)));
  
  m_menu_app.items().push_back(MenuElem("_Reset Joints", Gtk::AccelKey("n"),
    sigc::mem_fun(*this, &AppWindow::resetJoints)));
  
  m_menu_app.items().push_back(MenuElem("_Reset All", Gtk::AccelKey("a"),
    sigc::mem_fun(*this, &AppWindow::resetAll)));
     
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  
  // Set up the mode menu
  Gtk::RadioButtonGroup mode_group;
  sigc::slot<void, int> slot_mode = sigc::mem_fun(*this, &AppWindow::setMode);

  m_menu_mode.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(mode_group, "_Position/Orientation", Gtk::AccelKey("p"), sigc::bind(slot_mode, 1)));
  
  m_menu_mode.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(mode_group, "_Joints", Gtk::AccelKey("j"), sigc::bind(slot_mode, 2)));

  // Set up the edit menu
  m_menu_edit.items().push_back(MenuElem("_Undo", Gtk::AccelKey("u"),
    sigc::mem_fun(*this, &AppWindow::undo)));
  
  m_menu_edit.items().push_back(MenuElem("_Redo", Gtk::AccelKey("r"),
    sigc::mem_fun(*this, &AppWindow::redo)));
  

  // Set up the options menu
  m_menu_options.items().push_back(Gtk::Menu_Helpers::CheckMenuElem("_Circle", Gtk::AccelKey("c"),    sigc::mem_fun(*this, &AppWindow::circle)));
  
  m_menu_options.items().push_back(Gtk::Menu_Helpers::CheckMenuElem("_Z-buffer", Gtk::AccelKey("z"),    sigc::mem_fun(*this, &AppWindow::zbuffer)));
  
  m_menu_options.items().push_back(Gtk::Menu_Helpers::CheckMenuElem("_Backface cull", Gtk::AccelKey("b"),    sigc::mem_fun(*this, &AppWindow::backface)));
  
  m_menu_options.items().push_back(Gtk::Menu_Helpers::CheckMenuElem("_Frontface cull", Gtk::AccelKey("f"),    sigc::mem_fun(*this, &AppWindow::frontface)));
  

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Edit", m_menu_edit));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", m_menu_options));
  //
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 400);
  m_vbox.pack_start(m_viewer);
  
  show_all();
}

void AppWindow::resetPosition() {}
void AppWindow::resetOrientation() {}
void AppWindow::resetJoints() {}
void AppWindow::resetAll() {}
void AppWindow::setMode(int mode) {}
void AppWindow::circle() {
  m_viewer.setCircle();  
  m_viewer.invalidate();
}
void AppWindow::zbuffer() {
  m_viewer.setZBuffer();
  m_viewer.invalidate();
}
void AppWindow::backface() {
  m_viewer.setBackFace();
  m_viewer.invalidate();
}
void AppWindow::frontface() {
  m_viewer.setFrontFace();
  m_viewer.invalidate();
}
void AppWindow::undo() {}
void AppWindow::redo() {}
void AppWindow::setRootNode(SceneNode* node) {
  m_viewer.setRootNode(node);
}

  }
}
