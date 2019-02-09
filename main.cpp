#include "mainwindow.h"
#include "videotracker.h"
#include <gtkmm.h>


int main(int argc, char* argv[])
{
	// Initialize gtkmm user interface
	Gtk::Main app(argc, argv);

	// Create window
	mainwindow w;
	try
	{
		// Main loop
		Gtk::Main::run(w);
		return 0;
	}
	catch(const std::exception&)
	{
	}
}