#ifndef _ACTIVE_GRAPH_
#define _ACTIVE_GRAPH_

#include "main.h"
#include "ALICE_ROBOT_DLL.h"
using namespace ROBOTICS;
#include "metaMesh.h"
#include "nachi.h"
#include "graph.h"
#include "newPhysics.h";
#include "rigidCube.h"



class activeGraph : public toroidalGraph
{
public:

	rigidCube RC;
	vector<rigidCube> RCsOnCurve;

	activeGraph()
	{
		MeshFactory fac;
		Mesh M = fac.createPlatonic(1.0 / sqrt(2.0), 6);
		RC = *new rigidCube(M);
	}

	void constructFromToroidalGraph(toroidalGraph &TG)
	{
		reset();
		for (int i = 0; i < TG.n_v; i++) createVertex(TG.positions[i]);

		for (int i = 0; i < n_v; i++) createEdge(vertices[Mod(i, n_v)], vertices[Mod(i + 1, n_v)]);
		fixed.assign(n_v, false);
		
	}

	void populateRigidBodies( float depth =0.25, float ht = 0.25 )
	{
		RCsOnCurve.clear();
		vec S, E, cen, XA, YA, ZA;
		ZA = vec(0, 0, 1);


		for (int i = 0; i < n_e; i++)
		{
			if (fixed[edges[i].vStr->id] && fixed[edges[i].vEnd->id]) continue;

			S = positions[ edges[i].vStr->id ];
			E = positions[ edges[i].vEnd->id ];
			cen = (S + E)*0.5;
			XA = (E - S).normalise();
			YA = ZA.cross(XA).normalise();

			float Scale[3];
			Scale[0] = S.distanceTo(E)* 1.0;
			Scale[1] = depth; Scale[2] = ht ;

			//RC = *new rigidCube(M);
			RC.setTransformation(XA, YA, ZA, cen);
			RC.setScale(Scale);
			RC.transform();
				RCsOnCurve.push_back(RC);
			RC.inverseTransform();
		}

		
	}



	void display()
	{
		for (auto &rc : RCsOnCurve) rc.draw();
	}

	void display(vec *P, int RES = 10 )
	{

		for (auto &rc : RCsOnCurve) rc.draw();
		

		for (auto &rc : RCsOnCurve)
		{
			rc.computeGrid(P, RES);
			rc.drawGridAsPoints(P, RES*RES*RES);
		}
		
	
	}

	void computeContacts()
	{

		//cout << " ------------------- " << P <<  endl;
		//for (int i = 0; i <RES*RES*RES; i++)P[i].print();
		//
		////P = NULL;
		////delete P;
	}
};




#endif // !_ACTIVE_GRAPH_
