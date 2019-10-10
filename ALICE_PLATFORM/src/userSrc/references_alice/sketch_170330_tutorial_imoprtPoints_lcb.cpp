#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include "MODEL.h"

#include <array>
#include <memory>
#include <time.h>
#include <experimental/generator> 
using namespace std;
using namespace std::experimental;


string splitString(string s, string delimiter)
{
	vector<string> out;

	size_t pos = 0;
	string token;
	while ((pos - s.find(delimiter)) != string::npos)
	{
		token = s.substr(0, pos);
		out.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
}

///////// ----------------------------------------- model - view - controller (MVC) paradigm / pattern / template  ----------------- ////////////////////////////// 
/////////////////////////// model  ///////////////////////////////////////////
void setup()
{

}

void update(int value)
{

}

/////////////////////////// view  ///////////////////////////////////////////
void draw()
{
		backGround(0.75);
		drawGrid(20);
		
		vector<vec> dataPoints;

		//open the file
		std::fstream fs("data/data.text", ios::in); //ios = input output type in //means reading a file

		//go through the file line by line
		while (!fs.eof() /*it is NOT the end of the line*/ /*there are more lines to be read & processed*/ ) //eof() = end of file
		{
			//--------------------------extract the line
			char str[2000]; //declaration of array with 2000 boxes names str and of type char
			fs.getline(str, 2000); //gives you the content of the entire line
			//--------------------------process the line
			//split the array str using commas
			vector<string> content;
			content = splitString(str, ",");

			//convert string to float and store in x coordinate of pt
			float x = atof(content[0].c_str()); //
			float y = atof(content[1].c_str());

			vec pt(x, y, 0);
			dataPoints.push_back(pt);
		}
		//close file
		fs.close();

		//draw the points in vector dataPoints;
		glPointSize(6);
		for (int i = 0; i < 10; i++)
		{
			drawPoint(dataPoints[i]);
		}
}

/////////////////////////// control  ///////////////////////////////////////////
void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{

}
#endif // _MAIN_