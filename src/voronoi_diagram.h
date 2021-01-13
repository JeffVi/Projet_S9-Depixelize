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


	void union_2vertex(int indice_i, int indice_j);
	void union_3vertex(int indice_i, int indice_j);
	void init_cells();
	void compute_vertex();
	void polygon_union();
	Mat draw_voronoi();
	void draw_cells(Mat& voro);
	
private:
	
	float scale;
	int rows;
	int cols;
	Mat imagep;
	Mat pixels_nodes_todo;
	std::vector<cell> cells;
	std::vector<cell> union_cells;
	
	std::vector<int> get_cell_adj(int i, int j, int k);
};

#endif
