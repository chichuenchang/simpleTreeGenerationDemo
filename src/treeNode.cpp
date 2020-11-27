#include "treeNode.h"



treeNode::treeNode(float angle, float size)
{
	treeNodeList.clear();
	treeNodeList.push_back(MARK[2]);
	this->slope = angle;
	this->nodeDist = size;
	initTreeNode(0, MARK[2]);
}

glm::vec3 treeNode::computeNextNode(glm::vec3 v, float dir) {
	glm::mat4 R1 = glm::rotate(glm::radians(dir), glm::vec3(0, 0, 1));
	glm::mat4 R2 = glm::rotate(glm::radians(dir), glm::vec3(0.01, 0.0, 0.0));
	glm::vec3 d = glm::vec3((R2*R1) * glm::vec4(0, 1, 0, 0));
	d = normalize(d)*nodeDist;
	return v + 0.66f*d;
}

void treeNode::initTreeNode(float dir, glm::vec3 lastPoint) {

	float nextNodeDir = dir;	
	
	glm::vec3 p = lastPoint;
	
	//char* pattern = "ttttt[[]+t[][-tt[[]+t[-t[[]+[]t]]+t-t]-tt[+t[-t-t++t[+]-t]t[+t]]]]";
	//char* pattern = "ttt[[][]+t[][-t[[][][]+t[-t[[][[][[][-t]]]]][+t]]-tt[t]";
	char* pattern = "ttt[+t[][-t[+t[-t[[][[[-t]]]]][+t]]-tt[t]";
	for ( letterCount; letterCount < strlen(pattern); letterCount++) {
		if (pattern[letterCount] == 't') {
			p = computeNextNode(p, nextNodeDir);
			treeNodeList.push_back(p);
		}
		else if (pattern[letterCount] == '+')
			nextNodeDir += slope;
		else if (pattern[letterCount] == '-')
			nextNodeDir -= slope;
		else if (pattern[letterCount] == '[') {
			letterCount++;
			treeNodeList.push_back(MARK[1]);
			initTreeNode(nextNodeDir, p);
		}
		else if (pattern[letterCount] == ']') {
			treeNodeList.push_back(MARK[0]);
			return;
		}
	}
	treeNodeList.push_back(MARK[0]);
}




