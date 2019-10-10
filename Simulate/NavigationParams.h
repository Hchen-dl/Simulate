#pragma once
#ifndef NAVIGATION_PARAMS_H
#define NAVIGATION_PARAMS_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <vector>

class NavigationParams
{
public:
	NavigationParams() {};
	~NavigationParams() {};

	//���浼����������
	std::vector<cv::Vec2f> naviParams_;
	/*
	@return Vec2f of actual offset and angle

		@naviParam, cos,sin,x,y

	*/
	cv::Vec2f GetParameter(
		cv::Vec4f naviParam
	);



	//�жϵ���ƫ���Ƿ���Ч
	bool IsValid(cv::Vec2f);

private:

};
#endif // !NAVIGATION_PARAMS_H
