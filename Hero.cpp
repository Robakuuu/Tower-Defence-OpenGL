#include "Hero.h"

Hero::Hero(){}

Hero::Hero(vector<int> frames, int nameEnum)
{
	this->direction = 0;
	this->level = 0;
	this->maxLevel = 4;
	this->borders = 12.5f;//(64.0 - 2x7.0) / 4; //wysStolu - brzegi / liczba poziomow
	this->lastBorder = 7.0f;
	this->dead = false;
	this->endGame = false;
	this->animationFrames = frames;

	switch (nameEnum)
	{
	case 0:
		initialize(&GlobalProperties::hunterParams);
		break;
	case 1:
		initialize(&GlobalProperties::archerParams);
		break;
	case 2:
		initialize(&GlobalProperties::orionParams);
		break;
	default:
		Logger::error("Hero", "Błąd pobierania mapy parametrów dla: " + name);
		initialize(&GlobalProperties::hunterParams);
		break;
	}
}
void Hero::initialize(const map<string, string>* params)
{
	try
	{
		this->name = params->find("name")->second;
		this->healthPoints = stoi(params->find("healthPoints")->second);
		this->stepSize = stof(params->find("heroSpeed")->second);
		this->rightFacing = stof(params->find("rightFacing")->second);
		this->facing = rightFacing;
		this->value = stoi(params->find("value")->second);

		float xB = stof(params->find("heroPosX")->second);
		float yB = stof(params->find("heroPosY")->second);
		float zB = stof(params->find("heroPosZ")->second);
		this->position = glm::vec3(xB, yB, zB);

		xB = stof(params->find("heroSizX")->second);
		yB = stof(params->find("heroSizY")->second);
		zB = stof(params->find("heroSizZ")->second);
		this->size = glm::vec3(xB, yB, zB);
		Logger::info("Hero", "Nowy Hero: " + name);
	}
	catch (const std::exception e)
	{
		Logger::error("Hero", "Błąd pobierania parametrów z mapy ");
	}
}

glm::mat4 Hero::getMainHeroMatrix()
{
	return this->mainHeroMatrix;
}

void Hero::walking()
{
	if (direction == Direction::right) {
		goRight();
	}
	else if (direction == Direction::left) {
		goLeft();
	}
	else if (direction == Direction::downL) {
		goDown();
	}
	else if (direction == Direction::downR) {
		goDown();
	}
}

void Hero::goRight()
{
	float z = position.z - stepSize;
	position = getVec3(position.x, position.y, z);
	if (position.z < -57.0f) {
		if (maxLevel == level) {
			Logger::info("Hero", "endGame");
			this->dead = true;
			this->endGame = true;
		}
		this->direction = Direction::downR;
		this->facing -= 90;
	}
}

void Hero::goLeft()
{
	float z = position.z + stepSize;
	position = getVec3(position.x, position.y, z);
	if (position.z > -5.0f) {
		this->direction = Direction::downL;
		this->facing += 90;
	}
}

bool Hero::checkIfNextLevel(float x) 
{
	float currentBorder = lastBorder + borders;
	if (x > currentBorder) {
		this->level += 1;
		this->lastBorder = currentBorder;
		return true;
	}
	return false;
}

void Hero::goDown()
{
	float x = position.x + stepSize;
	position = getVec3(x, position.y, position.z);

	if (checkIfNextLevel(x)) {
		if (direction == Direction::downL) {
			this->direction = Direction::right;
			this->facing = rightFacing;
		}
		if (direction == Direction::downR) {
			this->direction = Direction::left;
			this->facing -= 90;
		}
	}
}

glm::vec3 Hero::getVec3(float x, float y, float z)
{
	return glm::vec3(x, y, z);
}

glm::mat4 Hero::getPosition(glm::mat4 Mmodel)
{
	walking();

	Mmodel = glm::translate(Mmodel, position);
	Mmodel = glm::rotate(Mmodel, facing * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	Mmodel = glm::scale(Mmodel, size);

	this->mainHeroMatrix = Mmodel;
	return Mmodel;
}

glm::vec3 Hero::getVec3Position()
{
	return this->position;
}

bool Hero::isEndGame() 
{
	return this->endGame;
}

void Hero::getDamage(int damage)
{
	this->healthPoints -= damage;
}
int Hero::getvalue()
{
	return this->value;
}
string Hero::getName()
{
	return this->name;
}
bool Hero::isDead() {
	if (healthPoints <= 0) {
		this->dead = true;
	}
	return this->dead;
}
