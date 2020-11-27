#pragma once
#include "treeNode.h"

class Edge;
class Vertex;
class Face;

class Vertex {
public:
	Vertex(glm::vec3 p);
	glm::vec3 pointPos;
	std::vector<Face*> faceList_ofPoint;
	std::vector<Edge*> edgeList_ofPoint;
	Vertex* updateVertex;    

	void computeUpdateVert();
};

class Edge {
public:
	Edge(Vertex *a, Vertex *b);
	Vertex *a, *b; 
	std::vector<Face*> faceList_ofEdge;
	Vertex *updatedEdgePoint;
	glm::vec3 midPoint;
	Edge *newEdge1, *newEdge2;

	void computeUpdatedEdgePoint();
	void computeNewEdges(); 
};

class Face {
public:
	Edge *edge0, *edge1, *edge2, *edge3;
	Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3);
	Vertex* newFacePoint;
	Edge *newEdge0, *newEdge1, *newEdge2, *newEdge3;
	Face *newFace0, *newFace1, *newFace2, *newFace3;
	void computeFacePoint();
	void pushNewSurface();
	                           
};

struct treeCube {
	treeCube(glm::vec3 center, float scale);
	glm::vec3 center;
	//eight vertices
	Vertex *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7; 
	//12 edges
	Edge *e0, *e1, *e2, *e3, *e4, *e5, *e6, *e7, *e8, *e9, *e10, *e11;
	//6 faces
	Face *f0, *f1, *f2, *f3, *f4, *f5;
	bool Up = true, Down = true, Left = true, Right = true;
	void GenerateFace();
};

class treeMesh
{
public:
	std::vector<Vertex*> vertList_treeMesh;
	std::vector<Face*> faceList_treeMesh;
	std::vector<Edge*> edgeList_treeMesh;
	std::vector<treeCube*> cubeList_treeMesh;

	treeMesh();
	treeMesh(std::vector<glm::vec3> v);

	void PushCube(treeCube *c);
	void ConnectCube(treeCube* c1, treeCube* c2);
	void GenerateCubes(std::vector<glm::vec3> *v, std::vector<glm::vec3>::iterator & it, treeCube* lastCube);

	treeMesh* CatmullClarkSubdiv();

};



