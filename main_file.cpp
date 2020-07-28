/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define _CRT_SECURE_NO_DEPRECATE

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
#include <time.h>
#include <hero.h>
#include <windows.h>
#include <filesystem>
#include "gltext.h"
#include <Tower.h>
#include <Bullet.h>
#include <GlobalProperties.h>
#include <Logger.h>
#define GLT_IMPLEMENTATION

using namespace std;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float WindowX = 1920;
float WindowY = 1080;
float fov = 45.0f;
const float cameraSpeed = 0.1f;

vector<string>sciezkiDoObiektow;

//Wektory przejsciowe
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; 

std::vector<unsigned short> indices;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;

std::vector<float> wektor_indexed_vertices;

//liczniki wiezcholkow obiektow zgodne z numeracja obiektow
vector<int>Tablicelicznik1;
int tablicalicznik1=0;
vector<int>Tablicelicznik;
int licznik=0;
//

int numerswiata = 0;//numer po ktorym wiadomo od ktorego momentu mozna renderowac poszczegolne obiekty
int numerobiektu = 0;		 
//Wektory przechowywujace obiekty zgodnie z ich kolejnoscia wczytania
vector<vector<float>>TabliceWektorowX;
vector<vector<float>>TabliceUvX;
vector<vector<float>>TabliceNormalX;
vector<vector<float>>TabliceKolorowX;
//
glm::mat4 M = glm::mat4(1.0f); //Macierz swiata RTS 
glm::mat4 MacierzStolu = glm::mat4(1.0f); 

vector<Hero> heros;
vector<Tower> towers;
vector<Bullet> bullets;

glm::mat4 Mhero;
glm::mat4 Mtower;
glm::mat4 Mbarrel;
glm::mat4 Mbullet;
//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
bool isCreatingTower = false;
bool isGameON = false;
clock_t tStartGame, tLastHero, tLastLevel;
float levelGame = 1.0;
//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend

struct Obiekty
{
	string nazwaObiektu;
	int aktualnyNumerKlatki;//numery obiektu = numertekstury 
	int startAnimacji;
	int koniecAnimacji;
	
	
}Obiekt;
vector<Obiekty>WektorObiektowSwiata; //wektor przechowywujacy numery obiektow = numerytekstur																		 
const float radius = 10.0f;
float obrot = 0.1f;
float camX = 0.0f;
float camZ = 3.0f;
glm::vec3 cameraPos = glm::vec3(camX, 3.0f, camZ);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint doodczytu;
GLuint tex;
GLuint tex2; //Uchwyt – deklaracja globalna
vector<GLuint> wektortekstur;
vector<GLuint> wektortekstur2;

float jazda = 1.0f;
float jazda2 = 1.0f;
float jazda3 = 1.0f;
float jazda4 = 1.0f;

int wlacz_koordynaty = 1;
float speed = PI; //[radiany/s]
//Procedura obsługi błędów
int klatkaz = 0;
int ogien = 0;

float lastX = WindowX / 2, lastY = WindowY / 2;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

GLuint readTexture(const char* filename);
void error_callback(int error, const char* description);
void initOpenGLProgram(GLFWwindow* window);
void freeOpenGLProgram(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods);
void rysujobiekt(int numerobiektu);
void wyswietlpolozenie();
void animacja_ogniska(int migotanie);
void drawScene(GLFWwindow* window, float angle, int numerobiektu);
int removeDeadBodies();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void przybliz();
void oddal();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void wektorowanie();
void przygotuj_obiektVer2(const char* path, int numerobiektu);
void wczytaj_obiekty();
void wyrenderuj_swiat(int numerobiektu, glm::mat4 M);
void wczytajObiektySwiata(int ostatniNumerSwiata);
int getNumerKlatki(string nazwaObiektu, int aktualnyNumerKlatki);
int przypiszKlatke(string nazwaObiektu);
vector<string>czytaj_sciezke(vector<string>wektor);			
bool createNewTower(int nameEnum);
void createNewHero(int nameEnum);
ShaderProgram *sp;

int main(void)
{
	Logger::info("main_file", "Start gry");
	srand(time(NULL));

	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(WindowX, WindowY, "Tower Defence v2.32 alpha", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okdrgdrgna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}
	initOpenGLProgram(window); //Operacje inicjujące

	glfwSetTime(0);
	srand(time(NULL));
	wczytaj_obiekty();
	float angle = 0;

	//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
	system("cls");
	Logger::info("GAME", "NACISNIJ ENTER, ABY ROPOCZAC GRE...");

	//Główna pętla
	while (!glfwWindowShouldClose(window)) {
		if (isGameON) {
			if ((double(clock() - tLastLevel) / double(CLOCKS_PER_SEC)) > 60) {
				levelGame++;
				Logger::info("GAME", "NEW LEVEL " + to_string((int)levelGame));
				tLastLevel = clock();
			}
			if ((double(clock() - tLastHero) / double(CLOCKS_PER_SEC)) > (3 / levelGame)) {
				tLastHero = clock();
				int zakresPostaci = 1;
				if (levelGame > 1) zakresPostaci++;
				if (levelGame > 2) zakresPostaci++;

				switch ((rand() % zakresPostaci) + 1)
				{
				case 1:
					createNewHero(Hero::HUNTER);
					break;
				case 2:
					createNewHero(Hero::ARCHER);
					break;
				case 3:
					createNewHero(Hero::ORION);
					break;
				default:
					Logger::error("main_file", "Blad zakresu postaci");
					break;
				}
			}
		}
		//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend
		angle += speed * glfwGetTime();

		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);

		glfwSetTime(0);

		glfwSetKeyCallback(window, key_callback);
		drawScene(window, angle, numerobiektu);//liczba obiektow
		glfwPollEvents();

	}
	//Usunięcie tekstury z pamięci karty graficznej – w freeOpenGLProgram
	glDeleteTextures(1, &tex);
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
bool createNewTower(int nameEnum) {
	const map<string, string>* params;
	int val = 99999;

	switch (nameEnum)
	{
	case 0:
		params = &GlobalProperties::lunaParams;
		break;
	case 1:
		params = &GlobalProperties::calypsoParams;
		break;
	case 2:
		params = &GlobalProperties::narviParams;
		break;
	default:
		params = &GlobalProperties::lunaParams;
		Logger::error("main_file", "Błąd pobierania wartosci dla: " + nameEnum);
		break;
	}
	val = stoi(params->find("value")->second);
	//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
	if (val <= GlobalProperties::coins) {
		towers.push_back(Tower(MacierzStolu, nameEnum));
		return true;
	}
	//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend
	else {
		Logger::info("Wallet", "Nie stac cie na to! Pod F13 jest darmowy gold!");
		Logger::info("Wallet", "Masz: " + to_string(GlobalProperties::coins));
		return false;
	}
}
void createNewHero(int nameEnum) {
	vector<int> animationFrames;
	switch (nameEnum)
	{
	case 0:
		animationFrames.push_back(przypiszKlatke("kostka")); //W case'ach oczywiscie kolejne rodzaje hero
		animationFrames.push_back(przypiszKlatke("RobotRece"));
		break;
	case 1:
		animationFrames.push_back(przypiszKlatke("kostka"));
		animationFrames.push_back(przypiszKlatke("RobotRece"));
		break;
	case 2:
		animationFrames.push_back(przypiszKlatke("kostka"));
		animationFrames.push_back(przypiszKlatke("RobotRece"));
		break;
	default:
		animationFrames.push_back(przypiszKlatke("kostka"));
		animationFrames.push_back(przypiszKlatke("RobotRece"));
		Logger::error("main_file", "Błąd pobierania startowej animacji dla: " + nameEnum);
		break;
	}
	//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
	heros.push_back(Hero(animationFrames, nameEnum));
}
//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	cout << "ERROR:" << error << "\n";
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	//glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//initShaders();
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	MacierzStolu = glm::scale(M, glm::vec3((float)0.1, 0.1, 0.1));
	MacierzStolu = glm::translate(MacierzStolu, glm::vec3((float)-14, 6, -63));//X Y Z lewy gorny rog

}
//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}
void youLoseEndGame() {
	towers.clear();
	heros.clear();
	bullets.clear();
	isCreatingTower = false;
	isGameON = false;
	system("cls");
	Logger::info("GAME", "Ostatnia gra - czas: " + to_string(double(clock() - tStartGame) / double(CLOCKS_PER_SEC)));
	Logger::info("GAME", "Ostatnia gra - wynik: " + to_string(GlobalProperties::coins));
	Logger::info("GAME", "Ostatnia gra - level: " + to_string(levelGame));
	Logger::info("GAME", "NACISNIJ ENTER, ABY ROPOCZAC GRE...");
}
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
		//GAME
		if (isGameON) {
			if (isCreatingTower) {
				Tower* tower = &towers.back();
				if (key == GLFW_KEY_LEFT) {
					tower->move(false, 0, 0, 3.6);
				}
				if (key == GLFW_KEY_RIGHT) {
					tower->move(false, 0, 0, -3.6);
				}
				if (key == GLFW_KEY_UP) {
					tower->move(false, -12.5, 0, 0);
				}
				if (key == GLFW_KEY_DOWN) {
					tower->move(false, 12.5, 0, 0);
				}
				if (key == GLFW_KEY_ENTER) {
					isCreatingTower = false;
					tower->move(true, 0, 0, 0);
					GlobalProperties::coins -= tower->getValue();
					Logger::info("main_file", "Nowy Tower: " + tower->getName());
					Logger::info("Wallet", "Pozostalo: " + to_string(GlobalProperties::coins));
				}
				if (key == GLFW_KEY_ESCAPE) {
					isCreatingTower = false;
					towers.pop_back();
				}
			}
			else if (key == GLFW_KEY_1 && !isCreatingTower) {
				isCreatingTower = createNewTower(Tower::LUNA);
			}
			else if (key == GLFW_KEY_2 && !isCreatingTower) {
				isCreatingTower = createNewTower(Tower::CALYPSO);
			}
			else if (key == GLFW_KEY_3 && !isCreatingTower) {
				isCreatingTower = createNewTower(Tower::NARVI);
			}
			else if (key == GLFW_KEY_4) {
				youLoseEndGame();
			}
			else if (key == GLFW_KEY_ENTER) {
				Logger::info("Wallet", "Masz: " + to_string(GlobalProperties::coins) + " golda");
			}
		}
		else {
			if (key == GLFW_KEY_ENTER) {
				isGameON = true;
				levelGame = 1;
				GlobalProperties::coins = 100;
				Logger::info("GAME", "START GRY");
				tStartGame = clock();
				tLastLevel = clock();
				tLastHero = clock();
			}
		}
		//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend
		//GAME
		if (key == GLFW_KEY_ESCAPE && !isCreatingTower) {
			Logger::info("main_file", "Klawisz ESC, koniec gry");
			exit(EXIT_SUCCESS);
		}
		if (key == GLFW_KEY_A) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (key == GLFW_KEY_D) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (key == GLFW_KEY_S) {
			cameraPos -= cameraSpeed * cameraUp;
		}
		if (key == GLFW_KEY_W) {
			cameraPos += cameraSpeed * cameraUp;
		}
		if (key == GLFW_KEY_EQUAL) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda++;
		}
		if (key == GLFW_KEY_MINUS) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda--;
		}
		if (key == GLFW_KEY_0) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda2++;
		}
		if (key == GLFW_KEY_9) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda2--;
		}
		if (key == GLFW_KEY_8) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda3++;
		}
		if (key == GLFW_KEY_7) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda3--;
		}
		
		if (key == GLFW_KEY_6) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda4 -= 0.1f;
		}
		if (key == GLFW_KEY_5) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda4 += 0.1f;
		}
		if (key == GLFW_KEY_Q) {
			//cameraPos += cameraSpeed * cameraFront;
			wlacz_koordynaty++;
			system("CLS");
		}
	}
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_A)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (key == GLFW_KEY_D) {
			
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (key == GLFW_KEY_S) {
			cameraPos -= cameraSpeed * cameraUp;
		}
		if (key == GLFW_KEY_W) {
			cameraPos += cameraSpeed * cameraUp;
		}
		if (key == GLFW_KEY_EQUAL) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda++;
		}
		if (key == GLFW_KEY_MINUS) {
			//cameraPos -= cameraSpeed * cameraFront;
			jazda--;
		}
		if (key == GLFW_KEY_0) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda2++;
		}
		if (key == GLFW_KEY_9) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda2--;
		}
		if (key == GLFW_KEY_8) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda3++;
		}
		if (key == GLFW_KEY_7) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda3--;
		}
		if (key == GLFW_KEY_6) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda4 -= 0.1f;
		}
		if (key == GLFW_KEY_5) {
			//cameraPos += cameraSpeed * cameraFront;
			jazda4 += 0.1f;
		}
		
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) printf("puszczone W\n");
		if (key == GLFW_KEY_S) printf("puszczone S\n");
		if (key == GLFW_KEY_A) printf("puszczone A\n");
		if (key == GLFW_KEY_D) printf("puszczone D\n");
	}
}
void rysujobiekt(int numerobiektu)
{
	//spTextured->use();
	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glEnableVertexAttribArray(sp->a("color"));

	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &TabliceWektorowX[numerobiektu][0]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &TabliceNormalX[numerobiektu][0]);
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &TabliceUvX[numerobiektu][0]);
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, &TabliceKolorowX[numerobiektu][0]);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);


	glDrawArrays(GL_TRIANGLES, 0, Tablicelicznik1[numerobiektu]);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("color"));
	glClearColor(0.1, 0.1, 0.1, 0);
}
//Procedura rysująca zawartość sceny
void wyswietlpolozenie()
{
	system("CLS");
	cout << "X:" << jazda << "\n";
	cout << "Y:" << jazda2 << "\n";
	cout << "Z:" << jazda3 << "\n";
	cout << "Wspolczynnik swiatla:" << jazda4 << "\n";
}
void animacja_ogniska(int migotanie)
{

	if (migotanie % 12 == 0)
	{
		ogien = rand() % 10 + 1;
	}
	if (jazda4 < ogien)
	{
		jazda4 += 0.2;
	}
	if (jazda4 > ogien)
	{
		jazda4 -= 0.2;
	}
}
void wyrenderuj_swiat(int numerstartowyswiata)
{
	//glUniformMatrix4fv(sp->u("Mt"), 1, false, glm::value_ptr(Mt));
	//glUniform4f(sp->u("lp"), 0, 0, -6, 1);

	//animacja_ogniska(klatkaz);
	//glUniform4f(sp->u("lp"), 18, 9, -172, 0);//zdrodlo swiatla 1 kominek
	
	//glUniform4f(sp->u("lp2"), jazda, jazda2,jazda3, 0);//zdrodlo swiatla 2
	//glUniform4f(sp->u("wspolczynnikswiatla"), jazda4, jazda4, jazda4, jazda4);//zdrodlo swiatla 1

	
	for (int x = numerstartowyswiata; x < numerswiata; x++)
	{
		tex = wektortekstur[x];
		tex2 = wektortekstur2[x];
		//glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		rysujobiekt(x);
	}
	if (wlacz_koordynaty % 2 == 0)
	{
		wyswietlpolozenie();
	}
	
}
//TODO
int getNumerKlatki(int actualFrameNumber)
{
	for(vector<Obiekty >::iterator obiekt = WektorObiektowSwiata.begin(); obiekt != WektorObiektowSwiata.end(); ++obiekt)
	{
		if (obiekt->startAnimacji <= actualFrameNumber && obiekt->koniecAnimacji >= actualFrameNumber) {
			int nextFrameNum = actualFrameNumber + 1;
			if (nextFrameNum >= obiekt->koniecAnimacji) {
				return obiekt->startAnimacji;
			}
			else {
				return nextFrameNum;
			}
		}
	}
}
int przypiszKlatke(string nazwaObiektu)
{
	for (vector<Obiekty>::iterator obiekt = WektorObiektowSwiata.begin(); obiekt != WektorObiektowSwiata.end(); ++obiekt)
	{
		if (obiekt->nazwaObiektu == nazwaObiektu) {
			return obiekt->startAnimacji;
		}
	}
}
void drawScene(GLFWwindow* window, float angle, int numerobiektu) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie bufora kolorów i bufora głębokości
	glEnable(GL_DEPTH_TEST);
	glm::mat4 P = glm::perspective(glm::radians(fov), WindowX / WindowY, 0.1f, 100.0f);  //Wyliczenie macierzy rzutowania
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //Wyliczenie macierzy widoku

	sp->use();//Aktywacja programu cieniującego

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	wyrenderuj_swiat(0);
	//int numerswiata = 1;//numer po ktorym wiadomo od ktorego momentu mozna renderowac poszczegolne obiekty
	int x = numerswiata; //Czy to potrzebne???

	for (vector<Hero>::iterator hero = heros.begin(); hero != heros.end(); ++hero)
	{
		Mhero = hero->getPosition(MacierzStolu);
		for (vector<int>::iterator animNumber = hero->animationFrames.begin(); animNumber != hero->animationFrames.end(); ++animNumber)
		{
			tex = wektortekstur[*animNumber];
			tex2 = wektortekstur2[*animNumber];
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mhero));
			rysujobiekt(*animNumber);
			*animNumber = getNumerKlatki(*animNumber);
		}
	}
	for (vector<Tower>::iterator tower = towers.begin(); tower != towers.end(); ++tower)
	{
		tex = wektortekstur[x];
		tex2 = wektortekstur2[x];
		Mtower = tower->getPosition();
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mtower));
		rysujobiekt(x);
		
		tex = wektortekstur[x];
		tex2 = wektortekstur2[x];
		
		//Nie najlepsze rozwiazanie ale nie zakladam zbyt duzej ilosci Herosow na planszy
		//ZApisac objekt ztargetowanego herosa, aby nie przeszukiwac wszytskich kiedy ten sam jest 
		//nadal w dangerZone EZ
		bool hasBarrel = false;
		for (vector<Hero>::iterator hero = heros.begin(); hero != heros.end(); ++hero) {
			if (tower->isHeroInDangerZone(hero->getMainHeroMatrix())) {
				Mbarrel = tower->getBarrelPosition(hero->getVec3Position());
				if (tower->canTakeShoot()) {
					//STRZELAŃSKO
					bullets.push_back(Bullet(Mbarrel, tower->getNameEnum(), *hero));
				}
				hasBarrel = true;
				break;
			}
		}
		if (!hasBarrel) {
			Mbarrel = tower->getBarrelPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		if (!(tower == towers.end() - 1 && isCreatingTower)) {
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mbarrel));
			rysujobiekt(x);
		}
	}
	for (vector<Bullet>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
		Mbullet = bullet->goForIt();
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mbullet));
		rysujobiekt(x);
	}
	removeDeadBodies();

	klatkaz++;

	glfwSwapBuffers(window);
}
//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYstart
int removeDeadBodies() {
	for (vector<Hero>::iterator hero = heros.begin(); hero != heros.end(); ++hero) {
		if (hero->isDead()) {
			if (hero->isEndGame()) {
				youLoseEndGame();
				return 0;
			}
			GlobalProperties::coins += hero->getvalue();
			Logger::info("Wallet", "Masz: " + to_string(GlobalProperties::coins));
			heros.erase(hero);
			break;
		}
	}
	for (vector<Bullet>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
		if (bullet->isDead()) {
			bullets.erase(bullet);
			break;
		}
	}
	return 0;
}
//ZMIANYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYend
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}
void przybliz()
{
	cameraPos += cameraSpeed*10 * cameraFront;
}
void oddal()
{
	cameraPos -= cameraSpeed*10 * cameraFront;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		przybliz();
		//jazda--;
	}
	else
	{
		oddal();
		//jazda++;
	}
	
	
		
		
}
void wektorowanie()
{	
	//wierzcholki
	for (int i = 0; i < vertices.size(); i++)
	{
		licznik++;
		wektor_indexed_vertices.push_back(vertices[i].x);
		wektor_indexed_vertices.push_back(vertices[i].y);
		wektor_indexed_vertices.push_back(vertices[i].z);
		wektor_indexed_vertices.push_back(1.0);
		tablicalicznik1++;
		tablicalicznik1++;
		tablicalicznik1++;
		tablicalicznik1++;

	}
	TabliceWektorowX.push_back(wektor_indexed_vertices);
	cout << "PRZEPISANIE NA WIERZCHOLKI\n";
	//Uv
	int licznik2 = 0;
	vector<float>wektor_uvs;
	for (int i = 0; i < uvs.size(); i++)
	{
		licznik2++;
		licznik2++;
		wektor_uvs.push_back(uvs[i].x);
		wektor_uvs.push_back(uvs[i].y);


	}
	TabliceUvX.push_back(wektor_uvs);
	cout << "PRZEPISANIE NA UV\n";
	//normal
	int licznik3 = 0;
	vector<float>wektor_normals;
	for (int i = 0; i < normals.size(); i++)
	{
		licznik3++;
		wektor_normals.push_back(normals[i].x);
		wektor_normals.push_back(normals[i].y);
		wektor_normals.push_back(normals[i].z);
		wektor_normals.push_back(0.0);



	}
	TabliceNormalX.push_back(wektor_normals);
	cout << "PRZEPISANIE NA NORMALE\n";
	//kolory
	vector<float>kolory;
	for (int a = 0; a < wektor_indexed_vertices.size(); a+=4)
	{
		kolory.push_back(1.0);

		kolory.push_back(0.0);
		kolory.push_back(0.0);
		kolory.push_back(1.0);
		
	}
	TabliceKolorowX.push_back(kolory);
	cout << "PRZEPISANIE NA KOLORY\n";

	//czyszczenie

	Tablicelicznik1.push_back(vertices.size());
	tablicalicznik1 = 0;

	Tablicelicznik.push_back(licznik);
	licznik = 0;


	vertices.clear();
	uvs.clear();
	normals.clear();

	//vertexbuffer.clear();
	//uvbuffer.clear();
	//normalbuffer.clear();
	//elementbuffer.clear();

	indices.clear();
	indexed_vertices.clear();
	indexed_uvs.clear();
	indexed_normals.clear();

	wektor_indexed_vertices.clear();
	wektor_uvs.clear();
	wektor_normals.clear();
	kolory.clear();
	cout << "UKONCZONO\n";
}
void przygotuj_obiektVer2(const char* path, int numerobiektu)
{

	bool res = loadOBJ(path, vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
	
	wektorowanie();
	


}
void wczytaj_obiekty() {
	przygotuj_obiektVer2("model//BezierCircle003_Mesh001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//BezierCircle003_Mesh001.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//BezierCircle003_Mesh001.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//BezierCircle005_Mesh004.obj", numerobiektu);
	doodczytu = readTexture("tekstury//BezierCircle005_Mesh004.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//BezierCircle005_Mesh004.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle000.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle000.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle000.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle005.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle005.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle005.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle006_Circle010.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle006_Circle010.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle006_Circle010.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle008_Circle003.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle008_Circle003.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle008_Circle003.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle010_Circle004.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle010_Circle004.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle010_Circle004.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle014_Circle016.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle014_Circle016.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle014_Circle016.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle015.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle015.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle015.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Circle001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Circle001.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Circle001.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Cube003_Cube023.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Cube003_Cube023.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Cube003_Cube023.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Cube008_Cube009.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Cube008_Cube009.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Cube008_Cube009.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Cube_1.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Cube_1.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Cube_1.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	/////////////
	przygotuj_obiektVer2("model//Cube_Cube002.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Stol.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Stol.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Stol//Pien.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Pien.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Pien.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Stol//KoronaDrzewa.obj", numerobiektu);
	doodczytu = readTexture("tekstury//KoronaDrzewa.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//KoronaDrzewa.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Stol//Nasypy.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Trawa1.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//TrawaSpec1.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Stol//RamaStolu.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Deski.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Deski.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	przygotuj_obiektVer2("model//Stol//Mlyn.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Mlyn.png");//stol
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Mlyn.png");//stol
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	//przygotuj_obiektVer2("model//Stol//Wyspy.obj", numerobiektu);
	//doodczytu = readTexture("tekstury//Wyspa.png");//stol
	//wektortekstur.push_back(doodczytu);
	//numerswiata++;
	/////////////
	przygotuj_obiektVer2("model//Cube_Cube003.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Cube_Cube003.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Cube_Cube003.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Cylinder_Cylinder001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Cylinder_Cylinder001.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Cylinder_Cylinder001.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//HalfSmallWall.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane002.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane003_Plane017.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane004.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane004.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane004.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane006_Plane003.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane006_Plane003.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane006_Plane003.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane007_Plane038.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane007_Plane038.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane007_Plane038.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane008_Plane006.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane008_Plane006.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane008_Plane006.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane011_Plane012.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane011_Plane012.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane011_Plane012.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane013_Plane009.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane013_Plane009.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane013_Plane009.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane014_Plane011.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane014_Plane011.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane014_Plane011.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane015_Plane018.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane015_Plane018.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane015_Plane018.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane016.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane016.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane016.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane017_Plane019.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane017_Plane019.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane017_Plane019.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane018_Plane020.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane018_Plane020.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane018_Plane020.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane019_Plane021.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane019_Plane021.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane019_Plane021.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane020_Plane022.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane020_Plane022.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane020_Plane022.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane021_Plane010.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane021_Plane010.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane021_Plane010.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane022_Plane008.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane024_Plane030.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane026_Plane023.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane026_Plane023.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane024_Plane030.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane028_Plane026.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");//szafa
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");//szafa
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane029_Plane031.obj", numerobiektu);
	doodczytu = readTexture("tekstury//bricks2.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//bricks2.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane031_Plane028.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane020_Plane022.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane020_Plane022.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane032_Plane029.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Plane042_Plane048.obj", numerobiektu);
	doodczytu = readTexture("tekstury//szary.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//szary.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//SmallDoor.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	
	przygotuj_obiektVer2("model//Template001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	
	przygotuj_obiektVer2("model//Template003.obj", numerobiektu);
	doodczytu = readTexture("tekstury//cegly.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//cegly.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	
	przygotuj_obiektVer2("model//Template004.obj", numerobiektu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	
	przygotuj_obiektVer2("model//Template005.obj", numerobiektu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Template006_Template007.obj", numerobiektu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//ivory.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Wallpaper.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Wallpaper001.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;

	przygotuj_obiektVer2("model//Domek.obj", numerobiektu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//HalfSmallWall.png");
	wektortekstur2.push_back(doodczytu);
	numerswiata++;
	




	//obiekty z poza swiata

	wczytajObiektySwiata(numerswiata);



	//tex = readTexture("metal.png");
	//tex2 = readTexture("metal_spec.png");


	//numerobiektu++;
	//przygotuj_obiektVer2("model//kostka.obj", numerobiektu);
	//doodczytu = readTexture("tekstury//bricks.png");
	//wektortekstur.push_back(doodczytu);
	//
	//numerobiektu++;
	//przygotuj_obiektVer2("model//kostka.obj", numerobiektu);
	//doodczytu = readTexture("tekstury//test.png");
	//wektortekstur.push_back(doodczytu);
	//
	//numerobiektu++;
	//przygotuj_obiektVer2("model//kostka.obj", numerobiektu);
	//doodczytu = readTexture("tekstury//bricks.png");
	//wektortekstur.push_back(doodczytu);

}
vector<string>czytaj_sciezke(vector<string>wektor, string sciezka)
{
	string path = sciezka;
	string plik;
	string plikOBJ;
	for (const auto & entry : experimental::filesystem::directory_iterator(path))
	{
		//if(entry.path()=="*.obj")
		plik = entry.path().string();
		int znaleziono = 0;
		znaleziono = plik.find(".obj");
		if (znaleziono > 0)
		{
			plikOBJ = plik;
			wektor.push_back(plikOBJ);
		}
	}
	return wektor;
}
void wczytajAnimacje(string tekstura1, string tekstura2, string nazwaAnimacji)
{
	vector<Obiekty>animacja;
	const char* sciezkaTekstury1 = tekstura1.c_str();
	const char* sciezkaTekstury2 = tekstura2.c_str();
	for (int x = 0; x < sciezkiDoObiektow.size(); x++)
	{
		//sciezkiDoObiektow[x]
		numerobiektu++;

		const char * c = sciezkiDoObiektow[x].c_str();

		przygotuj_obiektVer2(c, numerobiektu);
		cout << x << endl;

		doodczytu = readTexture(sciezkaTekstury1);
		wektortekstur.push_back(doodczytu);
		doodczytu = readTexture(sciezkaTekstury2);
		wektortekstur2.push_back(doodczytu);

		Obiekt.nazwaObiektu = nazwaAnimacji;

		if (x == 0)
		{
			Obiekt.startAnimacji = numerobiektu;
			Obiekt.aktualnyNumerKlatki = numerobiektu;
		}
		Obiekt.koniecAnimacji = numerobiektu;

	}


	WektorObiektowSwiata.push_back(Obiekt);
	sciezkiDoObiektow.clear();
}
void wczytajObiektySwiata(int ostatniNumerSwiata)
{



	numerobiektu = ostatniNumerSwiata;

	przygotuj_obiektVer2("model//kostka.obj", numerobiektu);
	doodczytu = readTexture("tekstury//test.png");
	wektortekstur.push_back(doodczytu);
	doodczytu = readTexture("tekstury//test.png");
	wektortekstur2.push_back(doodczytu);


	sciezkiDoObiektow = czytaj_sciezke(sciezkiDoObiektow, "model//AnimacjaRobot");
	wczytajAnimacje("tekstury//metal.png", "tekstury//metal_spec.png", "kostka");
	sciezkiDoObiektow = czytaj_sciezke(sciezkiDoObiektow, "model//RobotRece");
	wczytajAnimacje("tekstury//metal.png", "tekstury//metal_spec.png", "RobotRece");


	//animacja.push_back(Obiekt);
	//petla animacji

	//WektorObiektowSwiata.push_back(animacja);
	//animacja.clear();



	/*
	numerobiektu++;
	przygotuj_obiektVer2("model//robot.obj", numerobiektu);
	doodczytu = readTexture("tekstury//Plane022_Plane008.png");
	wektortekstur.push_back(doodczytu);
	*/
}