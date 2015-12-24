#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>

using namespace cv;
using namespace std;

vector<Vec3d> translatePosition( vector<Vec3d> positions ){
	double scalarX = -0.046;
	double scalarY = 2.509;
	vector<Vec3d> pos;

	for( int i = 0; i < 4; i++ ){
		pos.push_back( Vec3d( positions[i][0]*scalarX,
			positions[i][1]*scalarY, positions[i][2] * 180 / 3.141519265359 ) );
	}
	return pos;
}

vector<Vec3d> calcPos( string path){
	cv::Mat sourceImage,src_gray, modImage;
    int thresh = 100;
    int max_thresh = 255;
    RNG rng(12345);
    
    sourceImage = cv::imread(path.c_str()); // Load a gray scale picture.
    if (!sourceImage.data)
        exit(1); //can't open, then exit

    cv::Rect crop(44,0,470,310);
    cv::Mat srcImg = sourceImage(crop);
    
    cvtColor( srcImg, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Detect edges using canny
    Canny( src_gray, modImage, thresh, thresh*2, 3 );
    /// Find contours
    findContours( modImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
	//calc moments

	vector<Moments> mom;
	for( int j = 0; j< contours.size(); j++){
		mom.push_back(moments(contours[j]));
	}

	
    /// Draw contours
    Mat drawing = Mat::zeros( modImage.size(), CV_8UC3 );
	//namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//namedWindow( "Src", CV_WINDOW_AUTOSIZE );

	vector<Vec3d> centroids;
    for( int i = 0; i< contours.size(); i++ )
    {
		cout<<mom[i].m00<<endl;
		if(mom[i].m00>50){
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		}
		//centroid
		Vec3d tempC = Vec3d( mom[i].m10 / mom[i].m00, mom[i].m01 / mom[i].m00,
			 0.5 * atan2( 2 * mom[i].mu11, mom[i].mu20 - mom[i].mu02 ) );
		bool add = true;
		for( int j = 0; j < centroids.size(); j++){
			double distX = abs(centroids[j][0] - tempC[0]);
			double distY = abs( centroids[j][1] - tempC[1] );
			if( pow(distX, 2) + pow(distY, 2) < 50){
				add = false;
			}

		}
		if( add && mom[i].m00>50 ) {
			centroids.push_back( tempC );

			//drawing
			/*circle( srcImg, Point2d( tempC[0], tempC[1] ), 100, Scalar(255, 0, 0) );
			Point2d x0 = Point2d( 0, tempC[1] - tan( tempC[2] ) * tempC[0] );
			Point2d x1 = Point2d( srcImg.cols, tempC[1] + tan(tempC[2]) * (srcImg.cols - tempC[0] ));
			line( srcImg, x0, x1, Scalar(255,0,0) );
			cout<<"add"<<endl;*/
		}
		//imshow( "Contours", drawing );
	//imshow( "Src", srcImg );
		//waitKey();
    }
	cout<<"Size: "<< centroids.size() <<endl;

	return translatePosition(centroids);
}

int main( int argc, char** argv )
{
	vector<Vec3d> pos = calcPos( "lights_pic.jpg" );
	for( int i = 0; i<4; i++){
		cout<<pos[i]<<endl;
	}
	system("PAUSE");
	return 0;
}