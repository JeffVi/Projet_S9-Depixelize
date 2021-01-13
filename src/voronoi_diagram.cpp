#include "voronoi_diagram.h"

cell::cell() : px(0.0f), py(0.0f) {}

cell::cell(float x, float y) : px(x), py(y) {}

int cell::get_id_vertex_origin(int k)
{
	int id;
	
	switch(k)
	{
		// haut droite
		case 1:
		id = id_vertex[1];
		break;
		
		// bas droite	
		case 3:
		id = id_vertex[2];
		break;
		
		// bas gauche
		case 5:
		id = id_vertex[3];
		break;
		
		// haut droite	
		case 7:
		id = id_vertex[0];
		break;
	}
	
	if(id == -1){std::cout<<"cross not solved !"<<std::endl;}
	return id;
}

int cell::get_update_id_vertex_origin(int k)
{
	int id;
	
	switch(k)
	{
		// haut droite
		case 1:
		id = id_vertex[1];
		id_vertex[1] = -1;
		id_vertex[2]++;
		id_vertex[3]++;
		break;
		
		// bas droite	
		case 3:
		id = id_vertex[2];
		id_vertex[2] = -1;
		id_vertex[3]++;
		break;
		
		// bas gauche
		case 5:
		id = id_vertex[3];
		id_vertex[3] = -1;
		break;
		
		// haut droite	
		case 7:
		id = id_vertex[0];
		id_vertex[0] = -1;
		id_vertex[1]++;
		id_vertex[2]++;
		id_vertex[3]++;
		break;
	}
	
	if(id == -1){std::cout<<"vertex already changed !"<<std::endl;}
	return id;
}

voronoi::voronoi(){}

voronoi::voronoi(Mat image ,Mat pixels_nodes, float diagram_scale)
{
	rows = image.rows;
    	cols = image.cols;
	imagep = image;
	pixels_nodes_todo = pixels_nodes.clone();
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
			c.color = imagep.at<Vec3b>(i,j);
			c.id_vertex.push_back(0);
			c.id_vertex.push_back(1);
			c.id_vertex.push_back(2);
			c.id_vertex.push_back(3);
			cells.push_back(c);
		}
	}
}

void voronoi::compute_vertex()
{
	Mat cells_pixels_nodes = pixels_nodes_todo.clone();
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			
			for(int k=0;k<4;k++)
			{
				if(cells_pixels_nodes.at<int>(i,j,2*k+1) == 1) //seulement les diagonales
				{
					std::vector<int> voisin = get_cell_adj(i,j,2*k+1);
					int vi = voisin[0];
					int vj = voisin[1];
					int vk = voisin[2];
					int vi_adj1 = voisin[3];
					int vj_adj1 = voisin[4];
					int vk_adj1 = voisin[5];
					int vi_adj2 = voisin[6];
					int vj_adj2 = voisin[7];
					int vk_adj2 = voisin[8];
					
					
				}
			}			
		}
	}
}

std::vector<int> voronoi::get_cell_adj(int i, int j, int k)
{
	//Cellule voisine en diagonale
	int vi;
	int vj;
	int vk;
	
	//Cellule adjacente 1
	int vi_adj1;
	int vj_adj1;
	int vk_adj1;
	
	//Cellule adjacente 2
	int vi_adj2;
	int vj_adj2;
	int vk_adj2;
	
	std::vector<int> voisins;
	
	case 1:
		vi = i-1;
		vj = j+1;
		vk = 5;
		vi_adj1 = i-1;
		vj_adj1 = j;
		vk_adj1 = 3;
		vi_adj2 = i;
		vj_adj2 = j+1;
		vk_adj2 = 7;
		break;
				
		case 3:
		vi = i+1;
		vj = j+1;
		vk = 7;
		vi_adj1 = i;
		vj_adj1 = j+1;
		vk_adj1 = 5;
		vi_adj2 = i+1;
		vj_adj2 = j;
		vk_adj2 = 1;
		break;
		
		case 5:
		vi = i+1;
		vj = j-1;
		vk = 1;
		vi_adj1 = i+1;
		vj_adj1 = j;
		vk_adj1 = 7;
		vi_adj2 = i;
		vj_adj2 = j-1;
		vk_adj2 = 3;
		break;
				
		case 7:
		vi = i-1;
		vj = j-1;
		vk = 3;
		vi_adj1 = i;
		vj_adj1 = j-1;
		vk_adj1 = 1;
		vi_adj2 = i-1;
		vj_adj2 = j;
		vk_adj2 = 5;
		break;
	}
	
	voisins.push_back(vi);
	voisins.push_back(vj);
	voisins.push_back(vk);
	voisins.push_back(vi_adj1);
	voisins.push_back(vj_adj1);
	voisins.push_back(vk_adj1);
	voisins.push_back(vi_adj2);
	voisins.push_back(vj_adj2);
	voisins.push_back(vk_adj2);
	
	return voisins;
}

Mat voronoi::draw_voronoi()
{
	Mat voro = Mat(rows*ceil(scale),cols*ceil(scale),CV_8UC3,Scalar(0));
	std::vector<cell>::iterator it;
	
	for(it=cells.begin(); it!=cells.end(); it++)
	{
		cell cellule = *it;
		std::vector<Point> vertex_list = cellule.vertex;
		const int npt = vertex_list.size();
		const Point* ppt = &vertex_list[0];
		fillPoly(voro,&ppt,&npt,1,cellule.color);
	}
		
	return voro;
}
