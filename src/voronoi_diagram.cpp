#include "voronoi_diagram.h"

cell::cell() : px(0.0f), py(0.0f) {}

cell::cell(float x, float y) : px(x), py(y) {}

voronoi::voronoi(){}

voronoi::voronoi(Mat image ,Mat pixels_nodes, float diagram_scale)
{
	rows = image.rows;
    	cols = image.cols;
	imagep = image;
	pixels_nodes_todo = pixels_nodes;
	scale = diagram_scale;
}

void voronoi::init_cells()
{
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			cell c(i*scale + scale/2 , j*scale + scale/2);
			c.vertex.push_back(Point(j*scale,i*scale)); 		// haut gauche
			c.vertex.push_back(Point((j+1)*scale,i*scale)); 	// haut droite
			c.vertex.push_back(Point((j+1)*scale,(i+1)*scale)); 	// bas droite
			c.vertex.push_back(Point(j*scale,(i+1)*scale)); 	// bas gauche
			cells.push_back(c);
		}
	}
}


void voronoi::polygon_union()
{


	for(int i = 0;i < rows-1; i++)
	{
		for (int j = 0; j < cols-1; j++)
		{
			bool have_adj = false; 

			if (cells[i,j].united == false)
			{
				if (pixels_nodes_todo.at<int>(i, j, 2) == 1) have_adj = true;

				if (pixels_nodes_todo.at<int>(i, j, 3) == 1) have_adj = true;

				if (pixels_nodes_todo.at<int>(i, j, 4) == 1) have_adj = true;

				union_cells.push_back(cell(cells[i, j].px, cells[i, j].py));
			}

			while(have_adj)
			{ 			
				//TODO 					
			}
			/*std::vector<cell>::iterator it;
			for (it = cells.begin(); it != cells.end(); it++)
			{

			}*/
		}	
	}
}
