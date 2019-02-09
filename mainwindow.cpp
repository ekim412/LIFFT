#include "mainwindow.h"
#include <iostream>


// Constructor for the main Window in order to build the user interface
mainwindow::mainwindow()
{
	// Initialize the main window and hide the title bar
	this->set_border_width(10);

	image.set("logo.png");
	mainGrid.attach(image,0,0,2,1);
	
	// Add the load button
	buttonLoad.add_label("Load file");
	buttonLoad.set_size_request(250,75);
	buttonLoad.signal_pressed().connect(sigc::mem_fun(*this,&mainwindow::on_button_load));
	mainGrid.attach(buttonLoad,0,1,2,1);

	// Add the Quit button
	quitButton.add_label("Quit");
	quitButton.set_size_request(250,75);
	quitButton.signal_pressed().connect(sigc::mem_fun(*this,&mainwindow::close));
	mainGrid.attach(quitButton,0,2,2,1);

	// Display the main grid in the main window
	mainGrid.show_all();

	// Insert the grid in the main window
	add(mainGrid);
}


// Destructor of the class
mainwindow::~mainwindow()
{}


void mainwindow::on_button_load()
{
	// Create the dialog box FileChooser
	Gtk::FileChooserDialog dialog("Please choose a file",Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	// Present dialog box and wait for user response:
	int response = dialog.run();

	// Handle the user response:
	switch(response)
	{
	case(Gtk::RESPONSE_CANCEL):
	{
		// User cancel the operation
		std::cout << "Cancel clicked." << std::endl;
		break;
	}
	case(Gtk::RESPONSE_OK):
	{
		// The user selected a file
		filename = dialog.get_filename();
		std::cout << "Open clicked." << std::endl;
		Videotracker v;
		v.file_select(filename);
		v.vmain();
		break;
	}
	default:
	{
		// If the user closes the dialog box
		std::cout << "Unexpected button clicked." << std::endl;
		break;
	}
	}
}
