//#define _MAIN_

#ifdef _MAIN_

// alice header
#include "main.h"

// zSpace v003
// zSpace header
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>
//#include "quickhull.h" // from https://github.com/karimnaaji/3d-quickhull

// include toolset header
#include <headers/zApp/include/zTsStatics.h>

using namespace zSpace;
using namespace std;

void convexHull(zPointArray &_pts, zObjMesh &_mesh)
{
	int num = _pts.size();
	qh_vertex_t *vertices = new qh_vertex_t[num];

	for (int i = 0; i < num; ++i)
	{
		vertices[i].x = _pts[i].x;
		vertices[i].y = _pts[i].y;
		vertices[i].z = _pts[i].z;
	}
	
	qh_mesh_t mesh = qh_quickhull3d(vertices, num);

	zItMeshFace f(_mesh);
	zFnMesh fnTmpMesh = zFnMesh(_mesh);
	fnTmpMesh.clear();

	zPointArray pts;
	for (int i = 0; i < mesh.nvertices; i += 3)
	{
		pts.clear();

		pts.push_back(zVector(mesh.vertices[i + 0].x, mesh.vertices[i + 0].y, mesh.vertices[i + 0].z));
		pts.push_back(zVector(mesh.vertices[i + 1].x, mesh.vertices[i + 1].y, mesh.vertices[i + 1].z));
		pts.push_back(zVector(mesh.vertices[i + 2].x, mesh.vertices[i + 2].y, mesh.vertices[i + 2].z));
		fnTmpMesh.addPolygon(pts, f);
	}
}

/*Objects*/
zObjGraph operateGraphObj;
zObjMesh convexHullObj;
zObjMesh tmpObjMesh;
zObjMeshArray colOfMeshObj;

/*Function sets*/
zFnGraph fnOperateGraph;
zFnMesh fnConvexHull;

zFnMesh tmpFnMesh;

/*Tool sets*/
zTsPolytopal myPolytopal;

zObjGraph formObj;
zObjMesh formMeshObj;
zObjMeshArray polytopalObjs;


/*General variables*/
string path = "C:/Users/Leo.b/Desktop/graph/graphPlaceholder3.txt";
string path_1 = "C:/Users/Leo.b/Desktop/graph/graphPlaceholder.txt";
string path_2 = "C:/Users/Leo.b/Desktop/graph/graphPlaceholder2.txt";
string path_3 = "C:/Users/Leo.b/Desktop/graph/graphPlaceholder3.txt";

zModel model;
zVector overallCenter;
zVectorArray localCenter;

double explodeFactor = 0.0;
bool drawGraph = true;
bool drawConHull = false;
bool drawMeshes = false;

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

	// get convex hull
	fnConvexHull = zFnMesh(convexHullObj);
	// create convex hull
	zPointArray vertPos;
	fnOperateGraph.getVertexPositions(vertPos);
	convexHull(vertPos, convexHullObj);
	model.addObject(convexHullObj);

	// align graph IDs with the convex hull //NOT SOLVED
	for (zItMeshVertex vM(convexHullObj); !vM.end(); vM++)
		for (zItGraphVertex vG(operateGraphObj); !vG.end(); vG++)
			if (vM.getPosition().distanceTo(vG.getPosition()) < EPS)
			{
				//vM.setId(vG.getId());
				cout << "\n" << vM.getPosition().distanceTo(vG.getPosition());
				//printf("\ngraphVertexId: %i", vG.getId());			
				//printf("\nmeshVertexId: %i\n", vM.getId());
			}
		
	// get the center vertex ID
	int centerId;
	for (zItGraphVertex v(operateGraphObj); !v.end(); v++)
		if (!v.checkValency(1)) centerId = v.getId();

	// construct polyhedra
	zIntArray vertsPerFace;
	zItMeshFace newFaces;
	zPointArray tmpVerts;
	tmpFnMesh = zFnMesh(tmpObjMesh);

	for (zItMeshFace f(convexHullObj); !f.end(); f++)
	{
		vertsPerFace.clear();
		tmpFnMesh.clear();

		f.getVertices(vertsPerFace);

		for (int i = 0; i < vertsPerFace.size(); i++)
		{
			tmpVerts.clear();

			// vert 1
			tmpVerts.push_back(convexHullObj.mesh.vertexPositions[vertsPerFace[i]]);
			// vert 2
			tmpVerts.push_back(operateGraphObj.graph.vertexPositions[centerId]);

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

		// add polyhedron to collection
		colOfMeshObj.push_back(tmpObjMesh);
	}
			
	// get perpendicular dual graph
	myPolytopal = zTsPolytopal(formObj, formMeshObj, colOfMeshObj, polytopalObjs);

	myPolytopal.getPrimal_GlobalElementIndicies(zForceDiagram, -1, 3);
	//myPolytopal.createFormMeshFromForce(zColor(0, 1, 0, 1), false, 0.1);

	myPolytopal.getDual(0.8);

	model.addObject(formObj);
	
	model.addObject(formMeshObj);

	printf("\nformGraph: %i", formObj.graph.edges.size());


	// add to model & get explode vector
	for (int i = 0; i < colOfMeshObj.size(); i++)
	{
		model.addObject(colOfMeshObj[i]);

		zPoint BBmin;
		zPoint BBmax;
		colOfMeshObj[i].getBounds(BBmin, BBmax);
		localCenter.push_back((BBmin + BBmax) / 2);
		overallCenter += localCenter[i];
	}
	overallCenter /= colOfMeshObj.size();

	// buttons & sliders
	B = *new ButtonGroup(Alice::vec(50, 450, 0));
	B.addButton(&drawGraph, "drawGraph");
	B.buttons[0].attachToVariable(&drawGraph);
	B.addButton(&drawConHull, "drawConvexHull");
	B.buttons[1].attachToVariable(&drawConHull);
	B.addButton(&drawMeshes, "drawMeshes");
	B.buttons[2].attachToVariable(&drawMeshes);

	S = *new SliderGroup(Alice::vec(50, 250, 0));
	S.addSlider(&explodeFactor, "explodeFactor");
	S.sliders[0].attachToVariable(&explodeFactor, 0.0, 10.0);
}

void update(int value)
{
	operateGraphObj.setShowObject(drawGraph);
	convexHullObj.setShowObject(drawConHull);

	for (int i = 0; i < colOfMeshObj.size(); i++)
	{
		// add to model
		colOfMeshObj[i].setShowObject(drawMeshes);

		// translate for explosion
		zFnMesh tmp;
		tmp = zFnMesh(colOfMeshObj[i]);
		zVector tmp_vec = localCenter[i] - overallCenter;
		tmp_vec.normalize();
		
		tmp.setTranslation(tmp_vec * explodeFactor);
	}
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
	if (k == '1')
	{
		path = path_1;
		explodeFactor = 0.0;
		fnConvexHull.clear();
		colOfMeshObj.clear();
		localCenter.clear();
		setup();
	}

	if (k == '2')
	{
		path = path_2;
		explodeFactor = 0.0;
		fnConvexHull.clear();
		colOfMeshObj.clear();
		localCenter.clear();
		setup();
	}

	if (k == '3')
	{
		path = path_3;
		explodeFactor = 0.0;
		fnConvexHull.clear();
		colOfMeshObj.clear();
		localCenter.clear();
		setup();
	}
}

void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

#endif // _MAIN_