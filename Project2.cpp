#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Project2 ////////////////

Mat img, imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docsPoints;

float w = 420, h = 596;

Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	//erode(imgDil, imgErode, kernel);

	return imgDil;
}

vector<Point> getContours(Mat imgDil)  // 寻找图像的边缘
{
	vector<vector<Point>> contours; // 创建轮廓容器
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	float maxArea = 0;


	for (int i = 0; i < contours.size(); i++)
	{
		float area = contourArea(contours[i]);
		//cout << area << endl;

		if (area > 1000) // 去除噪点
		{
			float peri = arcLength(contours[i], true);// 计算轮廓周长
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);// 用多边形近似

			if (area > maxArea && conPoly[i].size() == 4) {
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 5);
				biggest = { conPoly[i][0] ,conPoly[i][1],conPoly[i][2],conPoly[i][3] };
				maxArea = area;
			}

			//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);// 绘制近似的多边形轮廓
		}
	}

	return biggest;
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
	}

}

vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;


	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3


	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{

	Point2f scr[4] = { points[0],points[1],points[2],points[3] };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(scr, dst); // 给出变换矩阵
	warpPerspective(img, imgWarp, matrix, Size(w, h));

	return imgWarp;
}


int main()
{
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	//resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	// Processing
	imgThre = preProcessing(imgOriginal);
	// Get Contours - Biggest
	initialPoints = getContours(imgThre);
	//drawPoints(initialPoints, Scalar(0, 0, 255));
	docsPoints = reorder(initialPoints);
	//drawPoints(docsPoints, Scalar(0, 255, 0));

	// Warp
	imgWarp = getWarp(imgOriginal, docsPoints, w, h);

	// Crop
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * 5), h - (2 * 5));
	imgCrop = imgWarp(roi);

	imshow("ImageOriginal", imgOriginal);
	imshow("ImageThre", imgThre);
	imshow("ImageWrap", imgWarp);
	imshow("ImageCrop", imgCrop);
	waitKey(0);

	return EXIT_SUCCESS;
}


