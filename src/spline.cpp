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

void spline::calculate_spline(Mat& image, polygon spline_poly)
{
	std::vector<Point> res_spline;
	
	Mat im = Mat(rows*ceil(scale),cols*ceil(scale),CV_8UC3,Scalar(0));
	
	for(int i=0; i < spline_poly.vertex.size(); i++)
	{
		Mat P_x;
		Mat P_y;
		float t;
		std::cout << spline_poly.vertex.size() << std::endl;
		if(i < spline_poly.vertex.size()-3)
		{
			float P0_x = spline_poly.vertex[i].x;
			float P1_x = spline_poly.vertex[i+1].x;
			float P2_x = spline_poly.vertex[i+2].x;
			float P3_x = spline_poly.vertex[i+3].x;
			P_x = (Mat_<float>(4,1) << P0_x, P1_x, P2_x, P3_x);
			
			float P0_y = spline_poly.vertex[i].y;
			float P1_y = spline_poly.vertex[i+1].y;
			float P2_y = spline_poly.vertex[i+2].y;
			float P3_y = spline_poly.vertex[i+3].y;
			P_y = (Mat_<float>(4,1) << P0_y, P1_y, P2_y, P3_y);
		}
		else if(i == spline_poly.vertex.size()-3)
		{
			float P0_x = spline_poly.vertex[i].x;
			float P1_x = spline_poly.vertex[i+1].x;
			float P2_x = spline_poly.vertex[i+2].x;
			float P3_x = spline_poly.vertex[0].x;
			P_x = (Mat_<float>(4,1) << P0_x, P1_x, P2_x, P3_x);
			
			float P0_y = spline_poly.vertex[i].y;
			float P1_y = spline_poly.vertex[i+1].y;
			float P2_y = spline_poly.vertex[i+2].y;
			float P3_y = spline_poly.vertex[0].y;
			P_y = (Mat_<float>(4,1) << P0_y, P1_y, P2_y, P3_y);
		}
		else if(i == spline_poly.vertex.size()-2)
		{
			float P0_x = spline_poly.vertex[i].x;
			float P1_x = spline_poly.vertex[i+1].x;
			float P2_x = spline_poly.vertex[0].x;
			float P3_x = spline_poly.vertex[1].x;
			P_x = (Mat_<float>(4,1) << P0_x, P1_x, P2_x, P3_x);
			
			float P0_y = spline_poly.vertex[i].y;
			float P1_y = spline_poly.vertex[i+1].y;
			float P2_y = spline_poly.vertex[0].y;
			float P3_y = spline_poly.vertex[1].y;
			P_y = (Mat_<float>(4,1) << P0_y, P1_y, P2_y, P3_y);
		}
		else if(i == spline_poly.vertex.size()-1)
		{
			float P0_x = spline_poly.vertex[i].x;
			float P1_x = spline_poly.vertex[0].x;
			float P2_x = spline_poly.vertex[1].x;
			float P3_x = spline_poly.vertex[2].x;
			P_x = (Mat_<float>(4,1) << P0_x, P1_x, P2_x, P3_x);
			
			float P0_y = spline_poly.vertex[i].y;
			float P1_y = spline_poly.vertex[0].y;
			float P2_y = spline_poly.vertex[1].y;
			float P3_y = spline_poly.vertex[2].y;
			P_y = (Mat_<float>(4,1) << P0_y, P1_y, P2_y, P3_y);
		}
		std::cout << "sortie" << std::endl;
		for(int j = 0; j < 10; j++)
		{
			Mat T;
			Point ajout;
			
			t = j/10;
			T = (Mat_<float>(1,4) << pow(t,3), pow(t,2), t, 1);
			
			Mat spline_ajout_x = T*Ms*P_x;
			Mat spline_ajout_y = T*Ms*P_y;
			
			ajout = Point(spline_ajout_x.at<float>(0,0), spline_ajout_y.at<float>(0,0));
			res_spline.push_back(ajout);
		}	
		
		
		
		
	}	
	std::cout << res_spline.size() << std::endl;
	for(int k=1; k < res_spline.size()-2; k++)
	{
		line(image, res_spline[k], res_spline[k+1], Scalar(255, 0, 0));
	}
	
	
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
