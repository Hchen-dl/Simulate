//#include "stdafx.h"

#include "Ransac_WG_CH.h"
#include "PointsGenerate.h"

int main()
{

	//��0�еĵ�
	std::vector<cv::Point> line_0_Points;

	//��ʼ������
	int numPoints = 1600;// 80�ף�ÿ��10��points?
	cv::Vec4f lineParam(0,1,20,0);
	int step = 5;
	int threshError = 5;

	//
	line_0_Points = GenerateRandomPoints(numPoints, lineParam,step,threshError);
	cv::Mat showImage = cv::Mat::ones(cv::Size(600, line_0_Points.size() * 2), 0);
	DrawPoins(line_0_Points, showImage);

	//step = 10;
	////��һ�еĵ�
	//std::vector<cv::Point> line_1_Points;
	//int step = 10;//?
	std::vector<cv::Point> line_1_Points = GeneratePoints(line_0_Points, step);
	DrawPoins(line_1_Points, showImage);


	std::vector<cv::Point> line_2_Points = GeneratePoints(line_1_Points, step);
	DrawPoins(line_2_Points, showImage);

	std::vector<cv::Point> line_3_Points = GeneratePoints(line_2_Points, step);
	DrawPoins(line_3_Points, showImage);

	std::vector<cv::Point> line_4_Points = GeneratePoints(line_3_Points, step);
	DrawPoins(line_4_Points, showImage);

	std::vector<cv::Point> line_5_Points = GeneratePoints(line_4_Points, step);
	DrawPoins(line_5_Points, showImage);

	return 0;
}

