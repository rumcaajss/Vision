#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;
int main(){
	Mat OurImage;
	string Destination = "/home/andrzej/Desktop/vis/Lena.png";
	OurImage = imread(Destination, CV_LOAD_IMAGE_GRAYSCALE);
	if(! OurImage.data){
	printf("No image!");
	getchar();
	return -1;
	}
/*Mat edges;
//threshold(OurImage, bin, 125, 255,0);
Canny( OurImage, edges, 50, 200, 3 );
vector<Vec2f> lines;
HoughLines(edges, lines, 1, CV_PI/180,190, 0, 0 );
for( size_t i = 0; i < lines.size(); i++ ){
	float rho = lines[i][0], theta = lines[i][1];
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1000*(-b));
	pt1.y = cvRound(y0 + 1000*(a));
	pt2.x = cvRound(x0 - 1000*(-b));
	pt2.y = cvRound(y0 - 1000*(a));
	line( edges, pt1, pt2, Scalar(255,0,0), 2, CV_AA);
}*/

Mat hist;
int histSize = 256;
float range[] = { 0, 255 };
const float* histRange = { range };
calcHist( &OurImage, 1, 0, Mat(), hist, 1, &histSize, &histRange, 1, 0 );
normalize( hist, hist, 0, 50, NORM_MINMAX, -1, Mat() );
Mat histImage( histSize, 256, CV_8UC3, Scalar( 0,0,0) );
for( int i = 1; i < histSize; i++ )
{
line( histImage,Point(i,255),Point(i,256-hist.at<float>(i)),Scalar(0,200,230),1,8);
}
namedWindow("WINDOW", CV_WINDOW_AUTOSIZE);
imshow("WINDOW",histImage);
waitKey(0);
}