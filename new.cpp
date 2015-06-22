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

String path = "/home/andrzej/Desktop/vis/4templ_1.jpg";
Mat pattern = imread(path, CV_LOAD_IMAGE_COLOR);
Mat patternGray = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
Mat patternThresh, drawing1Gray, drawing1Thresh, threshQuad, quadGray;

int main(){
	threshold(patternGray, patternThresh, 100, 255,0);

	vector<Vec3f> circles;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(patternThresh,contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	Mat drawing1 = Mat::zeros(patternGray.size(),CV_8UC3);
	for (int i =1; i<contours.size(); i++){
		drawContours(drawing1, contours,i,Scalar(255, 255, 255),-1,8,hierarchy, 0,Point());
	}
	cout<<contours.size()<<"SSS"<<endl;
	cvtColor(drawing1, drawing1Gray, CV_RGB2GRAY);
	threshold(drawing1Gray, drawing1Thresh, 120, 255,0);		
	findContours(drawing1Thresh,contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	
	Mat drawing2 = Mat::zeros(patternGray.size(),CV_8UC3);
	
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());

	vector<kontur> tab;

	String name = "Window #";
	char c = '2';
	String quadr = "quadrilateral #";
	char d = '1';
	String windowName;
	String windowNameQuadr;
	for (int a =0; a<contours.size(); a++){
		kontur asd;
		Scalar color = Scalar(0,255,0);

		mu[a] = moments(contours[a],false);
		mc[a] = Point2f(mu[a].m10/mu[a].m00, mu[a].m01/mu[a].m00);
		
		

		asd.centroidX = mc[a].x;
		asd.centroidY = mc[a].y;
		asd.area = contourArea(contours[a]);
		asd.LmostX=-1;
		asd.LmostY=-1;
		asd.RmostX=-1;
		asd.RmostY=-1;
		asd.TmostX=-1;
		asd.TmostY=-1;
		asd.BmostX=-1;
		asd.BmostY=-1;
		if (asd.area > 700){
			windowName = name + c;
			c++;
			Mat tempWindow(pattern.size(),CV_8UC3, Scalar(0));
			drawContours(tempWindow, contours,a,Scalar(0, 255, 0),2,8,hierarchy, 0,Point());
			uchar* p = tempWindow.data;
			tab.push_back(asd);
			for (int i = 0; i<tempWindow.rows; i++){
				for (int j = 0; j<tempWindow.cols; j++){
					p = tempWindow.data + tempWindow.cols*i+j;
					Vec3b zxc = tempWindow.at<Vec3b>(i,j);
					if (zxc.val[1]==255){
						if (asd.TmostY == -1)
							asd.TmostY = i;
						if (asd.TmostX == -1)
							asd.TmostX = j;
						asd.BmostX = j;
						asd.BmostY = i;
					}
				}
			}
			for (int i = 0; i<tempWindow.cols; i++){
				for (int j =0; j<tempWindow.rows; j++){
					p = tempWindow.data + tempWindow.cols*j+i;
					Vec3b zxc = tempWindow.at<Vec3b>(j,i);
					if (zxc.val[1]==255){
						if (asd.LmostX == -1)
							asd.LmostX = i;
						if (asd.LmostY == -1)
							asd.LmostY = j;
						asd.RmostX = i;
						asd.RmostY = j;
					}
				}
			}
			//cout<<asd.LmostY<<endl<<asd.LmostX<<endl;
			circle( pattern, Point(asd.BmostX, asd.BmostY), 5, Scalar(255,0,0), 3, 8, 0 );
			circle( pattern, Point(asd.TmostX, asd.TmostY), 5, Scalar(0,255,0), 3, 8, 0 );
			circle( pattern, Point(asd.RmostX, asd.RmostY), 5, Scalar(0,0,255), 3, 8, 0 );
			circle( pattern, Point(asd.LmostX, asd.LmostY), 5, Scalar(255,255,2), 3, 8, 0 );
			Point2f pt = Point (asd.LmostX, asd.LmostY);
			asd.corners.push_back(pt);
			pt = Point (asd.TmostX, asd.TmostY);
			asd.corners.push_back(pt);
			pt = Point (asd.RmostX, asd.RmostY);
			asd.corners.push_back(pt);
			pt = Point (asd.BmostX, asd.BmostY);
			asd.corners.push_back(pt);	

			sortCorners(asd.corners, center);
			if (asd.corners.size() == 0){
				cout << "The corners were not sorted correctly!" << endl;
				return -1;

			}
			windowNameQuadr = quadr + d;
			Mat dst = pattern.clone();
			Mat quad = Mat::zeros(320, 320, CV_8UC3);
			vector<Point2f> quad_pts;
			quad_pts.push_back(Point2f(0, 0));
			quad_pts.push_back(Point2f(quad.cols, 0));
			quad_pts.push_back(Point2f(quad.cols, quad.rows));
			quad_pts.push_back(Point2f(0, quad.rows));
			Mat transmtx = getPerspectiveTransform(asd.corners, quad_pts);
			warpPerspective(pattern, quad, transmtx, quad.size());
			cvtColor(quad, quadGray, CV_RGB2GRAY);
			threshold(quadGray, threshQuad, 120, 255,0);
			HoughCircles( threshQuad, circles, CV_HOUGH_GRADIENT, 1,10, 255, 10, threshQuad.cols/2-5, threshQuad.cols/2+5);
			
			if (circles.size() > 0 && circles.size() < 5){
				imshow("image", dst);
				int value  = GetValue(quad);
				string s=NumberToString(value);
				putText( quad, s, Point(160,160), FONT_HERSHEY_SIMPLEX, 1, color, 2,1);
				imshow(windowNameQuadr, quad);
				d++;
			}
		}
	}

	waitKey(0);
	return 0;
}