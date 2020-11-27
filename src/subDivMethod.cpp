#include "subDivMethod.h"

Edge::Edge(Vertex *a, Vertex *b) {
	this->a = a;
	this->b = b;
	a->edgeList_ofPoint.push_back(this);
	b->edgeList_ofPoint.push_back(this);
	midPoint = (a->pointPos + b->pointPos) / 2.f;
}

void Edge::computeUpdatedEdgePoint() {
	glm::vec3 result = (faceList_ofEdge[0]->newFacePoint->pointPos + faceList_ofEdge[1]->newFacePoint->pointPos
		+ a->pointPos + b->pointPos) / 4.f;
	updatedEdgePoint = new Vertex(result);
}
void Edge::computeNewEdges()
{
	newEdge1 = new Edge(a->updateVertex, updatedEdgePoint);
	newEdge2 = new Edge(b->updateVertex, updatedEdgePoint);
}

//repeatitive pushback???
Face::Face(Edge *e0, Edge *e1, Edge *e2, Edge *e3){
	this->edge0 = e0;
	this->edge1 = e1;
	this->edge2 = e2;
	this->edge3 = e3;
	e0->faceList_ofEdge.push_back(this);
	e1->faceList_ofEdge.push_back(this);
	e2->faceList_ofEdge.push_back(this);
	e3->faceList_ofEdge.push_back(this);
	e0->a->faceList_ofPoint.push_back(this);
	e0->b->faceList_ofPoint.push_back(this);
	e2->a->faceList_ofPoint.push_back(this);
	e2->b->faceList_ofPoint.push_back(this);
}

void Face::computeFacePoint() {
	glm::vec3 result = (edge0->a->pointPos + edge0->b->pointPos + edge2->a->pointPos + edge2->b->pointPos) / 4.f;
	newFacePoint = new Vertex(result);
}

void Face::pushNewSurface()
{
	newEdge0 = new Edge(edge0->updatedEdgePoint, newFacePoint);
	newEdge1 = new Edge(edge1->updatedEdgePoint, newFacePoint);
	newEdge2 = new Edge(edge2->updatedEdgePoint, newFacePoint);
	newEdge3 = new Edge(edge3->updatedEdgePoint, newFacePoint);

	newFace0 = new Face(edge0->newEdge1, newEdge0, newEdge3, edge3->newEdge1);
	newFace1 = new Face(edge1->newEdge1, newEdge1, newEdge0, edge0->newEdge2);
	newFace2 = new Face(edge2->newEdge2, newEdge2, newEdge1, edge1->newEdge2);
	newFace3 = new Face(edge3->newEdge2, newEdge3, newEdge2, edge2->newEdge1);

}


Vertex::Vertex(glm::vec3 p) {
	this->pointPos = p;
}

void Vertex::computeUpdateVert()
{
	glm::vec3 R(0), F(0);
	int n = faceList_ofPoint.size();
	if (n != edgeList_ofPoint.size()) {
		return;
	}
	for (int i = 0; i < n; i++) {
		F += faceList_ofPoint[i]->newFacePoint->pointPos;
		R += edgeList_ofPoint[i]->midPoint;
	}
	float N = n;
	glm::vec3 result = (F/N + 2.f * R/N + (N - 3.f)*pointPos) / N;

	updateVertex = new Vertex(result);
}

treeMesh::treeMesh()
{
	edgeList_treeMesh.clear();
	faceList_treeMesh.clear();
	vertList_treeMesh.clear();
}

treeMesh::treeMesh(std::vector<glm::vec3> v)
{

	std::vector<glm::vec3>::iterator it = v.begin();
	treeCube *cube = new treeCube(*it, 0.14f);
	cubeList_treeMesh.push_back(cube);
	GenerateCubes(&v, ++it, cube);
	std::vector<treeCube*>::iterator itc = cubeList_treeMesh.begin();
	for (itc; itc < cubeList_treeMesh.end(); itc++) {
		(*itc)->GenerateFace();
		PushCube(*itc);
	}
}


treeCube::treeCube(glm::vec3 center, float scale)
{
	this->center = center;
	p0 = new Vertex(unitVert[0]* scale + center);
	p1 = new Vertex(unitVert[1] * scale + center);
	p2 = new Vertex(unitVert[2] * scale + center);
	p3 = new Vertex(unitVert[3] * scale + center);
	p4 = new Vertex(unitVert[4] * scale + center);
	p5 = new Vertex(unitVert[5] * scale + center);
	p6 = new Vertex(unitVert[6] * scale + center);
	p7 = new Vertex(unitVert[7] * scale + center);
	//MARK: CHANGE OF EDGE POINT ORDER
	e0 = new Edge(p0, p1); e1 = new Edge(p1, p2); e2 = new Edge(p3, p2); e3 = new Edge(p0, p3);
	e4 = new Edge(p0, p4); e5 = new Edge(p1, p5); e6 = new Edge(p2, p6); e7 = new Edge(p3, p7);
	e8 = new Edge(p4, p5); e9 = new Edge(p5, p6); e10 = new Edge(p7, p6); e11 = new Edge(p4, p7);
	//e0 = new Edge(p0, p1); e1 = new Edge(p1, p2); e2 = new Edge(p2, p3); e3 = new Edge(p3, p0);
	//l0 = new Edge(p0, q0); l1 = new Edge(p1, q1); l2 = new Edge(p2, q2); l3 = new Edge(p3, q3);
	//t0 = new Edge(q0, q1); t1 = new Edge(q1, q2); t2 = new Edge(q2, q3); t3 = new Edge(q3, q0);

	f4 = new Face(e1, e6, e9, e5);
	f5 = new Face(e3, e7, e11, e4);
}

void treeCube::GenerateFace()
{
	if(Up)
		f2 = new Face(e8, e9, e10, e11);
	if(Down)
		f0 = new Face(e0, e1, e2, e3);
	if(Right)
		f3 = new Face(e2, e6, e10, e7);
	if(Left)
		f1 = new Face(e0, e5, e8, e4);
}

void treeMesh::PushCube(treeCube * c)
{
	vertList_treeMesh.push_back(c->p0); vertList_treeMesh.push_back(c->p1); vertList_treeMesh.push_back(c->p2); vertList_treeMesh.push_back(c->p3);
	vertList_treeMesh.push_back(c->p4); vertList_treeMesh.push_back(c->p5); vertList_treeMesh.push_back(c->p6); vertList_treeMesh.push_back(c->p7);
	edgeList_treeMesh.push_back(c->e0); edgeList_treeMesh.push_back(c->e1); edgeList_treeMesh.push_back(c->e2); edgeList_treeMesh.push_back(c->e3);
	edgeList_treeMesh.push_back(c->e4); edgeList_treeMesh.push_back(c->e5); edgeList_treeMesh.push_back(c->e6); edgeList_treeMesh.push_back(c->e7);
	edgeList_treeMesh.push_back(c->e8); edgeList_treeMesh.push_back(c->e9); edgeList_treeMesh.push_back(c->e10); edgeList_treeMesh.push_back(c->e11);
	if(c->Down)  faceList_treeMesh.push_back(c->f0); 
	if(c->Left)	 faceList_treeMesh.push_back(c->f1); 
	if(c->Up)    faceList_treeMesh.push_back(c->f2);
	if(c->Right) faceList_treeMesh.push_back(c->f3); 
	faceList_treeMesh.push_back(c->f4); faceList_treeMesh.push_back(c->f5);
}

void treeMesh::ConnectCube(treeCube * c1, treeCube * c2)
{
	glm::vec3 dir = c2->center - c1->center;
	if (abs(dir.y) > abs(dir.x)) {
		if (dir.y > 0) {
			Edge *E0 = new Edge(c1->p4, c2->p0),
				*E1 = new Edge(c1->p5, c2->p1),
				*E2 = new Edge(c1->p6, c2->p2),
				*E3 = new Edge(c1->p7, c2->p3);
			edgeList_treeMesh.push_back(E0); edgeList_treeMesh.push_back(E1); edgeList_treeMesh.push_back(E2); edgeList_treeMesh.push_back(E3);
			Face *f0 = new Face(c1->e8, E1, c2->e0, E0),
				*f1 = new Face(c1->e9, E2, c2->e1, E1),
				*f2 = new Face(c1->e10, E2, c2->e2, E3),
				*f3 = new Face(c1->e11, E3, c2->e3, E0);
			faceList_treeMesh.push_back(f0); faceList_treeMesh.push_back(f1); faceList_treeMesh.push_back(f2); faceList_treeMesh.push_back(f3);
			c1->Up = false;
			c2->Down = false;
		}
		else
			ConnectCube(c2, c1);
		
	}
	else if(abs(dir.y) <= abs(dir.x)) {
		if (dir.x > 0) {
			Edge *E0 = new Edge(c1->p2, c2->p1),
				*E1 = new Edge(c1->p6, c2->p5),
				*E2 = new Edge(c1->p7, c2->p4),
				*E3 = new Edge(c1->p3, c2->p0);
			edgeList_treeMesh.push_back(E0); edgeList_treeMesh.push_back(E1); edgeList_treeMesh.push_back(E2); edgeList_treeMesh.push_back(E3);
			Face *f0 = new Face(c1->e2, E0, c2->e0, E3),
				*f1 = new Face(c1->e6, E1, c2->e5, E0),
				*f2 = new Face(c1->e10, E1, c2->e8, E2),
				*f3 = new Face(c1->e7, E2, c2->e4, E3);
			faceList_treeMesh.push_back(f0); faceList_treeMesh.push_back(f1); faceList_treeMesh.push_back(f2); faceList_treeMesh.push_back(f3);
			c1->Right = false;
			c2->Left = false;
		}
		else
			ConnectCube(c2, c1);
	}	
}

void treeMesh::GenerateCubes(std::vector<glm::vec3> *v, std::vector<glm::vec3>::iterator &it, treeCube* lastCube)
{
	int index = std::distance(v->begin(), it);
	treeCube* c1 = lastCube;
	for (it; it < v->end(); it++) {
		if (*it == MARK[1]) {
			GenerateCubes(v, ++it, c1);
		}
		else if (*it == MARK[0]) {
			return;
		}
		else {
			treeCube *cube = new treeCube(*it, 0.11f-0.002f*index);
			cubeList_treeMesh.push_back(cube);
			ConnectCube(c1, cube);
			c1 = cube;
		}
	}
}

treeMesh* treeMesh::CatmullClarkSubdiv() {
	
	treeMesh* outPut = new treeMesh();
	
	std::vector<Face*>::iterator itFace = faceList_treeMesh.begin();
	
	for (itFace; itFace < faceList_treeMesh.end(); itFace++) {
		(*itFace)->computeFacePoint();
		outPut->vertList_treeMesh.push_back((*itFace)->newFacePoint);
	}
	
	std::vector<Edge*>::iterator itEdge = edgeList_treeMesh.begin();
	
	for (itEdge; itEdge < edgeList_treeMesh.end(); itEdge++) {
		(*itEdge)->computeUpdatedEdgePoint();
		outPut->vertList_treeMesh.push_back((*itEdge)->updatedEdgePoint);
	}
	
	std::vector<Vertex*>::iterator itVertex = vertList_treeMesh.begin();
	
	for (itVertex; itVertex < vertList_treeMesh.end(); itVertex++) {
		(*itVertex)->computeUpdateVert();
		outPut->vertList_treeMesh.push_back((*itVertex)->updateVertex);
	}

	for (itEdge = edgeList_treeMesh.begin(); itEdge < edgeList_treeMesh.end(); itEdge++) {
		(*itEdge)->computeNewEdges();
		outPut->edgeList_treeMesh.push_back((*itEdge)->newEdge1);
		outPut->edgeList_treeMesh.push_back((*itEdge)->newEdge2);
	}

	for (itFace = faceList_treeMesh.begin(); itFace < faceList_treeMesh.end(); itFace++) {
		(*itFace)->pushNewSurface();
		outPut->edgeList_treeMesh.push_back((*itFace)->newEdge0);
		outPut->edgeList_treeMesh.push_back((*itFace)->newEdge1);
		outPut->edgeList_treeMesh.push_back((*itFace)->newEdge2);
		outPut->edgeList_treeMesh.push_back((*itFace)->newEdge3);

		outPut->faceList_treeMesh.push_back((*itFace)->newFace0);
		outPut->faceList_treeMesh.push_back((*itFace)->newFace1);
		outPut->faceList_treeMesh.push_back((*itFace)->newFace2);
		outPut->faceList_treeMesh.push_back((*itFace)->newFace3);
	}

	return outPut;
}


