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

	//保存导航参数序列
	std::vector<cv::Vec2f> naviParams_;
	/*
	@return Vec2f of actual offset and angle

		@naviParam, cos,sin,x,y

	*/
	cv::Vec2f GetParameter(
		cv::Vec4f naviParam
	);



	//判断导航偏差是否有效
	bool IsValid(cv::Vec2f);

private:

};
#endif // !NAVIGATION_PARAMS_H
