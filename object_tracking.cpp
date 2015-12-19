#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	cv::Mat sourceImage;
    sourceImage = cv::imread("pic.jpg"); // Load a gray scale picture.
    if (!sourceImage.data)
        exit(1); //can't open, then exit

	//Blue object
	int iLowH = 101;
	int iHighH = 112;

	int iLowS = 130; 
	int iHighS = 255;

	int iLowV = 74;
	int iHighV = 163;

	Mat imgHSV;
	cvtColor(sourceImage, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
	Mat imgThresholded;
	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (removes small holes from the foreground)
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	//Calculate the moments of the thresholded image
	Moments oMoments = moments(imgThresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	int posX = dM10 / dArea;
	int posY = dM01 / dArea;
	double pAngle = 0.5 * atan2( 2 * oMoments.mu11, oMoments.mu20 - oMoments.mu02 );

	cout<<"Block centroid: ("<< posX <<", "<<posY<<") \n Angle: "<<pAngle<<endl;

	//drawing
	circle( sourceImage, Point2d( posX, posY ), 100, Scalar(255, 0, 0) );
	Point2d x0 = Point2d( 0, posY - tan( pAngle ) * posX );
	Point2d x1 = Point2d( sourceImage.cols, posY + tan(pAngle) * (sourceImage.cols - posX ));
	line( sourceImage, x0, x1, Scalar(255,0,0) );

	//imshow("Thresholded Image", imgThresholded); //show the thresholded image
	imshow("Original", sourceImage); //show the original image

	waitKey(0);

	return 0;
}