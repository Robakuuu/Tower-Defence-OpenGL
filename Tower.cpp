#include "Tower.h"

Tower::Tower(glm::mat4 Mmodel, int nameEnum2)
{
	this->position = glm::vec3(13.25f, 4.0f, -15.0f);
	this->mainTowerPosition = Mmodel;
	this->creating = true;
	this->haveTarget = false;
	this->isReloaded = true;
	this->reloadingTime[0] = 0;
	this->angle = 0;
	this->nameEnum = nameEnum2;

	switch (nameEnum2)
	{
	case 0:
		initialize(&GlobalProperties::lunaParams);
		break;
	case 1:
		initialize(&GlobalProperties::calypsoParams);
		break;
	case 2:
		initialize(&GlobalProperties::narviParams);
		break;
	default:
		Logger::error("Tower", "Błąd pobierania mapy parametrów dla: " + nameEnum2);
		initialize(&GlobalProperties::lunaParams);
		break;
	}
}
void Tower::initialize(const map<string, string> * params) 
{
	try
	{
		this->name = params->find("name")->second;
		this->reloadingTime[1] = stoi(params->find("reloadingTime")->second);
		this->dangerZone = stof(params->find("dangerZone")->second);
		this->value = stoi(params->find("value")->second);

		float xB = stof(params->find("barrelPosX")->second);
		float yB = stof(params->find("barrelPosY")->second);
		float zB = stof(params->find("barrelPosZ")->second);
		this->positionBarrel = glm::vec3(xB, yB, zB);

		xB = stof(params->find("barrelSizX")->second);
		yB = stof(params->find("barrelSizY")->second);
		zB = stof(params->find("barrelSizZ")->second);
		this->sizeBarrel = glm::vec3(xB, yB, zB);
	}
	catch (const std::exception e)
	{
		Logger::error("Tower", "Błąd pobierania parametrów z mapy ");
	}
}
void Tower::move(bool endCreate, float x, float y, float z) 
{
	if (!endCreate) {
		if (x != 0 && position.x + x < 51 && position.x + x > 13) {
			this->position.x += x;
		} 
		else if (z != 0 && position.z + z < -14 && position.z + z > -50) {
			this->position.z += z;
		}
	}
	else {
		this->position.y = 0;
		this->creating = false;
		this->mainTowerPosition = glm::translate(mainTowerPosition, position);
	}
}
glm::mat4 Tower::getPosition() 
{
	if (creating){
		return glm::translate(mainTowerPosition, position);
	}
	return mainTowerPosition;
}
glm::mat4 Tower::getBarrelPosition(glm::vec3 posHero)
{
	if (haveTarget){
		//z = arctg(a) = arctg((Yp - Yk) / (Xp - Xk))
		angle = atan2((posHero.x - position.x), (posHero.z - position.z));
		this->angle += 1.55; //default 1.5 ale wiecej aby trafialy 
	}
	else {
		this->angle -= 0.0006;
	}
	reloading();

	glm::mat4 mainBarrelPosition = glm::rotate(this->mainTowerPosition, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	mainBarrelPosition = glm::translate(mainBarrelPosition, this->positionBarrel);
	mainBarrelPosition = glm::scale(mainBarrelPosition, this->sizeBarrel);
	return mainBarrelPosition;
}
void Tower::reloading() 
{
	if (!isReloaded) {
		this->reloadingTime[0] -= 1;
		if (reloadingTime[0] <= 0) {
			this->isReloaded = true;
		}
	}
}
bool Tower::canTakeShoot()
{
	if (isReloaded) {
		this->reloadingTime[0] = reloadingTime[1];
		this->isReloaded = false;
		return true;
	}
	return false;
}

bool Tower::isHeroInDangerZone(glm::mat4 posHero) 
{
	float distance = glm::distance(posHero[3], mainTowerPosition[3]);
	if (!creating) {
		if (distance <= dangerZone) {
			this->haveTarget = true;
			return true;
		}
	}
	this->haveTarget = false;
	return false;
}

int Tower::getNameEnum()
{
	return this->nameEnum;
}

string Tower::getName()
{
	return this->name;
}

int Tower::getValue()
{
	return this->value;
}

