#ifndef SPLINE_H
#define SPLINE_H

#include <opencv2/opencv.hpp>

using namespace cv;

class polygon
{
public:
	
	polygon();
	
	std::vector<Point> vertex;
	
};


class spline
{
public:
	
	spline();
	spline(std::vector<Point> vertex_list);
	
	std::vector<Point> vertex;
	std::vector<polygon> holes;
	polygon spline_poly;
	
	void find_holes();
	
private:

	std::vector<Point>::iterator it_p1;
	std::vector<Point>::iterator it_p2;
	
	bool find_edge(std::vector<Point>::iterator it_1, std::vector<Point>::iterator it_2);
};

#endif
