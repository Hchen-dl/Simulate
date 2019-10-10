#include "SavePoints.h"

void SavePoint(std::vector<cv::Point> points,std::string fileName)
{
	std::ofstream outfile;
	outfile.open( fileName);
	std::vector<cv::Point>::iterator it;
	for (it = points.begin(); it != points.end(); it++)
	{
		outfile << *it<<std::endl;
	}
	outfile.close();
}
