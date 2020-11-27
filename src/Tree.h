#pragma once

#include "stdafx.h"


class treeNode
{
public:
	treeNode(float angle, float size);
	std::vector<glm::vec3> treeNodeList;

	float slope;
	float nodeDist;

	int letterCount = 0;
	glm::vec3 computeNextNode(glm::vec3 v, float dir);
	void initTreeNode(float dir, glm::vec3 lastPoint);
};


