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

//Gdyby w trakcie wystrzału zmniejszac y to bedzie ladnie opadał hueh na koniec do zrobienia
class Bullet : public Animations
{
private:
	glm::mat4 mainBulletMatrix; 
	glm::vec3 position;
	glm::vec3 size;

	string name;
	int damage;
	float speed;
	float maxDistance;
	float hitDistance;
	bool dead;
	Hero *hero;

	void initialize(const map<string, string>* params);

public:
	Bullet(glm::mat4 MBarrel, int fromTowerName, Hero &hero);

	glm::mat4 goForIt();
	bool hit();
	bool isDead();
};

