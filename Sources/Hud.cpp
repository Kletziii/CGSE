#include <stdlib.h> // for exit (must be above GL)
#include <iostream>
#include <string>
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  

#include "../Headers/Hud.h"

using namespace std;

Hud::Hud() {
	this->points = 0;
	this->lives = 3;
	this->runningPoints = 100;
	this->fireBallPoints = 100;
	this->gameOver = false;
	this->gameWon = false;
	this->font = GLUT_BITMAP_HELVETICA_18;
}

Hud::~Hud() {
}

void Hud::drawGameOver() {
	// draw text
	glColor3f(0.0f, 0.0f, 0.0f);
	string gameOverText = "Game Over";
	glRasterPos2i((GLUT_SCREEN_WIDTH / 2.0f) - (gameOverText.length() / 2.0f), GLUT_SCREEN_HEIGHT / 2.0f);
	for (char* c = &gameOverText[0]; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	// draw background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0, 0);
	glVertex2f(0, GLUT_SCREEN_HEIGHT);
	glVertex2f(GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
	glVertex2f(GLUT_SCREEN_WIDTH, 0);
	glEnd();
}

void Hud::drawGameWon() {
	// draw text
	glColor3f(0.0f, 0.0f, 0.0f);

	string gameWonText = "Game Won! Congratulation";
	glRasterPos2i((GLUT_SCREEN_WIDTH / 2.0f) - (gameWonText.length() / 2.0f), GLUT_SCREEN_HEIGHT / 2.0f);
	for (char* c = &gameWonText[0]; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	// draw background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0, 0);
	glVertex2f(0, GLUT_SCREEN_HEIGHT);
	glVertex2f(GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
	glVertex2f(GLUT_SCREEN_WIDTH, 0);
	glEnd();
}

void Hud::drawGameOverlay() {
	// draw points
	glColor3f(0.0f, 0.0f, 1.0f);
	glRasterPos2i(8, 14);
	string pointsText = "Punkte: " + to_string(points) + " / 45";
	for (char* c = &pointsText[0]; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	// draw points background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(5.0, 5.0);
	glVertex2f(38, 5.0);
	glVertex2f(38, 18);
	glVertex2f(5.0, 18);
	glEnd();

	// draw lives
	glColor3f(0.0f, 0.0f, 1.0f);
	glRasterPos2i(GLUT_SCREEN_WIDTH - 35, 14);
	string livesText = "Leben: " + to_string(lives);
	for (char* c = &livesText[0]; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	// draw lives background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(GLUT_SCREEN_WIDTH - 5.0, 5.0);
	glVertex2f(GLUT_SCREEN_WIDTH - 38, 5.0);
	glVertex2f(GLUT_SCREEN_WIDTH - 38, 18);
	glVertex2f(GLUT_SCREEN_WIDTH - 5.0, 18);
	glEnd();

	// draw running points
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(5, 23);
	glVertex2f(5 + (38 - 5)*(runningPoints / 100), 23);
	glVertex2f(5 + (38 - 5)*(runningPoints / 100), 33);
	glVertex2f(5, 33);
	glEnd();

	// draw running background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(5, 23);
	glVertex2f(38, 23);
	glVertex2f(38, 33);
	glVertex2f(5, 33);
	glEnd();

	// draw fireball points
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(5, 38);
	glVertex2f(5 + (38 - 5)*(fireBallPoints / 100), 38);
	glVertex2f(5 + (38 - 5)*(fireBallPoints / 100), 48);
	glVertex2f(5, 48);
	glEnd();

	// draw fireball background
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(5, 38);
	glVertex2f(38, 38);
	glVertex2f(38, 48);
	glVertex2f(5, 48);
	glEnd();
}

void Hud::drawHUD() {
	// render 2d
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);

	// draw hud
	if (this->gameOver) {
		drawGameOver();
	}
	else if (this->gameWon) {
		drawGameWon();
	}
	else {
		drawGameOverlay();
	}

	// render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}

void Hud::addPoints(int points) {
	this->points += points;

	if (this->points == 45) {	// 45 are the highscore points
		this->gameWon = true;
	}
}

void Hud::removeLife() {
	this->lives--;

	if (this->lives <= 0) {
		this->gameOver = true;
	}
}

float Hud::getRunningPoints() {
	return this->runningPoints;
}

void Hud::removeRunningPoints() {
	this->runningPoints -= 1;

	if (this->runningPoints < 0) {
		this->runningPoints = 0;
	}
}

void Hud::addRunningPoints() {
	this->runningPoints += 0.2;

	if (this->runningPoints > 100) {
		this->runningPoints = 100;
	}
}

bool Hud::isFireBallReady() {
	return this->fireBallPoints >= 100;
}

void Hud::removeFireBallPoints() {
	this->fireBallPoints = 0;
}

void Hud::addFireBallPoints() {
	this->fireBallPoints += 2;

	if (this->fireBallPoints > 100) {
		this->fireBallPoints = 100;
	}
}