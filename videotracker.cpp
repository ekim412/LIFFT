

/**
    STRATEGY FOR TRACKING INDIVIDUAL STICKERS
    Motivation: Analysis of a deadlift movement can be broken down into distinct points of interest in order to accept or reject the form of the lift. These points include the barbell, shoulder, mid-back, butt and knee.

    CHECKING NUMBER OF CLICKS
 
    Order of clicks will be categorized into respective arrays. The order of clicks must be as follows:
        1) Bar
        2) Shoulder
        3) Mid-back
        4) Butt
**/


#include "videotracker.h"

using namespace cv;
using namespace std;



std::string video_file;                                                         /**< video file path */

Point2f point;                                                                  /**< point2f structure variable for storing a point */
bool point_clicked = false;                                                     /**< a boolean to deal with whether a new point has been clicked by user */
TermCriteria criteria(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);           /**< termination criteria information for iterative functions used in vmain() */
Size window_size(30,30);                                                        /**< size of window */
const int MAX_COUNT = 4;                                                        /**< maximum number of clicks allowed */

std::vector<Point2f> array_of_vectors[2];                                            /**< create an array of two vectors to hold the points */

/*! \brief mouse_click() is a function used by vmain() to determine where the user has clicked
 *
 *
 *  mouse_click() modifies a point with the new coordinates of x,y, that a user has clicked on with the left button click
 *  It also sets point_clicked as true to signal that a new point has been clicked by the user
 *
 *  @param event event to monitor whether left click button has been clicked or not
 *  @param x x coordinate of clicked point
 *  @param y y coordinate of clicked point
 *  @return no return
 *
 */
void Videotracker::mouse_click( int event, int x, int y, int , void* )
{
    if (event == EVENT_LBUTTONDOWN)                 /**< when left button click */
    {
        point = Point2f((float)x, (float)y);        /**< set value of point to be the appropriate x and y coordinates */
        point_clicked = true;                       /**< point has been clicked */
    }

}


/*! \brief file_select() is a function used for the gui to select a file and provide it to videotracker.cpp 's vmain() function
 *
 *
 *  file_select is the helper method used to receive user's file selection from the gui.
 *  This helper method will return a string of the file path which will then be used to open the video
 *
 *  @param fName filename string given from gui
 *  @return no return
 *
 */
void Videotracker::file_select(std::string fName)
{
    video_file = fName;
}


/*! \brief vmain() function plays the video file in a new window and allows the user to click on the video and track the object clicked on for the remainder of the video. It also draws the curvature of back and travel path of the bar in addition to the desired curvatures and path
 *
 *
 * vmain() opens a window and takes each frame of the input video (given by an absolute path). It shows each of the frames in order and allows the user to click on the video to place a green sticker.
 * The green sticker will follow the object the user has clicked on. It will leave a path for the bar travel and it will show the curvature of the back. Additionally, it shows the desired curvature of the back and the desired bar pattern.
 * vmain() tracks objects by taking each individual frame image, turning it into greyscale, accepting a point (determining the coordinates on the frame) from mouse_click() and using lucas-kanade optical flow algorithms to determine the next position/point of the object in subsequent frames.
 * The user can place multiple stickers.
 *
 * @param no parameters
 * @return returns 0
 *
 */

int Videotracker::vmain()
{
    VideoCapture video(video_file);                                             /**< open the video file */
    namedWindow( "LIFFT", WINDOW_NORMAL );                                      /**< make a window called LIFFT */
    cv::resizeWindow("LIFFT", 700, 700);                                        /**< resize the window */
    setMouseCallback( "LIFFT", mouse_click, 0 );                                /**< mouse_click handler */
    Mat current_grey_image, previous_grey_image, current_colour_image, current_colour_screenshot;       /**< create Mat objects to hold the frames/images */

    int counter = 0;
    vector<Point2f> bar_vector;                                                  /**< vector to store barbell coordinates */
    vector<Point2f> shoulder_vector;                                             /**< vector to store shoulder coordinates */
    vector<Point2f> back_vector;                                                 /**< vector to store back coordinates */
    vector<Point2f> butt_vector;                                                 /**< vector to store butt coordinates */

    while(1){                                                                   /**< loop until broken */

        if(!video.read(current_colour_screenshot))                             /**< if no more frames, then break loop */
            break;

        current_colour_screenshot.copyTo(current_colour_image);                 /**< make the current frame an image */
        cvtColor(current_colour_image, current_grey_image, COLOR_BGR2GRAY);     /**< convert the image to greyscale */

        if(!array_of_vectors[0].empty()){                                          /**< as long as there are images in the first vector */
            vector<uchar> status;
            vector<float> error;
            if(previous_grey_image.empty())                                     /**< if there is no previous grey image then make the previous one the same as the current grey image */
                current_grey_image.copyTo(previous_grey_image);
            calcOpticalFlowPyrLK(previous_grey_image, current_grey_image, array_of_vectors[0], array_of_vectors[1], status, error, window_size, 3, criteria, 0, 0.001);           /**< lucas-kanade algorithm for determining the best next positions for each point and place them into the vector arrays */

            size_t i;

            for(i = 0; i < array_of_vectors[1].size(); i++){                   /**< for loop to iterate through all of the vectors of new/next points */
                if(!status[i]){                                                /**< if the next points havent been found for a point, then for loop again */
                    continue;
                }

                circle( current_colour_image, array_of_vectors[1][i], 3, Scalar(0,255,0), 4, 8);    /**< draw a circle on the current image for the current new point*/


                if (i == 0){                                                    /**< First vector references barbell coordinates */
                    bar_vector.push_back(array_of_vectors[1][i]);                     /**< Add the current next points to the barbell vector */
                    for (size_t k=0;k<bar_vector.size();k++){
                        circle( current_colour_image, bar_vector[k], 3, Scalar(0,255,0), 4, 8);    /**< Draw a circle for all the points in the barbell vector. As k progresses through the loop, the series of circles will display the continuous path of the barbell throughout the movement */
                    }
                }
                if (i == 1){                                                    /**< Second vector references shoulder coordinates */
                    shoulder_vector.push_back(array_of_vectors[1][i]);                /**< Add the current next points to the shoulder vector */
                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0)){ /**< Check if the back vector and butt vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t a = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */
                        size_t b = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */


                        line(current_colour_image, back_vector[a-1], butt_vector[b-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the back and butt. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector and shoulder vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t d = shoulder_vector.size();                       /**< To be used to get most recent coordinates for shoulder sticker (referenced by vectorSize-1) */
                        size_t e = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */

                        line(current_colour_image, shoulder_vector[d-1], back_vector[e-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the shoulder and back. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector, shoulder vector, and butt vector contain coordinates. If any vectors are empty, then some of them may not have been initialized with their original coordinates. Wait until all have been initalized to avoid segmentation fault */
                        size_t f = shoulder_vector.size();                       /**< To be used to get most recent coordinates for shoulder sticker (referenced by vectorSize-1) */
                        size_t g = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */

                        line(current_colour_image, shoulder_vector[f-1], butt_vector[g-1], Scalar(0,0,255), 4, 8);    /**< Draw a line between most recent coordinates for the shoulder and butt for desired curvature. This should display a line following the subject throughout the entire movement */
                    }
                }
                if (i == 2){                                                    /**< Third vector references back coordinates */
                    back_vector.push_back(array_of_vectors[1][i]);                    /**< Add the current next points to the shoulder vector */
                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0)){ /**< Check if the back vector and butt vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t a = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */
                        size_t b = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */


                        line(current_colour_image, back_vector[a-1], butt_vector[b-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the back and butt. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector and shoulder vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t d = shoulder_vector.size();                       /**< To be used to get most recent coordinates for shoulder sticker (referenced by vectorSize-1) */
                        size_t e = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */

                        line(current_colour_image, shoulder_vector[d-1], back_vector[e-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the shoulder and back. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector, shoulder vector, and butt vector all contain coordinates. If any vectors are empty, then some of them may not have been initialized with their original coordinates. Wait until all have been initalized to avoid segmentation fault */
                        size_t f = shoulder_vector.size();                       /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */
                        size_t g = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */

                        line(current_colour_image, shoulder_vector[f-1], butt_vector[g-1], Scalar(0,0,255), 4, 8);     /**< Draw a line between most recent coordinates for the shoulder and butt for desired curvature. This should display a line following the subject throughout the entire movement */
                    }
                }
                if (i == 3){                                                    /**< Fourth vector references butt coordinates */
                    butt_vector.push_back(array_of_vectors[1][i]);                    /**< Add the current next points to the butt vector */
                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0)){ /**< Check if the back vector and butt vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t a = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */
                        size_t b = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */


                        line(current_colour_image, back_vector[a-1], butt_vector[b-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the back and butt. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector and shoulder vector both contain coordinates. If one or both vectors are empty, then either of them may not have been initialized with their original coordinates. Wait until both have been initalized to avoid segmentation fault */
                        size_t d = shoulder_vector.size();                       /**< To be used to get most recent coordinates for shoulder sticker (referenced by vectorSize-1)  */
                        size_t e = back_vector.size();                           /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1)*/

                        line(current_colour_image, shoulder_vector[d-1], back_vector[e-1], Scalar(255,255,0), 4, 8);    /**< Draw a line between most recent coordinates for the shoulder and back. This should display a line following the subject throughout the entire movement */
                    }

                    if  ((back_vector.size() != 0) && (butt_vector.size() != 0) && (shoulder_vector.size() != 0)){     /**< Check if the back vector, shoulder vector, and butt vector all contain coordinates. If any vectors are empty, then some of them may not have been initialized with their original coordinates. Wait until all have been initalized to avoid segmentation fault */
                        size_t f = shoulder_vector.size();                       /**< To be used to get most recent coordinates for back sticker (referenced by vectorSize-1) */
                        size_t g = butt_vector.size();                           /**< To be used to get most recent coordinates for butt sticker (referenced by vectorSize-1) */

                        line(current_colour_image, shoulder_vector[f-1], butt_vector[g-1], Scalar(0,0,255), 4, 8);    /**< Draw a line between most recent coordinates for the shoulder and butt for desired curvature. This should display a line following the subject throughout the entire movement */
                    }
                }
            }
        }


        if(point_clicked)        /**< if left button clicked */
        {
            vector<Point2f> holder;                                                             /**< vector to hold on to newly clicked point */
            holder.push_back(point);                                                            /**< push point onto the vector */
            cornerSubPix( current_grey_image, holder, window_size, Size(-1,-1), criteria);      /**< refine corner locations of point, get pixel-accurate location of point */
            array_of_vectors[1].push_back(holder[0]);                                                /**< push the point onto the array of vectors */

            if (counter==0){                                                    /**< First click to initalize barbell coordinates */
                bar_vector.push_back(point);                                     /**< Add the current coordinates to the barbell vector */

            }
            if (counter==1){                                                    /**< Second click to initalize shoulder coordinates */
                shoulder_vector.push_back(point);                                /**< Add the current coordinates to the shoulder vector */

            }
            if (counter==2){                                                    /**< Third click to initalize back coordinates */
                back_vector.push_back(point);                                    /**< Add the current coordinates to the back vector */

            }
            if (counter==3){                                                    /**< Fourth click to initalize butt coordinates */
                butt_vector.push_back(point);                                    /**< Add the current coordinates to the butt vector */

            }

            counter+=1;                                                         /**< iterate counter to get ready to accept first point into another vector for the next body part */

            point_clicked = false;                                              /**< reset click to false */
        }

        if (bar_vector.size() != 0){                                                                 /**< if there are bar points aka once the bar has been clicked, we need to draw a line vertically from the initial click position as the desired bar path */
            float bottom_x = bar_vector[0].x;                                                        /**< variable for x value of the initial bar point */
            Point2f bottom_of_screen(bottom_x,0);                                                   /**<top of screen */
            Point2f top_of_screen(bottom_x,video.get(CAP_PROP_FRAME_HEIGHT));                       /**<bottom of screen*/
            line(current_colour_image, top_of_screen, bottom_of_screen, Scalar(0,0,255), 4, 8);     /**draw the line */
        }

        imshow("LIFFT", current_colour_image);                                      /**< show the current colour frame image (not grayscale) - the while loop will be fast enough to blend the individual frames into a video */
        char c = (char)waitKey(10);                                                 /**< gives highgui time to process draw requests from imshow */
        swap(array_of_vectors[1], array_of_vectors[0]);                                       /**< swaps the arrays */
        current_grey_image.copyTo(previous_grey_image);                             /**< move current image to be the previous image */
    }
    return 0;
}
