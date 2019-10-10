#ifndef RANSAC_WG_CH_H
#define RANSAC_WG_CH_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <list>
#include <math.h>
#include<numeric>
#define PI 3.14159;

struct SLine
{
	SLine() :
		numOfValidPoints(0),
		params(-1.f, -1.f, -1.f, -1.f)
	{}
	cv::Vec4f params;//(cos(t), sin(t), X0, Y0)
	int numOfValidPoints;
};

/*
@return SLine
{
	cv::Vec4f params;//(cos(t), sin(t), X0, Y0)
	int numOfValidPoints;
}
	@std::vector<cv::Point>& nzPoints//inputPoints
	@float t,//distance from main line
	@float p,//chance of hitting a valid pair
	@float e,//percentage of outliers
	@int T,//number of expected minimum inliers
*/
SLine LineFitRANSAC(
	std::vector<cv::Point>& nzPoints,
	float t = 5,
	float p=0.9,
	float e=0.1,
	int T=10
);

/*
@return Vec4f{
	cos, //variance x of points on line
	sin, //variance y of points on line
	x, //mean value x of points on line
	y, //mean value y of points on line
}

	@std::vector<cv::Point>& nzPoints //inputPoints
	@vector<int> ptOnLine //whether this point is on the line
*/
cv::Vec4f TotalLeastSquares(
	std::vector<cv::Point>& nzPoints,
	std::vector<int> ptOnLine);

#endif // !RANSAC_WG_CH_H