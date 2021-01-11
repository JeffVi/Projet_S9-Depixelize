#ifndef SIMILARITY_GRAPH_H
#define SIMILARITY_GRAPH_H

#include <opencv2/opencv.hpp>

using namespace cv;

class similarity_graph
{
public:

	similarity_graph();
	similarity_graph(Mat image);
	
	Mat pixels_nodes;
	Mat image_YUV;
	
	void init_graph();
	void compare_YUV();
	
private:
	
	int rows;
	int cols;
	Mat imagep;
	
	std::tuple<int, int, int> get_voisin(int i, int j, int k);

};

#endif
