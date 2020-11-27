#include "Tree.h"



Tree::Tree(float angle, float size)
{
	treeNode.clear();
	treeNode.push_back(MARK[2]);
	this->slope = angle;
	this->nodeDist = size;
	initTreeNode(0, MARK[2]);
}

glm::vec3 Tree::computeNextNode(glm::vec3 v, float dir) {
	glm::mat4 R1 = glm::rotate(glm::radians(dir), glm::vec3(0, 0, 1));
	glm::mat4 R2 = glm::rotate(glm::radians(dir), glm::vec3(0.01, 0.0, 0.0));
	glm::vec3 d = glm::vec3((R2*R1) * glm::vec4(0, 1, 0, 0));
	d = normalize(d)*nodeDist;
	return v + 0.66f*d;
}

void Tree::initTreeNode(float dir, glm::vec3 lastPoint) {

	char* pattern = "fffff[+f[-ff[+f[-f[+f]]+f-f]-ff[+f[-f-f++f[+]-f]f[+f]]]]";

	float nextNodeDir = dir;	
	
	glm::vec3 p = lastPoint;
	
	for ( letterCount; letterCount < strlen(pattern); letterCount++) {
		if (pattern[letterCount] == 'f') {
			p = computeNextNode(p, nextNodeDir);
			treeNode.push_back(p);
		}
		else if (pattern[letterCount] == '+')
			nextNodeDir += slope;
		else if (pattern[letterCount] == '-')
			nextNodeDir -= slope;
		else if (pattern[letterCount] == '[') {
			letterCount++;
			treeNode.push_back(MARK[1]);
			initTreeNode(nextNodeDir, p);
		}
		else if (pattern[letterCount] == ']') {
			treeNode.push_back(MARK[0]);
			return;
		}
	}
	treeNode.push_back(MARK[0]);
}




