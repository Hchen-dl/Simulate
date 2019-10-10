/*

*/

#include "PointsGenerate.h"
#include "Ransac_WG_CH.h" //不在头文件里包含自己写的头文件

/*
@returnn random points of a line

	@int numPoins, number of simulate
	@cv::Vec4f ideaLine, the param of an ideal line,
		which containa a start point and the direction(cos,sin) of this line
	@int step, the ideal distance between two neighbor poins
	@int thresh, the deviation of between the ideal point and the point generated
*/
std::vector<cv::Point> GenerateRandomPoints(int numPoins, cv::Vec4f ideaLine, int step, int thresh)
{
	//随机点的序列
	std::vector<cv::Point> randomPoints;
	//随机数种子
	srand((int)std::time(0));
	//理论点
	double ideal_x, ideal_y;
	//添加随机漂移threh距离后的点
	double x, y;

	if (numPoins <= 0||step<=0)
	{
		std::cout << "WRONG input!!!" << std::endl;
	}
	
	for (int i = 0; i < numPoins; i++)
	{		
		//距离初始点距离
		int lenth_line = step * i;
		//理论点y坐标
		ideal_y = ideaLine[3] + lenth_line;
		if (ideaLine[0] != 0)//直线不与y轴平行
		{
			//理论点x坐标
			ideal_x =ideaLine[2]+ lenth_line * ideaLine[0] / (ideaLine[1]);//x = x_0 + delta * sin_t / cos_t ;
		}
		else ideal_x = ideaLine[2];

		//产生0~2*thresh -thresh即-thresh~thresh之间的数。		//cv::RNG ((ushort)thresh);
		double offset = 2 * thresh*rand() / double(RAND_MAX) - thresh;

		x = ideal_x - offset * ideaLine[1]; //x= ideal_x - offset*sin_t;
		y = ideal_y + offset * ideaLine[0]; //y= ideal_y + offset*cos_t;

		randomPoints.push_back(cv::Point(x, y));
	}

	return randomPoints;
}

/*
@returnn points of simulated line

	@std::vector<cv::Point> nzPoints,
	@int windowSize,


	description:
	for (int i=ahead distance;i<nzPoints;i++) //step?
	{
		imagePoints=nzPoints[i~,i+windowSize]
		lineRansacFit(imagePoints);

	}

*/
std::vector<cv::Point> GeneratePoints(std::vector<cv::Point> nPoints,int step, int windowSize)
{
	//
	static int k = -1;
	std::vector<cv::Point>  nzPoints =ReversePoints(nPoints);
	std::vector<cv::Point> newPoints;
	SLine lineParam = LineFitRANSAC(nzPoints);// .params 包含cost,sint,x,y
	cv::Vec2f mainLine = GetParameter(lineParam.params);

	static double initialDistance = 100;
	int wheelDistance = 200;//后轮中心点距离图像成像中心点为200cm-----200像素
	double x = lineParam.params[2] + initialDistance;
	double y = nzPoints[0].y;
	double cos_t = lineParam.params[0];
	double sin_t = lineParam.params[1];
	if (sin_t < 0)
	{
		sin_t = -sin_t;
		cos_t = -cos_t;
	}
	
	cv::Size sizeWindow(336, 257);//图像窗口大小
	cv::Vec4f wheelCenter(cos_t, sin_t, x, y);//前视方向及后轮中心点,cos,sin,x,y

	
	newPoints.push_back(cv::Point(wheelCenter[2], wheelCenter[3]));//

	std::vector<cv::Point> windowPoints;
	do {
		windowPoints = GetSubPoints(nzPoints, sizeWindow, wheelCenter);
		if(windowPoints.size()>2)
		lineParam = LineFitRANSAC(windowPoints);
		else
		{
			//未找到作物行
			wheelCenter[3] += step;
			continue;
		}
		//if line Param is valid
		cos_t = lineParam.params[0];
		sin_t = lineParam.params[1];
		if (sin_t < 0)
		{
			sin_t = -sin_t;
			cos_t = -cos_t;
		}

		double d = GetDistance(wheelCenter, lineParam.params)- initialDistance;
		x = wheelCenter[2] + wheelDistance * cos_t - d * sin_t + step * cos_t;//d为平移距离 理想情况下瞬间响应,下一个点移动到此时偏差为零的点.
		y = wheelCenter[3] - wheelDistance + wheelDistance * sin_t + d * cos_t + step * sin_t;


		//cv::Vec2f tempParam = GetParameter(lineParam.params);

		wheelCenter = cv::Vec4f(cos_t, sin_t, x, y);
		newPoints.push_back(cv::Point(wheelCenter[2], wheelCenter[3]));//
	} 
	while (wheelCenter[3]+sizeWindow.height<nzPoints.back().y);//变量
	//initialDistance += 0.5;
	
	//补充
	std::vector<cv::Point> endPoints=GenerateRandomPoints((nzPoints.size()-newPoints.size()),wheelCenter,step,5);
	newPoints.insert(newPoints.end(), endPoints.begin(), endPoints.end());
	return newPoints;
}

/*
@return sub points of an input points vec
	@std::vector<cv::Point> nzPoints,
	@cv::Size sizeWindow, 
	@cv::Vec4f wheelCenter---cos,sin,x,y
*/
std::vector<cv::Point> GetSubPoints(std::vector<cv::Point> nzPoints,cv::Size sizeWindow,cv::Vec4f wheelCenter)
{
	std::vector<cv::Point> subPoint;
	//
	//length!=0
	//
	for (int i = 0; i < nzPoints.size(); i++)
	{
		//if nzPoints[i] is in the window, then

		//y-y0=-1/k(x-x0)  *  在窗口上下两条线之间
		bool isInWindow=false;
		//if (1-wheelCenter[1]>=0)//若垂直
		//{
		//if (wheelCenter[1] > 0)
		//{
			if (nzPoints[i].y < wheelCenter[3] + sizeWindow.height && nzPoints[i].y >= wheelCenter[3])
				isInWindow = true;
		//}
		
			
		//}
		//else
		//{
		//	double delta = (nzPoints[i].y - wheelCenter[3] + wheelCenter[0] / wheelCenter[1] * (nzPoints[i].x - wheelCenter[2]))*
		//		(nzPoints[i].y - (wheelCenter[3] + sizeWindow.height*wheelCenter[0])
		//			+ wheelCenter[0] / wheelCenter[1] * (nzPoints[i].x - (wheelCenter[2] + sizeWindow.height*wheelCenter[1])));
		//	if (delta < 0)
		//		isInWindow = true;
		//}
		if(isInWindow)
		subPoint.push_back(nzPoints[i]);
	}
	return subPoint;
}
/*
@return Vec2f of actual offset and angle

	@naviParam, cos,sin,x,y

*/
cv::Vec2f GetParameter(cv::Vec4f naviParam)
{

	return cv::Vec2f();
}

/*
draw points on a image
output the deviations
*/
void DrawPoins(std::vector<cv::Point> nzPoints, cv::Mat showImage)
{
	if (showImage.rows == 0)
		showImage = cv::Mat::ones(cv::Size(1800, nzPoints.size()*2),0);
	int sum = 0;
	for (int i = 0; i < nzPoints.size(); i++)
	{
		cv::circle(showImage, nzPoints[i], 1, cv::Scalar(255), 3);
		sum += nzPoints[i].x;
		//std::cout << "x: " << nzPoints[i].x << "  ";
		//std::cout << "y: " << nzPoints[i].y<< std::endl;
	}
	double mean = sum / (double)nzPoints.size();
	std::cout << "x: " << mean<< "  ";
	SLine lineParam=LineFitRANSAC(nzPoints,5,0.99,0.01,10
	);
	
	double x1=0, y1=0, x2=0, y2=0;
	y1= 0;
	y2 = showImage.rows;

	if (lineParam.params[0] != 0)
	{
		x1 = lineParam.params[2] - lineParam.params[3] *
			lineParam.params[0] / lineParam.params[1];//x_1=x_0-y_0*cos_t/sin_t
		x2 = x1 + y2 * 
			lineParam.params[0] / lineParam.params[1];//x_2=x_1+(y2-y1)*cos_t/sin_t
	}		
	else
	{
		x1 = lineParam.params[2];
		x2 = lineParam.params[2];
	}
	
	cv::line(showImage,cv::Point(x1, y1 ), cv::Point(x2, y2), cv::Scalar(255));

	std::cout << "cos_t:" << lineParam.params[0]
		<<"; sin_t: "<<lineParam.params[1]
		<<"; mean_x: "<<lineParam.params[2] 
		<<"; mean_y: "<<lineParam.params[3] << std::endl;

	std::cout << "numValid:" << lineParam.numOfValidPoints << "  ";
	std::cout << "numTotal:" << nzPoints.size()<<std::endl;

	cv::namedWindow("ShowPoints", cv::WINDOW_FREERATIO);
	cv::imshow("ShowPoints", showImage);
	std::cout << "显示点图像，在图像窗口按键继续……" << std::endl;
	cv::waitKey(10);
}

/*
calculate distance from wheelcenter to navigation line(fitted line)
*/
double GetDistance(cv::Vec4f wheelCenter, cv::Vec4f naviLine)
{
	double distance = 0;
	if (naviLine[0] != 0)
	{
		//联立解方程:x=cos*sin*(y0-y~)+sin*sin*x~+cos*cos*x0
		double x = naviLine[0] * naviLine[1] * (wheelCenter[3] - naviLine[3])
			+ naviLine[1] * naviLine[1] * naviLine[2] +
			naviLine[0] * naviLine[0] * wheelCenter[2];
		//distance=sqrt(((1/k)^2+1)(x-x0)^2)
		distance = sqrt(
			((naviLine[0] / naviLine[1])*(naviLine[0] / naviLine[1]) + 1)*
			(x - wheelCenter[2])*(x - wheelCenter[2]));
	}
	else distance = wheelCenter[2] - naviLine[2];
	return distance;
}

/*
@reverse the points
*/
std::vector<cv::Point> ReversePoints(std::vector<cv::Point> nzPoints)
{
	std::vector<cv::Point>newPoints;
	int length = nzPoints.size();
	for (int i = 0; i < length; i++)
	{
		newPoints.push_back(cv::Point(nzPoints[length-i-1].x,nzPoints[i].y));
	}
	return newPoints;
}
