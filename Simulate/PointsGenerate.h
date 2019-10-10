#pragma once
#ifndef POINTS_GENERATE_H
#define POINTS_GENERATE_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <ctime>

//#include <stdlib.h>
#include <iostream>

std::vector<cv::Point> GenerateRandomPoints(
	int numPoins,
	cv::Vec4f ideaLine,
	int step,
	int thresh
);

std::vector<cv::Point> GeneratePoints(
	std::vector<cv::Point> nzPoints,
	int windowSize=257,
	int step=1
);

std::vector<cv::Point> GetSubPoints(
	std::vector<cv::Point> nzPoints,
	cv::Size sizeWindow,
	cv::Vec4f wheelCenter
);

cv::Vec2f GetParameter(
	cv::Vec4f naviParam
);

void DrawPoins(
	std::vector<cv::Point> nzPoints,
	cv::Mat showImage
);

double GetDistance(cv::Vec4f, cv::Vec4f);

std::vector<cv::Point> ReversePoints(
	std::vector<cv::Point> nzPoints
);

/*
calculate the difference between two lines
*/
//

#endif // !POINTS_GENERATE_H

