#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/////////////  Color Detection ////////////////

VideoCapture cap(0);
Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 180, smax = 255, vmax = 255;
/// <summary>
/// HSV(Hue, Saturation, Value)这个模型中颜色的参数分别是：色调（H），饱和度（S），明度（V）
/// opencv中的HSV范围，H是0-180，S是0-255，V是0-255
/// </summary>

void main()
{
	/*string path = "Resources/lambo.png";*/

	Mat img;


	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 180);
	createTrackbar("Hue Max", "Trackbars", &hmax, 180);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true)
	{
		cap.read(img);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		cout << hmin << "," << smin << "," << vmin << "," 
			<< hmax << "," << smax << "," << vmax << endl;
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		//imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);
		waitKey(20);
	}



}