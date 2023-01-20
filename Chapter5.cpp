#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Warp Images ////////////////

float w = 250, h = 350;
Mat matrix, imgWarp;

void main()
{
	string path = "Resources/cards.jpg";
	Mat img = imread(path);


	Point2f scr[4] = { {529,142},{771,190},{405,395},{674,457} };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	matrix = getPerspectiveTransform(scr, dst); // ¸ø³ö±ä»»¾ØÕó
	warpPerspective(img, imgWarp, matrix, Size(w, h));

	for (int i = 0; i < 4; i++)
	{
		circle(img, scr[i], 10, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	imshow("ImageWarp", imgWarp);
	waitKey(0);
}