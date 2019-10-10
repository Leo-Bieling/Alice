#include "main.h"
#include <vector>
using namespace std;


class Box
{
public:
	vec pos;
	double r;

	Box(double x, double y, double z, double r_)
	{
		pos = vec(x, y, z);
		r = r_;
	}

	vector<Box> generate()
	{
		vector<Box> boxes;
		for (int x = -1; x < 2; x++)
			for (int y = -1; y < 2; y++)
				for (int z = -1; z < 2; z++)
				{
					double new_r = r / 3;
					int sum = abs(x) + abs(y) + abs(z);

					if(sum > 1)
					{
					Box b = Box(pos.x + x * new_r, pos.y + y * new_r, pos.z + z * new_r, new_r);
					boxes.push_back(b);
					}
				}
			
		return boxes;
	}

	void draw()
	{
		glPushMatrix();
		glTranslated(pos.x, pos.y, pos.z);
		drawCube(vec(-(r / 2), -(r / 2), -(r / 2)), vec((r / 2), (r / 2), (r / 2)), vec(0, 0, 0));
		glPopMatrix();
	}
};