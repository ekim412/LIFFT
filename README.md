# LIFFT
### Set-Up:
LIFFT uses OpenCV, a library of programming functions aimed at real-time computer vision. Download the latest stable version of OpenCV from here: https://www.opencv.org/releases.html

Once you have downloaded the latest stable version you should be able to compile the cpp source code files. 

Compile all cpp source code files, namely: main.cpp, mainwindow.cpp, mainwindow.h, videotracker.cpp, and videotracker.h. This can be done with the following code in the command line:
g++ mainwindow.cpp mainwindow.h main.cpp videotracker.cpp videotracker.h -o LIFFT pkg-config gtkmm-3.0 --cflags --libs opencv

### Running the program

Once compilation is complete there will be an executable file named ‘LIFFT’. This is the LIFFT application and you will be able to load a video file and to track how vertically aligned your deadlift is. Run ‘LIFFT’ by typing "./LIFFT" in the command line and a GUI will pop up.

![Alt text](/Screenshots/LIFFT_GUI.png)

Select 'Load File' and find and select the mp4 file you would like to analyze to begin the video analysis of the deadlift movement. 

![Alt text](/Screenshots/LIFFT_fileupload.png)

Begin by clicking the sticker on the bar, then the sticker on the shoulder, then the sticker on the back, and finally the sticker on the butt. Otherwise, select 'Quit' to close the application.

Below are some screenshots to show how it should look:

![Alt text](/Screenshots/LIFFT_video1.png)

![Alt text](/Screenshots/LIFFT_video2.png)
