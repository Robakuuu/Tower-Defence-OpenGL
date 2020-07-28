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
#include <GlobalProperties.h>
#include <Logger.h>
#include <sstream>
#include <Animations.h>

class Hero: public Animations
{
private:
	glm::mat4 mainHeroMatrix;
	glm::vec3 position;
	glm::vec3 size;

	string name;
	int direction;
	float facing;
	float rightFacing;
	float stepSize;
	int level;
	int maxLevel;
	float borders; //wysStolu - brzegi / liczba poziomow
	float lastBorder;
	bool dead;
	bool endGame;
	int healthPoints;
	int value;

	enum Direction {
		right,
		left,
		downL,
		downR
	};

	void walking();	//MGame, facing(enum), stepSize
	void goRight(); //set position and facing
	void goLeft();
	void goDown();
	bool checkIfNextLevel(float);
	void initialize(const map<string, string>* params);

	glm::vec3 getVec3(float, float, float);

public:
	static enum names {
		HUNTER,
		ARCHER,
		ORION
	};

	Hero();
	Hero(vector<int> frames, int nameEnum);
	glm::mat4 getMainHeroMatrix(); 
	glm::mat4 getPosition(glm::mat4);
	glm::vec3 getVec3Position();
	
	bool isEndGame();
	bool isDead();
	void getDamage(int);
	int getvalue();
	string getName();
};

