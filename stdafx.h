#pragma once

#ifndef STDAFX_H
#define STDAFX_H

//glm
#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp> // glm::pi
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

//some constants
const glm::vec3 MARK[3] = { glm::vec3(1111.f), glm::vec3(99.f), glm::vec3(0.f) };

const float scalar = 0.1f;

const glm::vec3 unitVert[8] = {glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1),
								glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1)	};

#endif
