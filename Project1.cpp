#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project1 ////////////////

Mat img;
vector<vector<int>> newPoints; // �洢���Ƶ�����꼰��ɫ����ɫ����ű�ʾ


// hmin<<smin<<vmin<<hmax<<smax<<vmax
// ������Ҫ��⵽����ɫ
vector<vector<int>> myColors{ {42,37,0,62,142,255},     // green
							  {111,39,0,135,104,247} };// blue

/// <summary>
/// ����ʱ��Ӧ����ɫ
/// scalar��ʾͼ����ɫʱ����ͨ��ͼ�����±�[0]���ɣ���ͨ��ͼ�����±�[0]��[1]��[2]�ֱ��ʾB��G��Rͨ��
/// Scalar(255,0,0)  Ч����ͬ�� { 255,0,0 }  
/// </summary>
vector<Scalar> mycolorValue{ {0,255,0},   // green ���0
							 {255,0,0} }; // blue ���1


Point getContours(Mat imgDil)  // Ѱ��ͼ��ı�Ե
{
	vector<vector<Point>> contours; // ������������
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		//cout << area << endl;

		if (area > 1000) // ȥ�����
		{
			double peri = arcLength(contours[i], true);// ���������ܳ�
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);// �ö���ν���

			//cout << conPoly[i].size() << endl;// ��������ɼ��������
			boundRect[i] = boundingRect(conPoly[i]); // ���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);// ���ƾ��ο�
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);// ���ƽ��ƵĶ��������
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