#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Images ////////////////

Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

void getContours(Mat imgDil, Mat img)  // Ѱ��ͼ��ı�Ե
{
	vector<vector<Point>> contours; // ������������
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string objectType;

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		cout << area << endl;


		if (area > 1000) // ȥ�����
		{
			double peri = arcLength(contours[i], true);// ���������ܳ�
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);// �ö���ν���

			cout << conPoly[i].size() << endl;// ��������ɼ��������
			boundRect[i] = boundingRect(conPoly[i]); // ���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�

			// �ж���������
			int objCor = (int)conPoly[i].size();
			if (objCor == 3) { objectType = "Tri"; }
			if (objCor == 4)
			{
				float aspRatio = (float)(boundRect[i].width) / boundRect[i].height;
				cout << aspRatio << endl;
				cout << aspRatio << endl;
				if (aspRatio > 0.95 && aspRatio < 1.05) { objectType = "Square"; }
				else { objectType = "Rect"; }
			}
			if (objCor > 4) { objectType = "Circle"; }


			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);// ���ƽ��ƵĶ��������
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);// ���ƾ��ο�
			putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 255), 1);// �����ı�
		}
	}
}

void main()
{
	string path = "Resources/shapes.png";
	Mat img = imread(path);

	// Preprocessing
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);


	getContours(imgDil, img);

	imshow("Image", img);
	//imshow("Image Gray", imgGray);
	//imshow("Image Blur", imgBlur);
	//imshow("Image Canny", imgCanny);
	//imshow("Image Dilation", imgDil);

	waitKey(0);
}