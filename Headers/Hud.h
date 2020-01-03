#pragma once

class Hud {
public:
	Hud();
	~Hud();
	void drawHUD();
	void addPoints(int points);
	void removeLife();
	float getRunningPoints();
	void removeRunningPoints();
	void addRunningPoints();
	bool isFireBallReady();
	void addFireBallPoints();
	void removeFireBallPoints();

private:
	void drawGameOverlay();
	void drawGameOver();
	void drawGameWon();
	bool gameOver;
	bool gameWon;
	int points;
	int lives;
	float runningPoints; // 100 = 100%
	float fireBallPoints; // 100 = fireball ready
	void* font;
};