#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {

    cout<<CV_VERSION<<endl;
    cv::Mat sourceImage, modImage;
    sourceImage = cv::imread("pic.jpg"); // Load a gray scale picture.
    if (!sourceImage.data)
        exit(1); //can't open, then exit
    
    namedWindow("Modified", WINDOW_AUTOSIZE);
    cv::imshow( "Source", sourceImage );
    //modImage = sourceImage.clone();
    
    
    int iLowH = 92;
    int iHighH = 124;
    
    int iLowS = 0;
    int iHighS = 255;
    
    int iLowV = 0;
    int iHighV = 255;
    
    //Create trackbars in "Control" window
    
    createTrackbar("LowH", "Modified", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Modified", &iHighH, 179);
    
    createTrackbar("LowS", "Modified", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Modified", &iHighS, 255);
    
    createTrackbar("LowV", "Modified", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Modified", &iHighV, 255);
    
    while(true)
    {
        cvtColor(sourceImage, modImage, COLOR_BGR2HSV);
        
        //cv::imshow( "Modified", modImage );
        //waitKey();
        
        
        
        cv::Mat imgThresholded;
        //cv::imshow( "src", modImage );
        
        //imgThresholded = modImage.clone();
        
        
        inRange(modImage, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
        
        
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        
        cv::imshow( "Modified", imgThresholded );
        
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }
    //waitKey();
    
    return 0;
}

//modifications
//cv::fastNlMeansDenoising( modImage, modImage, 12, 7, 21 ); //removes noise
//cv::GaussianBlur( modImage, modImage, cv::Size( 3, 3 ), 0, 0 ); //blurry
//cv::threshold(modImage, modImage, 128, 255, cv::THRESH_BINARY); //black or white

/*
 //find each contour in the picture
 std::vector< std::vector< cv::Point > > contours;
 std::vector<cv::Vec4i> hierarchy;
 findContours( modImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
 
 //calculate moment for each contour
 vector<Moments> allMoments( contours.size() );
 for( size_t i = 0; i < contours.size(); i++ )
 {
 allMoments[i] = moments( contours[i], true );
 }
 
 //calculate centroid & angle for each contour
 vector<Point2d> allCentroids( contours.size() );
 vector<double> pAngle( contours.size() );
 for( size_t i = 0; i < contours.size(); i++ )
 {
 allCentroids[i] = Point2d( allMoments[i].m10 / allMoments[i].m00, allMoments[i].m01 / allMoments[i].m00 );
 pAngle[i] = 0.5 * atan2( 2*allMoments[i].mu11, allMoments[i].mu20 - allMoments[i].mu02 );
 cout<<allCentroids[i] << ' ' << pAngle[i] * (180 / 3.14) << endl; //prints the results
 }
 
 /// Draw circle for centroid and line for principle angle
 Mat drawing = Mat::zeros( modImage.size(), CV_8UC3 );
 for( size_t i = 0; i< contours.size(); i++ )
 {
 circle( sourceImage, allCentroids[i], 100, Scalar(255, 0, 0) );
 Point2d x0 = Point2d( 0, allCentroids[i].y - tan( pAngle[i] ) * allCentroids[i].x );
 Point2d x1 = Point2d( modImage.cols, allCentroids[i].y + tan(pAngle[i]) * (modImage.cols - allCentroids[i].x ));
 line( sourceImage, x0, x1, Scalar(255,0,0) );
 }
 
 cv::imshow( "Modified", sourceImage ); //show the image in window
 waitKey();
 */
