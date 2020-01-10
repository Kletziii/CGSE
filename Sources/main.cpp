#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <algorithm>
#include <stdlib.h> // for exit (must be above GL)
#include <cstdlib>

#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Wall.h"
#include "../Headers/Obstacle.h"
#include "../Headers/Coin.h"
#include "../Headers/Hud.h"
#include "../Headers/Camera.h"
#include "../Headers/Light.h"
#include "../Headers/FireBall.h"
#include "../Headers/Vector3d.h"
#include "../Headers/TextureLoader.h"
#include "../Headers/CollisionBox.h"
#include "../Headers/Transparent.h"

#include "../Assimp/assimp-3.1.1/include/assimp/ai_assert.h"

using namespace std;

/* some math.h files don't define pi... */
#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)

#ifdef __STRICT_ANSI__
#define sinf(x) ((float)sin((x)))
#define cosf(x) ((float)cos((x)))
#define atan2f(x, y) ((float)atan2((x), (y)))
#endif 

vector<Wall> walls;
map<int, Obstacle> obstacles;
vector<FireBall> fireBalls;
vector<Coin> coins;
vector<Transparent> transparent;
Camera camera = Camera(0.05f);
Hud hud = Hud();
TextureLoader textureLoader = TextureLoader();

int window;
int keyArr[127]; // store key presses

// jumping variables
bool jumpingUp = false, jumpingDown = false;
float jumpIncr = 0, jumpSpeed = 2.0f, jumpHeight = 1.0f;

int moving = 0;     /* flag that is true while mouse moves */
int begin_x = 0;    /* x value of mouse movement */
int begin_y = 0;    /* y value of mouse movement */

// increment variables
float inc = 0, inc2 = 0;

//----------------------------------------------
//Assimp::Importer importer;
//const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll need
//
//aiMesh *mesh = scene->mMeshes[0]; //assuming you only want the first mesh
//float *vertexArray;
//float *normalArray;
//float *uvArray;
//
//int numVerts;
//
//numVerts = mesh->mNumFaces * 3;
//
//vertexArray = new float[mesh->mNumFaces * 3 * 3];
//normalArray = new float[mesh->mNumFaces * 3 * 3];
//uvArray = new float[mesh->mNumFaces * 3 * 2];
//
//for (unsigned int i = 0;i < mesh->mNumFaces;i++)
//{
//	const aiFace& face = mesh->mFaces[i];
//
//	for (int j = 0;j < 3;j++)
//	{
//		aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
//		memcpy(uvArray, &uv, sizeof(float) * 2);
//		uvArray += 2;
//
//		aiVector3D normal = mesh->mNormals[face.mIndices[j]];
//		memcpy(normalArray, &normal, sizeof(float) * 3);
//		normalArray += 3;
//
//		aiVector3D pos = mesh->mVertices[face.mIndices[j]];
//		memcpy(vertexArray, &pos, sizeof(float) * 3);
//		vertexArray += 3;
//	}
//}
//
//uvArray -= mesh->mNumFaces * 3 * 2;
//normalArray -= mesh->mNumFaces * 3 * 3;
//vertexArray -= mesh->mNumFaces * 3 * 3;

void resize(int width, int height) {
	// prevent division by zero
	if (height == 0) { height = 1; }

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void drawFloor(float posBeginX, float posBeginZ, float posEndX, float posEndZ, float factor) {
	glBindTexture(GL_TEXTURE_2D, textureLoader.get("flagstone"));

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(posBeginX, 0.0f, posBeginZ);

	glTexCoord2f(factor * abs(posBeginX - posEndX), 0.0f);
	glVertex3f(posEndX, 0.0f, posBeginZ);

	glTexCoord2f(factor * abs(posBeginX - posEndX), factor * abs(posBeginZ - posEndZ));
	glVertex3f(posEndX, 0.0f, posEndZ);

	glTexCoord2f(0.0f, factor * abs(posBeginZ - posEndZ));
	glVertex3f(posBeginX, 0.0f, posEndZ);

	glEnd();
	glPopMatrix();
}

void keyPressed(unsigned char key, int x, int y) {
	if (key < 127) { // no extended characters
		keyArr[key] = 1;
	}
}

void keyUp(unsigned char key, int x, int y) {
	if (key < 127) { // no extended characters
		keyArr[key] = 0;
	}
}

void spawnFireBall() {
	if (hud.isFireBallReady()) {
		fireBalls.push_back(FireBall(camera.getPosition(), camera.getLookAt(), 0.2f, textureLoader.get("fire")));
		hud.removeFireBallPoints();
	}
}

void handleFireBalls() {
	// loop through fire balls
	for (int i = 0; i < fireBalls.size(); i++) {
		if (fireBalls[i].getDistanceTraveled() >= 20 || fireBalls[i].isHit()) {
			// remove fire ball
			fireBalls.erase(fireBalls.begin() + i);
		}
		else {
			fireBalls[i].updatePosition();
		}
	}

	// reloading
	if (!hud.isFireBallReady()) {
		hud.addFireBallPoints();
	}
}

void handleKeys() {
	vector<char> debugKeys;

	// go through all possible key presses
	for (int key = 0; key < 127; key++) {

		// check if key is pressed
		if (keyArr[key] == 1) {
			debugKeys.push_back(key);

			switch (key) {
			case ' ':
				if (!jumpingUp && !jumpingDown) { // on the ground
					jumpingUp = true;
				}
				break;
			case 'a':
				camera.moveLeft();
				for (int i = 0; i < walls.size(); i++) { // check wall collision
					if (CollisionBox::CheckCollision(walls[i].getCollisionBox(), camera.getCollisionBox())) {
						camera.moveRight();
						break;
					}
				}
				for (auto& pair : obstacles) { // check obstacle collision
					if (!pair.second.isHarmful() && CollisionBox::CheckCollision(pair.second.getCollisionBox(), camera.getCollisionBox())) {
						camera.moveRight();
						break;
					}
				}
				break;
			case 'd':
				camera.moveRight();
				for (int i = 0; i < walls.size(); i++) { // check wall collision
					if (CollisionBox::CheckCollision(walls[i].getCollisionBox(), camera.getCollisionBox())) {
						camera.moveLeft();
						break;
					}
				}
				for (auto& pair : obstacles) { // check obstacle collision
					if (!pair.second.isHarmful() && CollisionBox::CheckCollision(pair.second.getCollisionBox(), camera.getCollisionBox())) {
						camera.moveLeft();
						break;
					}
				}
				break;
			case 'w':
				camera.moveForward();
				for (int i = 0; i < walls.size(); i++) { // check wall collision
					if (CollisionBox::CheckCollision(walls[i].getCollisionBox(), camera.getCollisionBox())) {
						camera.moveBackward();
						break;
					}
				}
				for (auto& pair : obstacles) { // check obstacle collision
					if (!pair.second.isHarmful() && CollisionBox::CheckCollision(pair.second.getCollisionBox(), camera.getCollisionBox())) {
						camera.moveBackward();
						break;
					}
				}
				break;
			case 's':
				camera.moveBackward();
				for (int i = 0; i < walls.size(); i++) { // check wall collision
					if (CollisionBox::CheckCollision(walls[i].getCollisionBox(), camera.getCollisionBox())) {
						camera.moveForward();
						break;
					}
				}
				for (auto& pair : obstacles) { // check obstacle collision
					if (!pair.second.isHarmful() && CollisionBox::CheckCollision(pair.second.getCollisionBox(), camera.getCollisionBox())) {
						camera.moveForward();
						break;
					}
				}
				break;
			case 27: // ESC
				glutDestroyWindow(window);
				exit(0);
				break;
			case 'f':
				spawnFireBall();
				break;
			default:
				break;
			}
		}
	}

	// debug camera position
	if (find(debugKeys.begin(), debugKeys.end(), 'w') != debugKeys.end() ||
		find(debugKeys.begin(), debugKeys.end(), 'a') != debugKeys.end() ||
		find(debugKeys.begin(), debugKeys.end(), 's') != debugKeys.end() ||
		find(debugKeys.begin(), debugKeys.end(), 'd') != debugKeys.end()) {
		camera.printDebug();
	}

	// debug keys pressed
	if (debugKeys.size() > 0) {
		printf("[KEYS]   ");

		for (int i = 0; i < debugKeys.size(); i++) {
			printf("'%c' ", debugKeys[i]);
		}

		printf("\n");
	}
}

void handleJumping() {
	if (jumpingUp) {
		jumpIncr += jumpSpeed;
		camera.moveVertical(jumpHeight * (sinf(RAD(jumpIncr)) - sinf(RAD(jumpIncr - jumpSpeed))));

		// sinus peak
		if (jumpIncr >= 90) {
			jumpingDown = true;
			jumpingUp = false;
		}
	}
	else if (jumpingDown) {
		jumpIncr -= jumpSpeed;
		camera.moveVertical(-(jumpHeight * (sinf(RAD(jumpIncr + jumpSpeed)) - sinf(RAD(jumpIncr)))));

		if (jumpIncr <= 0 || camera.getPosition().getY() <= 1) {
			camera.teleportTo(Vector3d(camera.getPosition().getX(), 1, camera.getPosition().getZ()), false);
			jumpingDown = false;
			jumpIncr = 0;
		}
	}
}

void doCollisionsFireBallWalls() {
	for (int i = 0; i < walls.size(); i++) {
		for (int j = 0; j < fireBalls.size(); j++) {
			if (CollisionBox::CheckCollision(walls[i].getCollisionBox(), fireBalls[j].getCollisionBox())) {
				cout << "COLLISION DETECTED: FireBall <-> Wall" << endl;

				fireBalls[j].setCollision();
			}
		}
	}
}

void doCollisionsFireBallObstacles() {
	for (auto& pair : obstacles) {
		for (int j = 0; j < fireBalls.size(); j++) {
			if (CollisionBox::CheckCollision(pair.second.getCollisionBox(), fireBalls[j].getCollisionBox())) {
				cout << "COLLISION DETECTED: Fireball <-> Obstacle " << pair.first << endl;

				pair.second.setCollision();
				fireBalls[j].setCollision();
			}
		}
	}
}

void doCollisionsCameraObstacles() {
	for (auto& pair : obstacles) {
		if (pair.second.isHarmful() && CollisionBox::CheckCollision(pair.second.getCollisionBox(), camera.getCollisionBox())) {
			cout << "COLLISION DETECTED: Camera <-> Obstacle " << pair.first << endl;

			// reset to spawn
			camera.teleportTo(Vector3d(0, 1, 0), true);
			// remove life
			hud.removeLife();
		}
	}
}

void doCollisionsCameraCoins() {
	for (int i = 0; i < coins.size(); i++) {
		if (CollisionBox::CheckCollision(coins[i].getCollisionBox(), camera.getCollisionBox())) {
			cout << "COLLISION DETECTED: Camera <-> Coin" << endl;

			// remove coin
			coins.erase(coins.begin() + i);
			// add points
			hud.addPoints(5);
		}
	}
}

void handleCoins() {
	for (int i = 0; i < coins.size(); i++) {
		coins[i].updateRotation();
	}
}

void handleObstacles() {
	// remove obstacles (needs iterator)
	// TODO Transparents here
	Vector3d camPos = camera.getPosition();

	for (auto it = obstacles.cbegin(); it != obstacles.cend();) {
		if (it->second.isDestructible() && it->second.isHit()) {
			it = obstacles.erase(it);
		}
		else {
			++it;
		}	
	}

	// draw obstacles
	for (auto& pair : obstacles) {



		Vector3d dif = camPos - pair.second.getPosition();
		double length = dif.length();
		if (length < 5) {
			pair.second.drawObstacle2();
		}
		else {
			pair.second.drawObstacle();
		}
	}
}

void handleTransparents() {
	struct {
		bool bigger(const Transparent& a, const Transparent& b) const {
			return abs((a.getPosition() - camera.getPosition()).length()) > abs((b.getPosition() - camera.getPosition()).length());
		}
	} sortFunc;

	std::sort(transparent.begin(), transparent.end(), sortFunc);

	// draw triangles
	for (int i = 0; i < transparent.size(); i++) {
		transparent[0].drawTransparent;
	}
}

void handleWalls() {
	for (int i = 0; i < walls.size(); i++) {
		walls[i].draw();
	}
}

void handleRunning() {
	if (camera.isRunning()) {
		hud.removeRunningPoints();

		if (hud.getRunningPoints() == 0) {
			camera.setRunning(false);
		}
	}
	else if (hud.getRunningPoints() < 100) {
		hud.addRunningPoints();
	}
}

void drawWorld() {
	// floor
	drawFloor(-10.0f, 2.0f, 28.0f, -40.0f, 2);

	// obstacle
	inc += 0.025f;
	inc2 += 0.04f;

	// slider at the beginning
	for (int i = 0; i < 2; i++) {
		if (obstacles.find(i) != obstacles.end()) {
			Vector3d currentPosition = obstacles[i].getPosition(); // current position of that obstacle

			obstacles[i].setPosition(Vector3d( // set new position for that obstacle   
				currentPosition.getX() + sinf(inc2) / 2.8, // add increment to current position on x axis
				currentPosition.getY(),
				currentPosition.getZ()));
		}
	}

	// trap1
	if (obstacles.find(2) != obstacles.end()) {
		Vector3d currentPosition = obstacles[2].getPosition(); // current position of that obstacle

		obstacles[2].setPosition(Vector3d( // set new position for that obstacle   
			currentPosition.getX(),
			currentPosition.getY(),
			currentPosition.getZ() - sinf(inc) / 16)); // add increment to current position on z axis
	}

	// trap2
	if (obstacles.find(3) != obstacles.end()) {
		Vector3d currentPosition = obstacles[3].getPosition(); // current position of that obstacle

		obstacles[3].setPosition(Vector3d( // set new position for that obstacle   
			currentPosition.getX(),
			currentPosition.getY(),
			currentPosition.getZ() + sinf(inc) / 16)); // add increment to current position on z axis
	}

	// wall1
	for (int i = 4; i < 9; i++) {
		if (obstacles.find(i) != obstacles.end()) {
			Vector3d currentPosition = obstacles[i].getPosition(); // current position of that obstacle

			obstacles[i].setPosition(Vector3d( // set new position for that obstacle   
				currentPosition.getX(),
				currentPosition.getY() + sinf(inc) / 35, // add increment to current position on y axis
				currentPosition.getZ()));
		}
	}

	// obstacle from above
	for (int i = 9; i < 18; i++) {
		if (obstacles.find(i) != obstacles.end()) {
			Vector3d currentPosition = obstacles[i].getPosition(); // current position of that obstacle

			obstacles[i].setPosition(Vector3d( // set new position for that obstacle   
				currentPosition.getX(),
				currentPosition.getY() + sinf(inc2) / 35, // add increment to current position on y axis
				currentPosition.getZ()));
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// camera
	gluLookAt(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ(), // camera position
		camera.getLookAt().getX(), camera.getLookAt().getY(), camera.getLookAt().getZ(), // look at point
		0.0f, 1.0f, 0.0f);

	// lighting
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_position[] = { 0, 10, 0, 1.0 };
	Light sunLight(GL_LIGHT0, light_position, light_ambient, light_diffuse, light_specular);
	sunLight.ActivateLight();

	// handlers
	handleJumping();
	handleKeys();
	handleFireBalls();
	handleCoins();
	handleObstacles();
	handleWalls();
	handleRunning();

	// TODO
	GLfloat mat_white[] = { 1.0, 1.0, 1.0, 0.5 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	glBegin(GL_QUADS);
	//glNormal3f(0.0, 0.0, 1.0);
	glColor4f(.23, .78, .32, 0.1);
	// immer gleich, höhe, breite
	glVertex3f(1.0, 1.0, 1.0);
	glColor4f(.23, .78, .32, 0.1);
	glVertex3f(1.0, 1, 0.0);
	glColor4f(.23, .78, .32, 0.1);
	glVertex3f(1.0, 0.0, 0.0);
	glColor4f(1.0, .78, .32, 0.1);
	glVertex3f(1.0, 0.0, 1.0);
	glEnd();
	glFlush();

	GLfloat mat_white2[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white2);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	// world
	drawWorld();

	//draw transparency
	handleTransparents();

	// collision handling
	doCollisionsFireBallObstacles();
	doCollisionsCameraObstacles();
	doCollisionsCameraCoins();
	doCollisionsFireBallWalls();

	// hud
	hud.drawHUD();

	glutSwapBuffers();
}

void init(int width, int height) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// --- LIGHTING ---
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// set material parameter
	GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	// --- LIGHTING ---

	resize(width, height);

	// load textures
	textureLoader.initialize();

	// create coins
	coins.push_back(Coin(Vector3d(14.5, 0.5, -38.5), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(20.5, 0.5, -30.5), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(-9.5, 0.5, -29.5), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back"))); // hidden coin
	coins.push_back(Coin(Vector3d(0, 0.5, -7), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(-8.5, 1.5, -16), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(8.5, 1.5, -16), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(0, 1.5, -16), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(0, 1, -24), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));
	coins.push_back(Coin(Vector3d(25, 0.5, -34.5), textureLoader.get("coin_front"), textureLoader.get("coin_core"), textureLoader.get("coin_back")));

	// create obstacles
	obstacles[0] = Obstacle(Vector3d(-9, 1, -15), Vector3d(1, 1, 1), true, true, 1, textureLoader.get("crate"));
	obstacles[1] = Obstacle(Vector3d(-9, 1, -17), Vector3d(1, 1, 1), true, true, 1, textureLoader.get("crate"));

	obstacles[2] = Obstacle(Vector3d(14.5, 1, -30.5), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[3] = Obstacle(Vector3d(20.5, 1, -38.5), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));

	obstacles[4] = Obstacle(Vector3d(0, 1, -20), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[5] = Obstacle(Vector3d(0, 1, -22), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[6] = Obstacle(Vector3d(0, 1, -24), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[7] = Obstacle(Vector3d(0, 1, -26), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[8] = Obstacle(Vector3d(0, 1, -28), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));

	obstacles[9] = Obstacle(Vector3d(7, 1, -29), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[10] = Obstacle(Vector3d(9, 1, -29), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[11] = Obstacle(Vector3d(7, 1, -27), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[12] = Obstacle(Vector3d(9, 1, -27), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[13] = Obstacle(Vector3d(7, 1, -25), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[14] = Obstacle(Vector3d(9, 1, -25), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[15] = Obstacle(Vector3d(9, 1, -31), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));
	obstacles[16] = Obstacle(Vector3d(5, 1, -29), Vector3d(1, 1, 1), true, true, 1, textureLoader.get("crate"));
	obstacles[17] = Obstacle(Vector3d(5, 1, -27), Vector3d(1, 1, 1), true, true, 1, textureLoader.get("crate"));

	obstacles[18] = Obstacle(Vector3d(-9.5, 0.5, -29.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[19] = Obstacle(Vector3d(-9.5, 0.5, -28.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[20] = Obstacle(Vector3d(-8.5, 0.5, -28.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[21] = Obstacle(Vector3d(-7.5, 0.5, -28.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[22] = Obstacle(Vector3d(-7.5, 0.5, -27.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[23] = Obstacle(Vector3d(-7.5, 0.5, -26.5), Vector3d(0.5, 0.5, 0.5), true, false, 2, textureLoader.get("crate"));
	obstacles[24] = Obstacle(Vector3d(-9.5, 0.5, -27.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[25] = Obstacle(Vector3d(-9.5, 0.5, -26.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[26] = Obstacle(Vector3d(-8.5, 0.5, -27.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[27] = Obstacle(Vector3d(-8.5, 0.5, -26.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[28] = Obstacle(Vector3d(-8.5, 0.5, -29.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[29] = Obstacle(Vector3d(-7.5, 0.5, -29.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[30] = Obstacle(Vector3d(-6.5, 0.5, -29.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[31] = Obstacle(Vector3d(-6.5, 0.5, -28.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[32] = Obstacle(Vector3d(-6.5, 0.5, -27.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));
	obstacles[33] = Obstacle(Vector3d(-6.5, 0.5, -26.5), Vector3d(0.5, 0.5, 0.5), false, false, 2, textureLoader.get("metalcrate"));

	// lava lake
	obstacles[34] = Obstacle(Vector3d(0, 0.0001, -16), Vector3d(10, 0.0001, 2), false, true, 1, textureLoader.get("lava"));
	obstacles[35] = Obstacle(Vector3d(0, 0.0001, -24), Vector3d(2, 0.0001, 6), false, true, 1, textureLoader.get("lava"));

	// create walls
	//walls.push_back(Wall(-2, 2, -2, -10, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(2, 2, 2, -10, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(-2, 2, 2, 2, 3, false, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(-2, -10, -10, -10, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(2, -10, 10, -10, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(-10, -10, -10, -30, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(10, -10, 10, -33, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(-10, -30, 7, -30, 3, true, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(7, -36, 13, -36, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(10, -33, 13, -33, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(7, -30, 7, -36, 3, true, 1, textureLoader.get("bricks")));

	// trap1
	walls.push_back(Wall(13, -36, 13, -40, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(16, -36, 16, -40, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(13, -40, 16, -40, 3, true, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(13, -33, 13, -29, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(16, -33, 16, -29, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(13, -29, 16, -29, 3, false, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(16, -36, 19, -36, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(16, -33, 19, -33, 3, false, 1, textureLoader.get("bricks")));

	// trap2
	walls.push_back(Wall(19, -36, 19, -40, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(22, -36, 22, -40, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(19, -40, 22, -40, 3, true, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(19, -33, 19, -29, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(22, -33, 22, -29, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(19, -29, 22, -29, 3, false, 1, textureLoader.get("bricks")));

	walls.push_back(Wall(22, -36, 28, -36, 3, true, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(22, -33, 28, -33, 3, false, 1, textureLoader.get("bricks")));
	walls.push_back(Wall(28, -33, 28, -36, 3, false, 1, textureLoader.get("bricks")));

	obstacles[36] = Obstacle(Vector3d(0, 1, -6), Vector3d(1, 1, 1), false, true, 1, textureLoader.get("metalcrate"));

	transparent.push_back()
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(15, timer, 1);
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:    /* spin scene around */
		if (state == GLUT_DOWN) {
			moving = 1;
			begin_x = x;
			begin_y = y;
		}
		else if (state == GLUT_UP) {
			moving = 0;
		}
		break;
	case GLUT_RIGHT_BUTTON:    /* running */
		if (state == GLUT_DOWN) {
			camera.setRunning(true);
		}
		else if (state == GLUT_UP) {
			camera.setRunning(false);
		}
		break;
	default:
		break;
	}
}

void mouseMotion(int x, int y) {
	if (moving) { /* mouse button is pressed */
		camera.changeLook(x - begin_x, y - begin_y);
		camera.printDebug();

		// reset begin
		begin_x = x;
		begin_y = y;
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Dungeon Lurker");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	glutKeyboardUpFunc(&keyUp);
	init(640, 480);
	glutTimerFunc(15, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutFullScreen();
	glutMainLoop();

	return 0;
}
