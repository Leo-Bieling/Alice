#define _DWELLT_UNITS_

#ifdef _DWELLT_UNITS_

#include "main.h"
#include "MODEL.h"
#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator> 
using namespace std;
using namespace std::experimental;

class units
{
public:

	// ---------------------------------------------------------------------------------
	//                                   Class Variables
	// ---------------------------------------------------------------------------------

	vec pos;
	int unitType;
	

	float unitsHeight = 2.6;

	float unit0Width = 2;
	float unit0Depth = 2;

	float unit1Width = 4;
	float unit1Depth = 2;
	
	float unit2Width = 6;
	float unit2Depth = 3;

	// ---------------------------------------------------------------------------------
	//                                     Constructor
	// ---------------------------------------------------------------------------------

	units() {}; // empty constructor
	units(vec _pos, int _unitType)
	{

		pos = _pos;
		unitType = _unitType;
	}

	// ---------------------------------------------------------------------------------
	//                             Variable Setters and Getters
	// ---------------------------------------------------------------------------------

	vec getPos()
	{
		return pos;
	}

	void setPos(vec _pos)
	{
		pos = _pos;
	}

	int getType()
	{
		return unitType;
	}

	void setType(int _unitType)
	{
		unitType = _unitType;
	}

	void draw()
	{
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glLineWidth(4);
		glColor3f(1, 1, 0);

		if (unitType == 0)
		{
			drawLine(vec((unit0Depth*0.5), (unit0Width*0.5), 0), vec((unit0Depth*0.5), -(unit0Width*0.5), 0));
			drawLine(vec((unit0Depth*0.5), -(unit0Width*0.5), 0), vec(-(unit0Depth*0.5), -(unit0Width*0.5), 0));
			drawLine(vec(-(unit0Depth*0.5), -(unit0Width*0.5), 0), vec(-(unit0Depth*0.5), (unit0Width*0.5), 0));
			drawLine(vec(-(unit0Depth*0.5), (unit0Width*0.5), 0), vec((unit0Depth*0.5), (unit0Width*0.5), 0));
			drawLine(vec((unit0Depth*0.5), -(unit0Width*0.5), 0), vec((unit0Depth*0.5), -(unit0Width*0.5), unitsHeight));
			drawLine(vec(-(unit0Depth*0.5), -(unit0Width*0.5), 0), vec(-(unit0Depth*0.5), -(unit0Width*0.5), unitsHeight));
			drawLine(vec(-(unit0Depth*0.5), (unit0Width*0.5), 0), vec(-(unit0Depth*0.5), (unit0Width*0.5), unitsHeight));
			drawLine(vec((unit0Depth*0.5), (unit0Width*0.5), 0), vec((unit0Depth*0.5), (unit0Width*0.5), unitsHeight));
			drawLine(vec((unit0Depth*0.5), (unit0Width*0.5), unitsHeight), vec((unit0Depth*0.5), -(unit0Width*0.5), unitsHeight));
			drawLine(vec((unit0Depth*0.5), -(unit0Width*0.5), unitsHeight), vec(-(unit0Depth*0.5), -(unit0Width*0.5), unitsHeight));
			drawLine(vec(-(unit0Depth*0.5), -(unit0Width*0.5), unitsHeight), vec(-(unit0Depth*0.5), (unit0Width*0.5), unitsHeight));
			drawLine(vec(-(unit0Depth*0.5), (unit0Width*0.5), unitsHeight), vec((unit0Depth*0.5), (unit0Width*0.5), unitsHeight));
		}
		
		if (unitType == 1)
		{
			drawLine(vec((unit1Depth*0.5), (unit1Width*0.5), 0), vec((unit1Depth*0.5), -(unit1Width*0.5), 0));
			drawLine(vec((unit1Depth*0.5), -(unit1Width*0.5), 0), vec(-(unit1Depth*0.5), -(unit1Width*0.5), 0));
			drawLine(vec(-(unit1Depth*0.5), -(unit1Width*0.5), 0), vec(-(unit1Depth*0.5), (unit1Width*0.5), 0));
			drawLine(vec(-(unit1Depth*0.5), (unit1Width*0.5), 0), vec((unit1Depth*0.5), (unit1Width*0.5), 0));
			drawLine(vec((unit1Depth*0.5), -(unit1Width*0.5), 0), vec((unit1Depth*0.5), -(unit1Width*0.5), unitsHeight));
			drawLine(vec(-(unit1Depth*0.5), -(unit1Width*0.5), 0), vec(-(unit1Depth*0.5), -(unit1Width*0.5), unitsHeight));
			drawLine(vec(-(unit1Depth*0.5), (unit1Width*0.5), 0), vec(-(unit1Depth*0.5), (unit1Width*0.5), unitsHeight));
			drawLine(vec((unit1Depth*0.5), (unit1Width*0.5), 0), vec((unit1Depth*0.5), (unit1Width*0.5), unitsHeight));
			drawLine(vec((unit1Depth*0.5), (unit1Width*0.5), unitsHeight), vec((unit1Depth*0.5), -(unit1Width*0.5), unitsHeight));
			drawLine(vec((unit1Depth*0.5), -(unit1Width*0.5), unitsHeight), vec(-(unit1Depth*0.5), -(unit1Width*0.5), unitsHeight));
			drawLine(vec(-(unit1Depth*0.5), -(unit1Width*0.5), unitsHeight), vec(-(unit1Depth*0.5), (unit1Width*0.5), unitsHeight));
			drawLine(vec(-(unit1Depth*0.5), (unit1Width*0.5), unitsHeight), vec((unit1Depth*0.5), (unit1Width*0.5), unitsHeight));
		}

		if (unitType == 2)
		{
			drawLine(vec((unit2Depth*0.5), (unit2Width*0.5), 0), vec((unit2Depth*0.5), -(unit2Width*0.5), 0));
			drawLine(vec((unit2Depth*0.5), -(unit2Width*0.5), 0), vec(-(unit2Depth*0.5), -(unit2Width*0.5), 0));
			drawLine(vec(-(unit2Depth*0.5), -(unit2Width*0.5), 0), vec(-(unit2Depth*0.5), (unit2Width*0.5), 0));
			drawLine(vec(-(unit2Depth*0.5), (unit2Width*0.5), 0), vec((unit2Depth*0.5), (unit2Width*0.5), 0));
			drawLine(vec((unit2Depth*0.5), -(unit2Width*0.5), 0), vec((unit2Depth*0.5), -(unit2Width*0.5), unitsHeight));
			drawLine(vec(-(unit2Depth*0.5), -(unit2Width*0.5), 0), vec(-(unit2Depth*0.5), -(unit2Width*0.5), unitsHeight));
			drawLine(vec(-(unit2Depth*0.5), (unit2Width*0.5), 0), vec(-(unit2Depth*0.5), (unit2Width*0.5), unitsHeight));
			drawLine(vec((unit2Depth*0.5), (unit2Width*0.5), 0), vec((unit2Depth*0.5), (unit2Width*0.5), unitsHeight));
			drawLine(vec((unit2Depth*0.5), (unit2Width*0.5), unitsHeight), vec((unit2Depth*0.5), -(unit2Width*0.5), unitsHeight));
			drawLine(vec((unit2Depth*0.5), -(unit2Width*0.5), unitsHeight), vec(-(unit2Depth*0.5), -(unit2Width*0.5), unitsHeight));
			drawLine(vec(-(unit2Depth*0.5), -(unit2Width*0.5), unitsHeight), vec(-(unit2Depth*0.5), (unit2Width*0.5), unitsHeight));
			drawLine(vec(-(unit2Depth*0.5), (unit2Width*0.5), unitsHeight), vec((unit2Depth*0.5), (unit2Width*0.5), unitsHeight));
		}
		glLineWidth(1);
		glColor3f(0, 0, 0);
		glPopMatrix();
	}
};

#endif // !_DWELLT_UNITS_