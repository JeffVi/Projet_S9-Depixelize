#ifndef VORONOI_DIAGRAM_H
#define VORONOI_DIAGRAM_H

#include <opencv2/opencv.hpp>

using namespace cv;

class cell
{
public:
	
	cell();
	cell(float x, float y);
	
	float px;
	float py;
	std::vector<Point> vertex;
	Scalar color;
	std::vector<int> id_vertex;
};

class voronoi
{
public:

	voronoi();
	voronoi(Mat image ,Mat pixels_nodes, float diagram_scale);
	
	void init_cells();
	void compute_vertex();
	Mat draw_voronoi();
	
private:
	
	float scale;
	int rows;
	int cols;
	Mat imagep;
	Mat pixels_nodes_todo;
	std::vector<cell> cells;
	
	std::vector<int> get_cell_adj(int i, int j, int k);
};

#endif
