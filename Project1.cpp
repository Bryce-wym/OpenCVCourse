#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project1 ////////////////

Mat img;
vector<vector<int>> newPoints; // 存储绘制点的坐标及颜色，颜色用序号表示


// hmin<<smin<<vmin<<hmax<<smax<<vmax
// 设置需要检测到的颜色
vector<vector<int>> myColors{ {42,37,0,62,142,255},     // green
							  {111,39,0,135,104,247} };// blue

/// <summary>
/// 绘制时对应的颜色
/// scalar表示图像颜色时，单通道图像用下标[0]即可，三通道图像用下标[0]、[1]、[2]分别表示B、G、R通道
/// Scalar(255,0,0)  效果等同于 { 255,0,0 }  
/// </summary>
vector<Scalar> mycolorValue{ {0,255,0},   // green 序号0
							 {255,0,0} }; // blue 序号1


Point getContours(Mat imgDil)  // 寻找图像的边缘
{
	vector<vector<Point>> contours; // 创建轮廓容器
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		//cout << area << endl;

		if (area > 1000) // 去除噪点
		{
			double peri = arcLength(contours[i], true);// 计算轮廓周长
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);// 用多边形近似

			//cout << conPoly[i].size() << endl;// 输出轮廓由几个点组成
			boundRect[i] = boundingRect(conPoly[i]); // 计算轮廓的垂直边界最小矩形，矩形是与图像上下边界平行的

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);// 绘制矩形框
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);// 绘制近似的多边形轮廓
		}
	}

	return myPoint;
}

vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint = getContours(mask);

		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x,myPoint.y,i });
		}
	}
	return newPoints;
}


void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> mycolorValue)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, mycolorValue[newPoints[i][2]], FILLED);
	}
}


int main()
{
	VideoCapture cap(0);

	while (true)
	{
		cap.read(img);
		newPoints = findColor(img);
		drawOnCanvas(newPoints, mycolorValue);
		imshow("Image", img);
		waitKey(1);
	}

	return EXIT_SUCCESS;
}