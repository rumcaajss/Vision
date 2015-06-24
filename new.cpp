#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <sstream>

using namespace cv;
using namespace std;
Point2f center(0,0);

template <typename T>
string NumberToString (T Number)
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }
void sortCorners(vector<Point2f> corners, Point2f center)
{
	vector<Point2f> top, bot;

	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	corners.clear();
	
	if (top.size() == 2 && bot.size() == 2){
		Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];
		
		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
	}
}
int GetValue(Mat img){

	float height = 320;
	float width = 320;
	float h4 = height/4;
	float w4 = width/4;

	Point characteristicPoints[8];
	Point valuesPoints[8];
	int pointValues[8][3];
	int segmentValues[8];
	
	int whichSegment;
	Vec3b point;
	int codedValue = 0;
	threshold(img, img, 100, 255,0);

	characteristicPoints[0].x = width/2;
	characteristicPoints[0].y = height/2-h4;
	
	characteristicPoints[1].x = width/2+w4;
	characteristicPoints[1].y = height/2-h4;

	characteristicPoints[2].x = width/2+w4;
	characteristicPoints[2].y = height/2;
	
	characteristicPoints[3].x = width/2+w4;
	characteristicPoints[3].y = height/2+h4;
	
	characteristicPoints[4].x = width/2;
	characteristicPoints[4].y = height/2+h4;
	
	characteristicPoints[5].x = width/2-w4;
	characteristicPoints[5].y = height/2+h4;
	
	characteristicPoints[6].x = width/2-w4;
	characteristicPoints[6].y = height/2;
	
	characteristicPoints[7].x = width/2-w4;
	characteristicPoints[7].y = height/2-h4;

	for (int i=0; i<8; i++){
		point = img.at<Vec3b>(characteristicPoints[i]);
		if (int(point.val[2]) > 120 && int(point.val[1])<70)
			whichSegment = i;
	}

	for (int i=0; i<8; i++){
		valuesPoints[i] = characteristicPoints[(i+whichSegment)%8];
		point = img.at<Vec3b>(valuesPoints[i]);
	}

	for (int i=0; i<8; i++){
		point = img.at<Vec3b>(valuesPoints[i]);
		pointValues[i][0] = int(point.val[0]);
		pointValues[i][1] = int(point.val[1]);
		pointValues[i][2] = int(point.val[2]);
	}
	
	//check color/value of the center
	point = img.at<Vec3b>(width/2, height/2);
	if (int(point.val[1] > 120))
		codedValue +=1;

	for (int i = 0; i<8; i++){
		if (pointValues[i][0]>120){
			segmentValues[i]=1;
			codedValue += pow(2, i);
		}
		else
			segmentValues[i]=0;
	}
	cout<<endl<<"The coded value is:"<<codedValue<<endl;
	return codedValue;
}

class kontur{
public:
	int LmostX;
	int LmostY;
	int RmostX;
	int RmostY;
	int TmostX;
	int TmostY;
	int BmostX;
	int BmostY;
	float centroidX;
	float centroidY;
	float area;
	vector <Point2f> corners;
};

String path = "/home/andrzej/Desktop/vis/side.jpg";
Mat pattern = imread(path, CV_LOAD_IMAGE_COLOR);
Mat patternGray = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
Mat patternThresh, threshQuad, quadGray, patternBlurred, filtered;

int main(){
	Mat kernel = getStructuringElement( MORPH_RECT, Size( 4, 4 ));
	morphologyEx( patternGray, filtered, 0, kernel );
	GaussianBlur( filtered, filtered, Size(7,7), 0 );
	threshold(filtered, patternThresh, 125,255,0);
	vector<Vec3f> circles;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(patternThresh,contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	Mat drawing1 = Mat::zeros(patternGray.size(),CV_8UC3);
	for (int i =0; i<contours.size(); i++){
		drawContours(drawing1, contours,i,Scalar(0, 255, 0),1,8,hierarchy, 0,Point());
	}
	imshow("Contours", drawing1);
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());
	vector<kontur> tab;

	String quadr = "quadrilateral #";
	char d = '1';
	String windowNameQuadr;
	Mat dst = pattern.clone();
	for (int a =0; a<contours.size(); a++){
		kontur contur;
		Scalar color = Scalar(0,255,0);

		mu[a] = moments(contours[a],false);
		mc[a] = Point2f(mu[a].m10/mu[a].m00, mu[a].m01/mu[a].m00);
		
		contur.centroidX = mc[a].x;
		contur.centroidY = mc[a].y;
		contur.area = contourArea(contours[a]);
		contur.LmostX=-1;
		contur.LmostY=-1;
		contur.RmostX=-1;
		contur.RmostY=-1;
		contur.TmostX=-1;
		contur.TmostY=-1;
		contur.BmostX=-1;
		contur.BmostY=-1;

		Mat tempWindow(pattern.size(),CV_8UC3, Scalar(0));
		drawContours(tempWindow, contours,a,Scalar(0, 255, 0),2,8,hierarchy, 0,Point());
		uchar* p = tempWindow.data;
		tab.push_back(contur);
		for (int i = 0; i<tempWindow.rows; ++i){
			for (int j = 0; j<tempWindow.cols; ++j){
				p = tempWindow.data + tempWindow.cols*i+j;
				Vec3b zxc = tempWindow.at<Vec3b>(i,j);
				if (zxc.val[1]==255){
					if (contur.TmostY == -1)
						contur.TmostY = i;
					if (contur.TmostX == -1)
						contur.TmostX = j;
					contur.BmostX = j;
					contur.BmostY = i;
				}
			}
		}
		for (int i = 0; i<tempWindow.cols; ++i){
			for (int j =0; j<tempWindow.rows; ++j){
				p = tempWindow.data + tempWindow.cols*j+i;
				Vec3b zxc = tempWindow.at<Vec3b>(j,i);
				if (zxc.val[1]==255){
					if (contur.LmostX == -1)
						contur.LmostX = i;
					if (contur.LmostY == -1)
						contur.LmostY = j;
					contur.RmostX = i;
					contur.RmostY = j;
				}
			}
		}
			
		circle( pattern, Point(contur.BmostX, contur.BmostY), 5, Scalar(255,0,0), 3, 8, 0 );
		circle( pattern, Point(contur.TmostX, contur.TmostY), 5, Scalar(0,255,0), 3, 8, 0 );
		circle( pattern, Point(contur.RmostX, contur.RmostY), 5, Scalar(0,0,255), 3, 8, 0 );
		circle( pattern, Point(contur.LmostX, contur.LmostY), 5, Scalar(255,255,2), 3, 8, 0 );
		Point2f pt = Point (contur.LmostX, contur.LmostY);
		contur.corners.push_back(pt);
		pt = Point (contur.TmostX, contur.TmostY);
		contur.corners.push_back(pt);
		pt = Point (contur.RmostX, contur.RmostY);
		contur.corners.push_back(pt);
		pt = Point (contur.BmostX, contur.BmostY);
		contur.corners.push_back(pt);
				
		sortCorners(contur.corners, center);
		if (contur.corners.size() == 0){
			cout << "The corners were not sorted correctly!" << endl;
			return -1;
		}

		windowNameQuadr = quadr + d;
		imshow("Pattern", pattern);
		Mat quad = Mat::zeros(320, 320, CV_8UC3);
		vector<Point2f> quad_pts;
		quad_pts.push_back(Point2f(0, 0));
		quad_pts.push_back(Point2f(quad.cols, 0));
		quad_pts.push_back(Point2f(quad.cols, quad.rows));
		quad_pts.push_back(Point2f(0, quad.rows));
		Mat transmtx = getPerspectiveTransform(contur.corners, quad_pts);
		warpPerspective(pattern, quad, transmtx, quad.size());
		cvtColor(quad, quadGray, CV_RGB2GRAY);
		GaussianBlur( quadGray, quadGray, Size(7,7), 0 );
		threshold(quadGray, threshQuad, 120, 255,0);
		HoughCircles( threshQuad, circles, CV_HOUGH_GRADIENT, 1,50, 255, 10, threshQuad.cols/2-7, threshQuad.cols/2+7);
		if (circles.size() > 0 && circles.size() < 15){
			int value  = GetValue(quad);
			string s=NumberToString(value);
			putText( dst, s, Point(contur.centroidX,contur.centroidY), FONT_HERSHEY_SIMPLEX, 1, color, 2,1);
			imshow(windowNameQuadr, quad);
			imshow("Image with values", dst);
			d++;
		}
	}
	waitKey(0);
	return 0;
}