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
					
					cells_pixels_nodes.at<int>(i,j,2*k+1) = 0;
					cells_pixels_nodes.at<int>(vi,vj,vk) = 0;
					
					//Cellule courante
					cell cell_cour = cells[i*cols + j];
					std::vector<Point> vertex = cell_cour.vertex;
					int vertex_id = cell_cour.get_update_id_vertex_origin(2*k+1);
					Point P1 = Point(cell_cour.py,cell_cour.px);
					
					//Cellule voisine
					cell cell_vois = cells[vi*cols + vj];
					std::vector<Point> vertex_voisin = cell_vois.vertex;
					int vertex_id_voisin = cell_vois.get_update_id_vertex_origin(vk);
					Point P2 = Point(cell_vois.py,cell_vois.px);
					
					//Cellule adjacente 1
					cell cell_adj1 = cells[vi_adj1*cols + vj_adj1];
					std::vector<Point> vertex_voisin_adj1 = cell_adj1.vertex;
					int vertex_id_voisin_adj1 = cell_adj1.get_id_vertex_origin(vk_adj1);
					Point P3 = Point(cell_adj1.py,cell_adj1.px);
					if(vertex_id_voisin_adj1==-1){break;}
					
					//Cellule adjacente 2
					cell cell_adj2 = cells[vi_adj2*cols + vj_adj2];
					std::vector<Point> vertex_voisin_adj2 = cell_adj2.vertex;
					int vertex_id_voisin_adj2 = cell_adj2.get_id_vertex_origin(vk_adj2);
					Point P4 = Point(cell_adj2.py,cell_adj2.px);
					if(vertex_id_voisin_adj2==-1){break;}
					
					//Compute vertex
					Point midpoint = P1 + ((P2 - P1)/2.0f);
					Point vertex1 = midpoint + ((P3 - midpoint)/2.0f);
					Point vertex2 = midpoint + ((P4 - midpoint)/2.0f);
					
					//Update vertex
					vertex[vertex_id] = vertex2;
					vertex.insert(vertex.begin() + vertex_id, vertex1);
					
					vertex_voisin[vertex_id_voisin] = vertex1;
					vertex_voisin.insert(vertex_voisin.begin() + vertex_id_voisin, vertex2);
					
					vertex_voisin_adj1[vertex_id_voisin_adj1] = vertex1;
					vertex_voisin_adj2[vertex_id_voisin_adj2] = vertex2;
					
					//Update cellule
					cell_cour.vertex = vertex;
					cell_vois.vertex = vertex_voisin;
					cell_adj1.vertex = vertex_voisin_adj1;
					cell_adj2.vertex = vertex_voisin_adj2;
					
					cells[i*cols + j] = cell_cour;
					cells[vi*cols + vj] = cell_vois;
					cells[vi_adj1*cols + vj_adj1] = cell_adj1;
					cells[vi_adj2*cols + vj_adj2] = cell_adj2;
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
	
	switch(k)
	{
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

void voronoi::draw_cells(Mat& voro)
{
	std::vector<cell>::iterator it;
	voro = voro + Scalar(100,100,100);//pour mieux voir les lignes noires
	
	for(it=cells.begin(); it!=cells.end(); it++)
	{
		cell cellule = *it;
		std::vector<Point> vertex_list = cellule.vertex;
		const int npt = vertex_list.size();
		const Point* ppt = &vertex_list[0];
		polylines(voro,&ppt,&npt,1,true,Scalar(0,0,0));
	}
}

void voronoi::polygon()
{
	std::vector<cell>::iterator it;
	for (it = cells.begin(); it != cells.end(); it++)
	{


		std::cout << "test" << std::endl;

		bool segment = have_segment(*it);
		if (!segment)
		{

			cell T_cell = *it;
			polygons.push_back(T_cell);
			


		}

	}
	std::cout << "end" << std::endl;

}

bool voronoi::have_segment(cell cellule )
{

	std::vector<cell>::iterator it_poly;
	int cpt = 0;

	if (polygons.size() == 0)return false;

	bool have_segment = false;

	for (it_poly = polygons.begin(); it_poly != polygons.end(); it_poly++)
	{
		std::vector<Point>::iterator it_vertcell;

		bool same_color = compare_color(polygons[cpt].color,cellule.color);

		if (same_color) {
			//int indice_vertcell = 0;
			for (it_vertcell = cellule.vertex.begin(); it_vertcell != cellule.vertex.end(); it_vertcell++)
			{
				//indice_vertcell ++;
				//int indice_vertpoly = 0;
				Point current_vertcell = *it_vertcell;
				std::vector<Point>::iterator it_vertpoly;


				for (it_vertpoly = polygons[cpt].vertex.begin(); it_vertpoly != polygons[cpt].vertex.end(); it_vertpoly++)
				{

					//indice_vertpoly++;
					Point current_vertpoly = *it_vertpoly;

					
					if (current_vertcell == current_vertpoly)
					{
						
						Point next_vertcell;
						Point next_vertpoly;

						if (it_vertcell != cellule.vertex.begin())
						{
							next_vertcell = *(it_vertcell-1);
						}
						
						else
						{
							next_vertcell = *(cellule.vertex.end()-1);
						}

					
						if (it_vertpoly != polygons[cpt].vertex.end()-1)
						{
							next_vertpoly = *(it_vertpoly+1);
						}
						else
						{
							next_vertpoly = *polygons[cpt].vertex.begin();
						}
						
						

						if (next_vertcell == next_vertpoly)
						{

							have_segment = true;


							std::cout << polygons[cpt].vertex.size() << std::endl;

							for (int i = 1; i < cellule.vertex.size() - 1; i++)
							{


								if (it_vertcell + i >= cellule.vertex.end())
								{

									polygons[cpt].vertex.insert(it_vertpoly + i -1, *(it_vertcell + i - cellule.vertex.size()));

								}
								else
								{

									polygons[cpt].vertex.insert(it_vertpoly + i -1, *(it_vertcell + i));

								}
							}
									


							

						}


					}


				}


			}
		}
		cpt++;
	}

	return have_segment;

}


bool voronoi::compare_color(Scalar color1, Scalar color2)
{

	bool dY;
	bool dU;
	bool dV;

	Mat color1m = Mat (1,1,CV_8UC3,color1);
	Mat color1mo;
	Mat color2m = Mat(1, 1, CV_8UC3, color2);
	Mat color2mo;

	cvtColor(color1m, color1mo, COLOR_BGR2YUV);
	cvtColor(color2m, color2mo, COLOR_BGR2YUV);

	dY = color1m.at<Vec3b>(0,0)[0] - color2m.at<Vec3b>(0, 0)[0] > 48;
	dU = color1m.at<Vec3b>(0, 0)[1] - color2m.at<Vec3b>(0, 0)[1] > 7;
	dV= color1m.at<Vec3b>(0, 0)[2] - color2m.at<Vec3b>(0, 0)[2] > 6;

	if (dY || dU || dV)
	{
		return false;
	}

	return true;

}
