#ifndef VORONOI_DIAGRAM_H
#define VORONOI_DIAGRAM_H

#include <opencv2/opencv.hpp>

using namespace cv;

class cell
{
public:

	cell();
	cell(float x, float y);
	cell(std::vector<Point>  T_vertex, Scalar T_color);

	float px;
	float py;
	bool united = false;

	std::vector<Point> vertex;
	Scalar color;
	std::vector<int> id_vertex;

	int get_id_vertex_origin(int k);
	int get_update_id_vertex_origin(int k);
};

class voronoi
{
public:

	voronoi();
	voronoi(Mat image, Mat pixels_nodes, float diagram_scale);

	void polygon();
	bool have_segment(cell cellule);
	void init_cells();
	void compute_vertex();
	bool compare_color(Scalar color1, Scalar color2);
	Mat draw_voronoi();
	void draw_cells(Mat& voro);
	void draw_union_cells(Mat& voro);

private:

	float scale;
	int rows;
	int cols;
	Mat imagep;
	Mat pixels_nodes_todo;
	std::vector<cell> cells;
	std::vector<cell> polygons;

	std::vector<int> get_cell_adj(int i, int j, int k);
};

#endif
