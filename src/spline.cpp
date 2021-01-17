#include "spline.h"

polygon::polygon() {}

spline::spline() {}

spline::spline(std::vector<Point> vertex_list) : vertex(vertex_list) {}

void spline::find_holes()
{
	std::vector<Point>::iterator it_1; //Début du trou + edge
	std::vector<Point>::iterator it_2; //Fin du trou + edge
	
	//it_p1 -> début du trou
	//it_p2 -> fin du trou
	
	std::vector<Point>::iterator region_begin;
	region_begin = vertex.begin();
	
	for(it_1=vertex.begin(); it_1!=vertex.end(); it_1++)
	{
		for(it_2=vertex.begin(); it_2!=vertex.end(); it_2++)
		{
			if(find_edge(it_1, it_2))
			{
				while(region_begin != it_1)
				{
					spline_poly.vertex.push_back(*region_begin);
					region_begin++;
				}
				
				polygon hole;
				
				while(it_p2 != it_p1);
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
	}
}

bool spline::find_edge(std::vector<Point>::iterator it_1, std::vector<Point>::iterator it_2)
{
	it_p1 = it_1;
	it_p2 = it_2;
	double dP = norm(*it_p1 - *it_p2);
	int cpt = 0;
	while(dP < 0.05f)
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
		
		//if(cpt>10){break;}
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
