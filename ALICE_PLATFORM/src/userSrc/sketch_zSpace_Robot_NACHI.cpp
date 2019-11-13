//#define _MAIN_

#ifdef _MAIN_

#include "main.h"

//// zSpace Core Headers
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

//// zSpace Toolsets
#include <headers/zToolsets/digifab/zTsRobot.h>

using namespace zSpace;

////////////////////////////////////////////////////////// Custom Methods

void drawTextAtVec(string s, zVector &pt)
{
	unsigned int i;
	glRasterPos3f(pt.x, pt.y, pt.z);

	for (i = 0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
}

////////////////////////////////////////////////////////// zSpace Objects

zModel model;
zUtilsCore coreUtils;

zTsRobot Nachi;

vector<zObjMesh> r_meshObjs;
zObjGraph r_graphObj;

zObjMesh objMesh;

zTransform robotTarget;
zTransform robotEE;

////////////////////////////////////////////////////////// Others

bool zRobot_IMPORT = true;
bool zRobot_IK = false;
bool zRobot_DISPLAY_targetFrame = false;
bool zRobot_TARGETS = false;
bool zRobot_GCODE = false;
bool zRobot_GCODE_Export = false;

bool resetPoints = false;
bool run = false;

int targetCounter = 0;

string dirPath = "C:/Users/Leo.b/source/repos/Alice2018_CUDA/ALICE_PLATFORM/x64/Release/EXE/data/NACHI_MZ07/Meshes_zSpace";
string robotName = "Nachi_MZ07.json";
vector<string> robotFile;

double zRobotVel = 100;
double increment = 1.0;
double background = 0.9;

vector<zVector> eCenters;

double d0 = 100.00;
double d1 = 99.99;
double d2 = 10.00;
double d3 = 9.99;

void setup()
{
	cout << d0 - d1 << endl;
	cout << d2 - d3 << endl;

	////////////////////////////////////////////////////////// Initialize Model

	model = zModel(1000000);

	////////////////////////////////////////////////////////// zROBOT Import
	
	//coreUtils.getFilesFromDirectory(robotFile, dirPath, zJSON);

	int nF = coreUtils.getNumfiles_Type(dirPath, zOBJ);
	if (nF < 8) nF = 8;

	r_meshObjs.assign(nF, zObjMesh());

	Nachi = zTsRobot(r_graphObj, r_meshObjs);

	Nachi.createRobotfromFile(dirPath + "/" + robotName, zJSON);

	Nachi.createRobotJointMeshesfromFile(dirPath, zOBJ);
	Nachi.createEndEffectorMeshfromFile(dirPath, zOBJ);

	//////// Set Endeffector Transform

	robotEE.setIdentity();

	zPoint* pos = Nachi.fnMeshJoints[7].getRawVertexPositions();

	zVector rx = pos[0] - pos[1];
	zVector ry = pos[0] - pos[2];
	zVector rz = rx.operator^(ry);

	rx.normalize(); ry.normalize(); rz.normalize();

	robotEE(0, 0) = rz.x;
	robotEE(0, 1) = rz.y;
	robotEE(0, 2) = rz.z;
	robotEE(1, 0) = ry.x;
	robotEE(1, 1) = ry.y;
	robotEE(1, 2) = ry.z;

	robotEE(2, 0) = rx.x *-1;
	robotEE(2, 1) = rx.y *-1;
	robotEE(2, 2) = rx.z *-1;

	robotEE(3, 0) = pos[0].x;
	robotEE(3, 1) = pos[0].y;
	robotEE(3, 2) = pos[0].z;

	local = coreUtils.toLocalMatrix(robotEE);

	local(3, 0) -= 2;
	local(3, 1) += 0;
	local(3, 2) -= 12.5;

	Nachi.setEndEffector(local);

	//////////////////////////////////////////////////////////  Display Setup

	model.addObject(r_graphObj);

	for (int i = 0; i < r_meshObjs.size(); i++)
		model.addObject(r_meshObjs[i]);
		
	r_graphObj.setShowElements(true, true);

	for (int i = 0; i < r_meshObjs.size(); i++)
	{
		r_meshObjs[i].appendToBuffer(Nachi.robot_jointMeshes_edgeAngle[i], true, 80);
		r_meshObjs[i].setShowTransform(true);	
	}

	model.setShowBufLines(true,false);
	model.setShowBufTris(true,true);
		
	//////////////////////////////////////////////////////////  Initialize Sliders

	S = *new SliderGroup();

	S.addSlider(&Nachi.jointRotations[0].rotation, "J1");
	S.addSlider(&Nachi.jointRotations[1].rotation, "J2");
	S.addSlider(&Nachi.jointRotations[2].rotation, "J3");
	S.addSlider(&Nachi.jointRotations[3].rotation, "J4");
	S.addSlider(&Nachi.jointRotations[4].rotation, "J5");
	S.addSlider(&Nachi.jointRotations[5].rotation, "J6");
	
	S.sliders[0].attachToVariable(&Nachi.jointRotations[0].rotation, Nachi.jointRotations[0].minimum , Nachi.jointRotations[0].maximum);
	S.sliders[1].attachToVariable(&Nachi.jointRotations[1].rotation, Nachi.jointRotations[1].minimum , Nachi.jointRotations[1].maximum );
	S.sliders[2].attachToVariable(&Nachi.jointRotations[2].rotation, Nachi.jointRotations[2].minimum , Nachi.jointRotations[2].maximum);
	S.sliders[3].attachToVariable(&Nachi.jointRotations[3].rotation, Nachi.jointRotations[3].minimum , Nachi.jointRotations[3].maximum );
	S.sliders[4].attachToVariable(&Nachi.jointRotations[4].rotation, Nachi.jointRotations[4].minimum , Nachi.jointRotations[4].maximum);
	S.sliders[5].attachToVariable(&Nachi.jointRotations[5].rotation, Nachi.jointRotations[5].minimum , Nachi.jointRotations[5].maximum );

	S.addSlider(&background, "background");

	//////////////////////////////////////////////////////////  Initialize Buttons

	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&zRobot_IMPORT, "Import");
	B.addButton(&zRobot_IK, "IK");
	B.addButton(&zRobot_DISPLAY_targetFrame, "Target Frames");
	B.addButton(&zRobot_GCODE, "Store GCODE");
	B.addButton(&zRobot_GCODE_Export, "Export");

	B.addButton(&run, "run");
	B.addButton(&resetPoints, "reset");
}

void update(int value)
{
	objMesh.setShowObject(zRobot_DISPLAY_targetFrame);

	if (!run)
	{		
		if (zRobot_IMPORT)
		{
			Nachi.createTargetsfromFile(dirPath + "/path.txt", zTXT);
			zRobot_IMPORT = !zRobot_IMPORT;
		}

		if (zRobot_IK)
		{
			//////// Set Targets Transform and Compute Inverse Kinematics

			if (Nachi.robotTargets.empty())
			{
				cout << "\nNo target frames have been assigned " << endl;
				zRobot_IK = !zRobot_IK;
			}
			else
			{
				robotTarget.setIdentity();
				robotTarget = Nachi.robotTargets[targetCounter];

				targetCounter = targetCounter % Nachi.robotTargets.size();

				Nachi.setTarget(robotTarget);	

				zVector posIK = Nachi.inverseKinematics();

				//////// Store G-Code

				if (zRobot_GCODE)
				{
					for (int i = 0; i < Nachi.robotTargets.size(); i++)
					{
						Nachi.setTarget(Nachi.robotTargets[i]);

						zVector posIK = Nachi.inverseKinematics();
						Nachi.gCode_store(posIK, zRobotVel, zMoveJoint, zEENeutral);
					}

					zRobot_GCODE = !zRobot_GCODE;
				}
			}
		}

		if (!zRobot_IK)
		{
			//////// Compute Forward Kinematics

			zVector posFK = Nachi.forwardKinematics(zJoint);

			if (zRobot_GCODE)
			{
				Nachi.gCode_store(posFK, zRobotVel, zMoveJoint, zEENeutral);

				zRobot_GCODE = !zRobot_GCODE;
			}
		}

		//////////////////////////////////////////////////////////  Update Vertex Buffer

		for (int i = 0; i < DOF + 2; i++)
		{
			if (Nachi.fnMeshJoints[i].numVertices() == 0) continue;
			model.displayUtils.bufferObj.updateVertexPositions(Nachi.fnMeshJoints[i].getRawVertexPositions(), Nachi.fnMeshJoints[i].numVertices(), Nachi.fnMeshJoints[i].getVBOVertexIndex());
		}

		//////////////////////////////////////////////////////////  Export GCode

		if (zRobot_GCODE_Export)
		{
			Nachi.gCode_to("data", zRobotNachi);

			zRobot_GCODE_Export = !zRobot_GCODE_Export;
		}
	}
	
}


void draw()
{
	backGround(background);
	drawGrid(20.0);

	//////////////////////////////////////////////////////////  Draw Sliders and Buttons

	S.draw();
	B.draw();

	//////////////////////////////////////////////////////////  Draw Model

	model.draw();
	
	if (zRobot_DISPLAY_targetFrame)
	{
		for (int i = 0; i < Nachi.robotTargets.size(); i++)
			model.displayUtils.drawTransform(Nachi.robotTargets[i]);	

		//////////////////////////////////////////////////////////  Draw On Screen

		for (int i = 0; i < Nachi.robotTargets.size(); i++)
			drawTextAtVec(to_string(i), zVector(Nachi.robotTargets[i](3, 0), Nachi.robotTargets[i](3, 1), Nachi.robotTargets[i](3, 2)));
	}
	
}

void keyPress(unsigned char k, int xm, int ym)
{

	if (k == 'w') robotTarget(3, 0) += increment;	
	if (k == 'W') robotTarget(3, 0) -= increment;
	if (k == 'y') robotTarget(3, 1) += increment;
	if (k == 'Y') robotTarget(3, 1) -= increment;	
	if (k == 'z') robotTarget(3, 2) += increment;
	if (k == 'Z') robotTarget(3, 2) -= increment;

	if (k == 'e')
		zRobot_GCODE_Export = !zRobot_GCODE_Export;

	if (k == 'n')
		targetCounter++;

	if (k == 'p')
	{
		zTransform originalTargetMatrix = robotTarget;

		zVector rtarget_X(originalTargetMatrix(0, 0), originalTargetMatrix(1, 0), originalTargetMatrix(2, 0));
		zVector rtarget_Y(originalTargetMatrix(0, 1), originalTargetMatrix(1, 1), originalTargetMatrix(2, 1));
		zVector rtarget_Z(originalTargetMatrix(0, 2), originalTargetMatrix(1, 2), originalTargetMatrix(2, 2));

		zVector new_rtarget_Z = rtarget_Z.rotateAboutAxis(rtarget_Y, 10);
		zVector new_rtarget_X = rtarget_X.rotateAboutAxis(rtarget_Y, 10);

		robotTarget(0, 2) = new_rtarget_Z.x; robotTarget(1, 2) = new_rtarget_Z.y; robotTarget(2, 2) = new_rtarget_Z.z;
		robotTarget(0, 0) = new_rtarget_X.x; robotTarget(1, 0) = new_rtarget_X.y; robotTarget(2, 0) = new_rtarget_X.z;
	}

}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_