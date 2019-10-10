#pragma once
#ifndef SAVEPOINTS_H
#define SAVEPOINTS_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<fstream>
#include<iostream>

void SavePoint(std::vector<cv::Point> points, std::string fileName);


#endif // !SAVEPOINTS_H
