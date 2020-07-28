#pragma once
#include<map>

using namespace std;

namespace GlobalProperties {

	//MAP
	static int coins = 100;
	//MAP

	//HERO
	static const map<string, string> hunterParams = {
		{"name", "Hunter"},
		{"heroSpeed", "0.1"},
		{"healthPoints", "100"},
		{"rightFacing", "180"},
		{"value", "10"},

		{"heroPosX", "7.0"},
		{"heroPosY", "-1.0"},
		{"heroPosZ", "-5.0"},

		{"heroSizX", "1.0"},
		{"heroSizY", "1.0"},
		{"heroSizZ", "1.0"}
	};
	static const map<string, string> archerParams = {
		{"name", "Archer"},
		{"heroSpeed", "0.2"},
		{"healthPoints", "100"},
		{"rightFacing", "180"},
		{"value", "20"},

		{"heroPosX", "7.0"},
		{"heroPosY", "-1.0"},
		{"heroPosZ", "-5.0"},

		{"heroSizX", "0.5"},
		{"heroSizY", "0.5"},
		{"heroSizZ", "1.5"}
	};
	static const map<string, string> orionParams = {
		{"name", "Orion"},
		{"heroSpeed", "0.3"},
		{"healthPoints", "100"},
		{"rightFacing", "180"},
		{"value", "30"},

		{"heroPosX", "7.0"},
		{"heroPosY", "-1.0"},
		{"heroPosZ", "-5.0"},

		{"heroSizX", "0.5"},
		{"heroSizY", "2.0"},
		{"heroSizZ", "0.5"}
	};
	//HERO

	//TOWER
	static const map<string, string> lunaParams = {
		{"name", "Luna"},
		{"reloadingTime", "90"},
		{"dangerZone", "1"},
		{"value", "100"},

		{"barrelPosX", "-1.5"},
		{"barrelPosY", "1.5"},
		{"barrelPosZ", "0.0"},

		{"barrelSizX", "1.5"},
		{"barrelSizY", "0.2"},
		{"barrelSizZ", "0.2"}
	};
	static const map<string, string> calypsoParams = {
		{"name", "Calypso"},
		{"reloadingTime", "90"},
		{"dangerZone", "1"},
		{"value", "250"},

		{"barrelPosX", "-1.5"},
		{"barrelPosY", "1.5"},
		{"barrelPosZ", "0.0"},

		{"barrelSizX", "1.5"},
		{"barrelSizY", "0.2"},
		{"barrelSizZ", "0.2"}
	};
	static const map<string, string> narviParams = {
		{"name", "Narvi"},
		{"reloadingTime", "10"},
		{"dangerZone", "2"},
		{"value", "400"},

		{"barrelPosX", "-1.5"},
		{"barrelPosY", "1.5"},
		{"barrelPosZ", "0.0"},

		{"barrelSizX", "1.5"},
		{"barrelSizY", "0.2"},
		{"barrelSizZ", "0.2"}
	};
	//TOWER

	//BARREL
	static const float angleBarrelCorrect = 1.45; //never used
	//BARREL

	//BULLET
	static const map<string, string> lunaBulletParams = {
		{"name", "TowerLight"},
		{"bulletSpeed", "0.4"},
		{"damage", "50"},
		{"maxDistance", "-30.0f"},
		{"hitDistance", "0.35f"},

		{"bulletPosX", "0.3"},
		{"bulletPosY", "0.0"},
		{"bulletPosZ", "0.0"},

		{"bulletSizX", "0.2"},
		{"bulletSizY", "1.0"},
		{"bulletSizZ", "1.0"}

	};
	static const map<string, string> calypsoBulletParams = {
		{"name", "TowerMedium"},
		{"bulletSpeed", "0.4"},
		{"damage", "10"},
		{"maxDistance", "-30.0f"},
		{"hitDistance", "0.18f"},

		{"bulletPosX", "0.3"},
		{"bulletPosY", "0.0"},
		{"bulletPosZ", "0.0"},

		{"bulletSizX", "0.2"},
		{"bulletSizY", "1.0"},
		{"bulletSizZ", "1.0"}

	};
	static const map<string, string> narviBulletParams = {
		{"name", "TowerPremium"},
		{"bulletSpeed", "0.8"},
		{"damage", "10"},
		{"maxDistance", "-30.0f"},
		{"hitDistance", "0.18f"},
		
		{"bulletPosX", "0.3"},
		{"bulletPosY", "0.0"},
		{"bulletPosZ", "0.0"},

		{"bulletSizX", "0.2"},
		{"bulletSizY", "1.0"},
		{"bulletSizZ", "1.0"}
	};
	//BULLET
}


