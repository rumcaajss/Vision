#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace cv;
using namespace std;

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
};


Mat pattern = imread("/home/andrzej/Desktop/rumcajs2.png", CV_LOAD_IMAGE_COLOR);
Mat patternGray = imread("/home/andrzej/Desktop/rumcajs2.png", CV_LOAD_IMAGE_GRAYSCALE);
Mat patternBlurred, patternThresh, readyToFind;

int main(){
	Scalar color = Scalar(0,255,0);
	GaussianBlur( patternGray, patternBlurred, Size(3, 3), 0 );
	namedWindow("Window", CV_WINDOW_AUTOSIZE);
	threshold(patternGray, patternThresh, 100, 255,0);
	//adaptiveThreshold(patternBlurred, adaThresh,130,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);  
	bitwise_not(patternGray, readyToFind);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	findContours(patternThresh,contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	
	Mat drawing = Mat::zeros(patternGray.size(),CV_8UC3);
	
	for(int i = 1; i < contours.size(); i++){
		Scalar color = Scalar(0,255,0);
		drawContours(drawing, contours,i,color,-1,8,hierarchy, 0,Point());
	}	
	
	cout<<contours.size()<<endl;
	bitwise_not(patternGray, readyToFind);
	Mat dupa, dupa2, dupa3;
	//dupa3 = dupa2.clone();
	Mat dupamiliontemp;
	Mat dupafinal;
	// first convert the image to grayscale
	cvtColor(drawing, dupa, CV_RGB2GRAY);

	// then adjust the threshold to actually make it binary
	threshold(dupa, dupa2, 100, 255, CV_THRESH_BINARY);
	
	findContours(dupa2,contours,hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cout<<contours.size()<<"a"<<endl;
	Mat dupamilion = Mat::zeros(dupa2.size(),CV_8UC3);
	Mat dupamilion1 = Mat::zeros(dupa2.size(),CV_8UC3);
	Mat finalnadupa = Mat::zeros(dupa2.size(),CV_8UC3);
	
	for(int i = 0; i < contours.size(); i++){
		//Scalar color = Scalar(0,255,0);
		drawContours(dupamilion, contours,i,color,2,8,hierarchy, 0,Point());
	}
			
	// first convert the image to grayscale
	cvtColor(dupamilion, dupamiliontemp, CV_RGB2GRAY);

	// then adjust the threshold to actually make it binary
	threshold(dupamiliontemp, dupafinal, 30, 255, CV_THRESH_BINARY);	
	String path;
	String windowName = "lol";
	char b='2';

	
		cout<<"kontury:"<<contours.size()<<endl;
	vector<Moments> mu(contours.size());
	for(int i = 0;i<contours.size(); i++){
		mu[i] = moments(contours[i],false);
	}
	vector<Point2f> mc(contours.size());
	for(int i = 0;i<contours.size(); i++){
		mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
		cout<<mc[i].x<<endl;
		cout<<mc[i].y<<endl<<endl;
	}
	
	kontur tab[contours.size()];
	uchar* p = dupafinal.data;

	for (int a =0; a<contours.size(); a++){
		windowName +=b;
		namedWindow(windowName, CV_WINDOW_AUTOSIZE);
		Mat tempWindow = Mat::zeros(patternGray.size(),CV_8UC3);
		drawContours(tempWindow, contours,a,color,2,8,hierarchy, 0,Point());
		tab[a].centroidX = mc[a].x;
		tab[a].centroidY = mc[a].y;
		tab[a].LmostX=-1;
		tab[a].LmostY=-1;
		tab[a].RmostX=-1;
		tab[a].RmostY=-1;
		tab[a].TmostX=-1;
		tab[a].TmostY=-1;
		tab[a].BmostX=-1;
		tab[a].BmostY=-1;
		for (int i = 0; i<tempWindow.rows; i++){
			for (int j = 0; j<tempWindow.cols; j++){
				p = tempWindow.data + tempWindow.cols*i+j;
				if (*p==255){
					if (tab[a].TmostY == -1)
						tab[a].TmostY = i;
					if (tab[a].TmostX == -1)
						tab[a].TmostX = j;
					tab[a].BmostX = j;
					tab[a].BmostY = i;
				}
			}
		}

		for (int i = 0; i<tempWindow.cols; i++){
			for (int j = 0; j<tempWindow.rows; j++){
				p = tempWindow.data + tempWindow.cols*j+i;
				if (*p==255){
					if (tab[a].LmostX == -1)
						tab[a].LmostX = i;
					if (tab[a].LmostY == -1)
						tab[a].LmostY = j;
					tab[a].RmostX = i;
					tab[a].RmostY = j;
				}
			}
		}
		
	circle( tempWindow, Point(tab[1].BmostX, tab[1].BmostY), 5, Scalar(255,255,255), 3, 8, 0 );
	circle( tempWindow, Point(tab[1].TmostX, tab[1].TmostY), 5, Scalar(255,255,255), 3, 8, 0 );
	circle( tempWindow, Point(tab[1].RmostX, tab[1].RmostY), 5, Scalar(255,255,255), 3, 8, 0 );
	circle( tempWindow, Point(tab[1].LmostX, tab[1].LmostY), 5, Scalar(255,255,255), 3, 8, 0 );
	imshow(windowName, tempWindow);
	//Vec3b p=tempWindow.at<Vec3b>(tab[1].LmostX,tab[1].RmostY);
	//cout<<int(p.val[0])<<endl;
	}
	imshow("Window", dupafinal);
	waitKey(0);
	return 0;
}