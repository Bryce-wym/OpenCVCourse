#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Images ////////////////

Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

void getContours(Mat imgDil, Mat img)  // 寻找图像的边缘
{
	vector<vector<Point>> contours; // 创建轮廓容器
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


		if (area > 1000) // 去除噪点
		{
			double peri = arcLength(contours[i], true);// 计算轮廓周长
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);// 用多边形近似

			cout << conPoly[i].size() << endl;// 输出轮廓由几个点组成
			boundRect[i] = boundingRect(conPoly[i]); // 计算轮廓的垂直边界最小矩形，矩形是与图像上下边界平行的

			// 判断轮廓类型
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


			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);// 绘制近似的多边形轮廓
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);// 绘制矩形框
			putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 255), 1);// 绘制文本
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