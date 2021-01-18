#include "spline.h"

polygon::polygon() {}

spline::spline() {}

spline::spline(std::vector<Point> vertex_list) : vertex(vertex_list) {}

B_splines::B_splines() {}

B_splines::B_splines(voronoi& voronoi_diagram)
{
	std::vector<cell>::iterator it_cell;
	
	for(it_cell=voronoi_diagram.polygons.begin(); it_cell!=voronoi_diagram.polygons.end(); it_cell++)
	{
		cell new_cell = *it_cell;
		spline new_spline = spline(new_cell.vertex);
		new_spline.color = new_cell.color;
		splines.push_back(new_spline);
	}
}

void B_splines::find_holes_splines()
{
	std::vector<spline>::iterator it_spline;
	
	for(it_spline=splines.begin(); it_spline!=splines.end(); it_spline++)
	{
		spline current_spline = *it_spline;
		current_spline.find_holes();
		*it_spline = current_spline;
	}
}

void B_splines::print_spline(int id_spline)
{
	spline printed_spline = splines[id_spline];
	std::vector<Point>::iterator it_vertex;
	
	for(it_vertex=printed_spline.spline_poly.vertex.begin(); it_vertex!=printed_spline.spline_poly.vertex.end(); it_vertex++)
	{
		std::cout<<*it_vertex<<std::endl;
	}
}

void spline::find_holes()
{
	std::vector<Point>::iterator it_1; //Début du trou + edge
	std::vector<Point>::iterator it_2; //Fin du trou + edge
	
	//it_p1 -> début du trou
	//it_p2 -> fin du trou
	
	for(it_1=vertex.begin(); it_1!=vertex.end(); it_1++)
	{
		for(it_2=vertex.begin(); it_2!=vertex.end(); it_2++)
		{
			if(find_edge(it_1, it_2))
			{
				polygon hole;
				
				while(it_p2 != it_p1)
				{
					hole.vertex.push_back(*it_p2);
					
					if (it_p2 != vertex.begin())
					{
						it_p2--;
					}
					else
					{
						it_p2 = vertex.end()-1;
					}
					
				}
				
				holes.push_back(hole);
				
				it_1 = it_2 + 1;
				break;
			}
		}
		
		spline_poly.vertex.push_back(*it_1);
	}
}

bool spline::find_edge(std::vector<Point>::iterator& it_1, std::vector<Point>::iterator& it_2)
{
	it_p1 = it_1;
	it_p2 = it_2;
	double dP = norm(*it_p1 - *it_p2);
	int cpt = 0;
	float seuil = 0.05f;
	while(dP < seuil)
	{
		if (it_p1 != vertex.end()-1)
		{
			it_p1++;
		}
		else
		{
			it_p1 = vertex.begin();
		}

		if (it_p2 != vertex.begin())
		{
			it_p2--;
		}
		else
		{
			it_p2 = vertex.end()-1;
		}
		
		
		dP = norm(*it_p1 - *it_p2);
		cpt++;
		
	}
	if (it_p2 != vertex.end()-1)
	{
		it_p2++;
	}
	else
	{
		it_p2 = vertex.begin();
	}

	if (it_p1 != vertex.begin())
	{
		it_p1--;
	}
	else
	{
		it_p1 = vertex.end()-1;
	}
	return cpt>1;
}

Mat spline::draw_spline(float scale, int rows, int cols)
{
	Mat drawed_spline = Mat(rows*ceil(scale),cols*ceil(scale),CV_8UC3,Scalar(255,255,255));
	
	std::vector<polygon>::iterator it;
	
	std::vector<Point> poly_vertex_list = spline_poly.vertex;
	const int npt_poly = poly_vertex_list.size();
	const Point* ppt_poly = &poly_vertex_list[0];
	fillPoly(drawed_spline,&ppt_poly,&npt_poly,1,Scalar(0,255,255));
	
	for(it=holes.begin(); it!=holes.end(); it++)
	{
		polygon poly = *it;
		std::vector<Point> vertex_list = poly.vertex;
		const int npt = vertex_list.size();
		const Point* ppt = &vertex_list[0];
		fillPoly(drawed_spline,&ppt,&npt,1,Scalar(255,255,0));
	}
	return drawed_spline;
}
