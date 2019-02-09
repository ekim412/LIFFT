#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "videotracker.h"


// The class mainwindow inherits from Gtk::Window
class mainwindow : public Gtk::Window
{
	// Constructor and destructor
public:    
	std::string filename = "";
	mainwindow();
	virtual					~mainwindow();



protected:
	void                    on_button_load();

	//Member widgets:
	Gtk::Image		image;
	Gtk::Button             buttonLoad;
	Gtk::Button             quitButton;
	Gtk::Grid               mainGrid;

};


#endif // MAINWINDOW_H