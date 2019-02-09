/// Temporary design pattern - will create class in later iterations of code

#ifndef VIDEOTRACKER_H
#define VIDEOTRACKER_H
#include "mainwindow.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>
using namespace std;
using namespace cv;

class Videotracker{
private:
	int mouse_clicked_count = 0;
	/// function used by main() to determine position of mouse click
	static void mouse_click( int event, int x, int y, int , void* );
public:
	void file_select(std::string fName);
	int vmain();
	
};
#endif
