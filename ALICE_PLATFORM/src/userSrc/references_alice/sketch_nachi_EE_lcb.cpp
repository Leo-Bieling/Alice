#define _MAIN_

#ifdef _MAIN_
#include "main.h"
#include "ALICE_ROBOT_DLL.h"
#include "metaMesh.h"
#include "nachi_lcb.h"
#include "graphStack.h"
using namespace ROBOTICS;

pathImporter path;
SliderGroup S;
ButtonGroup B;

bool showRobot = true;
bool run = false;

char s[200];
char t[200];
char jts[400];

void setup()
{
	int i = 0;
	{
		path.readPath("data/path_test.txt", ",", 1.15 + float(i) * 0.1);
		path.actualPathLength;
	}

	S = *new SliderGroup();
	S.addSlider(&path.Nachi_tester.rot[0], "J1");
	S.addSlider(&path.Nachi_tester.rot[1], "J2");
	S.addSlider(&path.Nachi_tester.rot[2], "J3");
	S.addSlider(&path.Nachi_tester.rot[3], "J4");
	S.addSlider(&path.Nachi_tester.rot[4], "J5");
	S.addSlider(&path.Nachi_tester.rot[5], "J6");

	S.sliders[0].attachToVariable(&path.Nachi_tester.rot[0], -170, 170);
	S.sliders[1].attachToVariable(&path.Nachi_tester.rot[1], -170, 170);
	S.sliders[2].attachToVariable(&path.Nachi_tester.rot[2], -170, 170);
	S.sliders[3].attachToVariable(&path.Nachi_tester.rot[3], -170, 170);
	S.sliders[4].attachToVariable(&path.Nachi_tester.rot[4], -170, 170);
	S.sliders[5].attachToVariable(&path.Nachi_tester.rot[5], -170, 170);

	S.sliders[6].minVal = 0;
	S.sliders[6].maxVal = 1400;

	B = *new ButtonGroup(vec(50, 450, 0));
	B.addButton(&showRobot, "showNachi");
}

void update(int value)
{
	path.Nachi_tester.ForwardKineMatics(path.Nachi_tester.rot);
}

void draw()
{
	backGround(0.25);

	S.draw();
	B.draw();

	if (showRobot)
		path.draw(false);

	////////////////////////////////////////////////////////// graph

	sprintf_s(s, " current point id : %i", path.currentPointId);
	sprintf_s(t, " total points in path : %i", path.actualPathLength - 1);
	int cid = path.currentPointId;

	if (cid < path.actualPathLength - 1 && cid >= 0)
		sprintf_s(jts, "%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f", path.rotations[cid][0], path.rotations[cid][1], path.rotations[cid][2],
			path.rotations[cid][3], path.rotations[cid][4], path.rotations[cid][5]);

	setup2d();

	drawString(s, winW * 0.5, winH - 50);
	drawString(t, winW * 0.5, winH - 75);
	drawString(jts, winW * 0.5, winH - 100);

	int hts = 50;
	int wid = winW * 0.75;
	drawString(" n : path.goToNextPoint();", wid, hts); hts += 25;
	drawString(" b : path.goToPrev();", wid, hts); hts += 25;
	drawString(" N : path.currentId = 0;", wid, hts); hts += 25;
	drawString(" q : path.checkPathForReachability();", wid, hts); hts += 25;
	drawString(" w : path.exportGCode();", wid, hts); hts += 25;
	drawString(" r : setup();", wid, hts); hts += 25;
	drawString(" h : path.home();", wid, hts); hts += 25;
	
	restore3d();
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'n')path.goToNextPoint();
	if (k == 'b')
	{
		path.currentPointId -= 2;;
		path.goToNextPoint();
	}
	if (k == 'N')path.currentPointId = 0;
	if (k == 'q')path.checkPathForReachability();
	if (k == 'w')path.exportGCode();
	if (k == 'r')setup();
	if (k == 'h')
	{
		for (int i = 0; i < DOF; i++)path.Nachi_tester.rot[i] = 0.;
		path.Nachi_tester.rot[1] = 90.0;

		path.Nachi_tester.ForwardKineMatics(path.Nachi_tester.rot);
	}
}

void mousePress(int b, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == state)
	{
		S.performSelection(x, y, HUDSelectOn);
		B.performSelection(x, y);
	}
}

void mouseMotion(int x, int y)
{
	S.performSelection(x, y, HUDSelectOn);
}

#endif
