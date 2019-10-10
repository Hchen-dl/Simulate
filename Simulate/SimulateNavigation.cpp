//#include "stdafx.h"

#include "Ransac_WG_CH.h"
#include "PointsGenerate.h"
#include "SavePoints.h"


int main()
{

	//��0�еĵ�
	std::vector<cv::Point> line_0_Points;

	//��ʼ������
	int numPoints = 1600;// 80�ף�ÿ��10��points?
	cv::Vec4f lineParam(0,1,20,0);
	int step = 5;
	int threshError = 10;

	//
	line_0_Points = GenerateRandomPoints(numPoints, lineParam,step,threshError);
	cv::Mat showImage = cv::Mat::ones(cv::Size(1600, line_0_Points.size() * 2), 0);
	DrawPoins(line_0_Points, showImage);

	//step = 10;
	////��һ�еĵ�
	//std::vector<cv::Point> line_1_Points;
	//int step = 10;//?
	
	std::vector<std::vector<cv::Point>> line_Points;
	line_Points.push_back(line_0_Points);
	std::string fileName;
	for (int i = 1; i < 40; i++)
	{
		fileName = ".///..//points//"+ std::to_string(i)+".txt";
		std::vector<cv::Point> line_1_Points = GeneratePoints(line_Points[i-1], step);
		//todo save points.
		line_Points.push_back(line_1_Points);
		SavePoint(line_1_Points,fileName);
		DrawPoins(line_1_Points, showImage);
	}

	cv::waitKey(10);
	return 0;
}

