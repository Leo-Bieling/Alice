//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

// include toolset header
#include <headers/zApp/include/zTsStatics.h>

using namespace zSpace;
using namespace std;

/*Objects*/
zObjGraph operateGraphObj;

zObjMesh convexHullObj;
zObjMeshArray colOfConHulls;

zObjMesh tmpForceObj;
zObjMeshArray colOfForceObj;
zObjMeshArray localColOfForceObj;

zObjGraph formObj;
zObjMesh formMeshObj;
zObjMeshArray polytopalObjs;

zObjMeshArray colOfFormMeshObj;
zObjGraphArray colOfFormObj;

/*Function sets*/
zFnGraph fnOperateGraph;
zFnMesh fnConvexHull;

zFnMesh tmpFnMesh;


/*Tool sets*/
zTsPolytopal myPolytopal;

/*General variables*/
//string path = "C:/Users/Leo.b/Desktop/graph/multi_graph.txt";
string path = "C:/Users/Leo.b/Desktop/graph/graphPlaceholder.txt";
//string path = "C:/Users/Leo.b/Desktop/graph/complex_2.txt";



zModel model;

bool drawGraph = true;
bool drawConHull = false;
bool drawForce = false;
bool drawForm = false;
bool drawFormMesh = false;

double background = 0.2;


////////////////////////////////////////////////////////////////////////// MAIN PROGRAM : MVC DESIGN PATTERN  ----------------------------------------------------
////// ---------------------------------------------------- MODEL  ----------------------------------------------------
void setup()
{
	model = zModel(10000);

	// load graph
	fnOperateGraph = zFnGraph(operateGraphObj);
	fnOperateGraph.from(path, zMAYATXT);
	model.addObject(operateGraphObj);
	operateGraphObj.setShowElements(true, true);

	// color valence 1 vertices
	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
		if (v.checkValency(1))
			v.setColor(zColor(0.0, 1.0, 0.0, 1.0));

	// compute convex hull per node
	zIntArray connectedVertIds;
	zPointArray connectedVertPos;
	fnConvexHull = zFnMesh(convexHullObj);

	// get vertices > valence 1
	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
	{
		if (v.getValence() > 1)
		{
			//////////////////////////////////////////////////////////////////
			// construct convex hull
			v.getConnectedVertices(connectedVertIds);

			// get positions and edge center for vert > 1
			for (int i = 0; i < connectedVertIds.size(); i++)
			{
				zItGraphVertex vTmp(operateGraphObj, connectedVertIds[i]);

				if (vTmp.checkValency(1))				
					connectedVertPos.push_back(operateGraphObj.graph.vertexPositions[connectedVertIds[i]]);				
				else			
					connectedVertPos.push_back((v.getPosition() + vTmp.getPosition()) / 2);
			}

			// make convex hull
			fnConvexHull.makeConvexHull(connectedVertPos);
	
			// add convex hull to collection // for visualisation
			colOfConHulls.push_back(convexHullObj);

			// clear
			connectedVertIds.clear();
			connectedVertPos.clear();

			// map hull to graph

			//////////////////////////////////////////////////////////////////
			// construct polyhedra
			zIntArray vertsPerFace;
			zItMeshFace newFaces;
			zPointArray tmpVerts;
			tmpFnMesh = zFnMesh(tmpForceObj);

			for (zItMeshFace f(convexHullObj); !f.end(); f++)
			{
				// clear
				vertsPerFace.clear();
				tmpFnMesh.clear();

				f.getVertices(vertsPerFace);

				for (int i = 0; i < vertsPerFace.size(); i++)
				{

					tmpVerts.clear();

					// vert 1
					tmpVerts.push_back(convexHullObj.mesh.vertexPositions[vertsPerFace[i]]);
					// vert 2
					tmpVerts.push_back(v.getPosition());

					// vert 3
					if (i < (vertsPerFace.size() - 1))
						tmpVerts.push_back(convexHullObj.mesh.vertexPositions[vertsPerFace[i + 1]]);
					else
						tmpVerts.push_back(convexHullObj.mesh.vertexPositions[vertsPerFace[0]]);

					tmpFnMesh.addPolygon(tmpVerts, newFaces);
				}

				// create last face
				tmpVerts.clear();
				f.getVertexPositions(tmpVerts);
				tmpFnMesh.addPolygon(tmpVerts, newFaces);
				tmpFnMesh.computeMeshNormals();

				// add polyhedron to force collection // for visualisation
				colOfForceObj.push_back(tmpForceObj);

				// ad
				localColOfForceObj.push_back(tmpForceObj);
			}

			// clear local convex hull for next node
			fnConvexHull.clear();

			//////////////////////////////////////////////////////////////////
			// construct dual
			myPolytopal = zTsPolytopal(formObj, formMeshObj, localColOfForceObj, polytopalObjs);
			myPolytopal.getPrimal_GlobalElementIndicies(zForceDiagram, -1, 3);

			myPolytopal.createFormMeshFromForce(zColor(0, 1, 0, 1), false, 0.1);
			//myPolytopal.getDual(0.8);



			colOfFormObj.push_back(formObj);			
			colOfFormMeshObj.push_back(formMeshObj);

			//cout << "\nfaceCount: " << formMeshObj.mesh.faces.size();
			// clear local collection for next node
			localColOfForceObj.clear();
		}

	}

	for (int i = 0; i < colOfConHulls.size(); i++)
		model.addObject(colOfConHulls[i]);
	
	for (int i = 0; i < colOfForceObj.size(); i++)
	{
		model.addObject(colOfForceObj[i]);
		colOfForceObj[i].setShowElements(true, true, false);
	}

	for (int i = 0; i < colOfFormObj.size(); i++)
		model.addObject(colOfFormObj[i]);

	for (int i = 0; i < colOfFormMeshObj.size(); i++)
	{
		model.addObject(colOfFormMeshObj[i]);
		zFnMesh tmp;
		tmp = zFnMesh(colOfFormMeshObj[i]);
		tmp.to("C:/Users/Leo.b/Desktop/EXPORT/test_" + to_string(i) + ".obj", zOBJ);
	}


	// buttons & sliders
	B = *new ButtonGroup(Alice::vec(50, 450, 0));
	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
	B.addButton(&drawConHull, "drawConvexHull");
	B.buttons[1].attachToVariable(&drawConHull);
	B.addButton(&drawForce, "drawForce");
	B.buttons[2].attachToVariable(&drawForce);
	B.addButton(&drawForm, "drawForm");
	B.buttons[3].attachToVariable(&drawForm);
	B.addButton(&drawFormMesh, "drawFormMesh");
	B.buttons[4].attachToVariable(&drawFormMesh);
}

void update(int value)
{
	operateGraphObj.setShowObject(drawGraph);

	for (int i = 0; i < colOfConHulls.size(); i++)
		colOfConHulls[i].setShowObject(drawConHull);

	for (int i = 0; i < colOfForceObj.size(); i++)
		colOfForceObj[i].setShowObject(drawForce);

	for (int i = 0; i < colOfFormObj.size(); i++)
		colOfFormObj[i].setShowObject(drawForm);

	for (int i = 0; i < colOfFormMeshObj.size(); i++)
		colOfFormMeshObj[i].setShowObject(drawFormMesh);
}

////// ---------------------------------------------------- VIEW  ----------------------------------------------------
void draw()
{
	drawGrid(10);
	backGround(background);
	
	glPointSize(6.0);
	model.draw();
	glPointSize(1.0);
}

////// ---------------------------------------------------- CONTROLLER  ----------------------------------------------------
void keyPress(unsigned char k, int xm, int ym)
{
}

void mousePress(int b, int state, int x, int y)
{
}

void mouseMotion(int x, int y)
{
}

#endif // _MAIN_