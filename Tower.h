#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "iostream"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <array>
#include "gltext.h"
#include <Hero.h>
#include <map>
#include <GlobalProperties.h>
#include <Animations.h>

using namespace std;

class Tower : public Animations
{
private:
	glm::mat4 mainTowerPosition;
	glm::vec3 position;
	glm::vec3 size;

	string name;
	int nameEnum;
	bool creating;
	bool haveTarget;
	bool isReloaded;
	int reloadingTime[2];
	float dangerZone;
	int value;

	glm::vec3 positionBarrel;
	glm::vec3 sizeBarrel;
	float angle;

	void initialize(const map<string, string> * params);
	void reloading();

public:
	static enum names {
		LUNA,
		CALYPSO,
		NARVI
	};

	Tower(glm::mat4 Mmodel, int nameEnum);
	void move(bool endCreate, float x, float y , float z);
	glm::mat4 getPosition();
	glm::mat4 getBarrelPosition(glm::vec3 posHero);
	bool canTakeShoot();
	bool isHeroInDangerZone(glm::mat4 posHero);
	int getNameEnum();
	string getName();
	int getValue();
};

