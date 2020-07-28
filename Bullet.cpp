#include "Bullet.h"

Bullet::Bullet(glm::mat4 MBarrel, int fromTowerName, Hero &hero)
{
    this->mainBulletMatrix = MBarrel;
    this->dead = false;
	this->hero = &hero;

	switch (fromTowerName)
	{
	case 0:
		initialize(&GlobalProperties::lunaBulletParams);
		break;
	case 1:
		initialize(&GlobalProperties::calypsoBulletParams);
		break;
	case 2:
		initialize(&GlobalProperties::narviBulletParams);
		break;
	default:
		Logger::error("Bullet", "Błąd pobierania mapy parametrów dla: " + fromTowerName);
		initialize(&GlobalProperties::lunaBulletParams);
		break;
	}
}
void Bullet::initialize(const map<string, string>* params)
{
	try
	{
		this->name = params->find("name")->second;
		this->damage = stoi(params->find("damage")->second);
		this->maxDistance = stof(params->find("maxDistance")->second);
		this->hitDistance = stof(params->find("hitDistance")->second);
		this->speed = stof(params->find("bulletSpeed")->second);

		float xB = stof(params->find("bulletPosX")->second);
		float yB = stof(params->find("bulletPosY")->second);
		float zB = stof(params->find("bulletPosZ")->second);
		this->position = glm::vec3(xB, yB, zB);

		xB = stof(params->find("bulletSizX")->second);
		yB = stof(params->find("bulletSizY")->second);
		zB = stof(params->find("bulletSizZ")->second);
		this->size = glm::vec3(xB, yB, zB);
	}
	catch (const std::exception e)
	{
		Logger::error("Bullet", "Błąd pobierania parametrów z mapy ");
	}
}

glm::mat4 Bullet::goForIt()
{
    this->position.x -= speed; //na koncu, aby zaczynala w dobrym miejscu
    glm::mat4 bulletMatrix = glm::translate(this->mainBulletMatrix, this->position);
	bulletMatrix = glm::scale(bulletMatrix, this->size);
	hit();
    return bulletMatrix;
}
bool Bullet::hit()
{
	float actualDistance = 0;
	
	glm::mat4 bulletMatrix = glm::translate(this->mainBulletMatrix, this->position);
	actualDistance = glm::distance(hero->getMainHeroMatrix()[3], bulletMatrix[3]);

	if (actualDistance < hitDistance) {
		this->dead = true;
		hero->getDamage(damage);
	}
	return true;
}

bool Bullet::isDead()
{
    if (this->position.x <= maxDistance) {
        this->dead = true;
    }
    return this->dead;
}
