#define OLC_PGE_APPLICATION
#include <iostream>
#include <string.h>
#include <time.h>
#include "olcPixelGameEngine.h"

class Dropping {
public:
	int x;
	float y;
	int speed;
	int w;
	int h;
};

bool paused = false;
bool exitf = false;
int px = 400;
int py = 300;
int score = 0;
int dn = 0;
int dif = 100;
Dropping drop[100];
float fTargetFrameTime = 1.0f / 500.0f; // Virtual FPS of 100fps
float fAccumulatedTime = 0.0f;



void death() {
	px = 400;
	py = 300;
	score = 0;
	dn = 0;
	dif = 100;
	for (int j = 0; j < 100; j++) {
		drop[j].y = 800;
	}
}



class BoxD : public olc::PixelGameEngine {

public:
	BoxD() {
		sAppName = "Box Dodge";
	}

public:
	bool OnUserCreate() override{
		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override{
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;

			if (dn == 99) {
				dn = 0;
			}
			if (score % 1000 == 0) {
				dif -= 5;
			}
			if (dif < 10) {
				dif = 10;
			}
			if (!paused) { score++; }
			std::string s = std::to_string(score);
			Clear(olc::BLACK);
			if (GetKey(olc::Key::W).bHeld && paused == false) {
				py-=(500*fElapsedTime);
			}
			if (GetKey(olc::Key::S).bHeld && paused == false) {
				py+=(500*fElapsedTime);
			}
			if (GetKey(olc::Key::A).bHeld && paused == false) {
				px-=(500*fElapsedTime);
			}
			if (GetKey(olc::Key::D).bHeld && paused == false) {
				px+=(500*fElapsedTime);
			}
			if (score % dif == 0) {
				drop[dn].x = rand() % 800;
				drop[dn].y = 0;
				drop[dn].w = rand() % 30 + 10;
				drop[dn].h = rand() % 30 + 10;
				drop[dn].speed = rand() % 4 + 1;
				dn++;
			}
			if (px < 0) {
				px = 20;;
			}
			else if (px > 800) {
				px = 780;
			}
			if (py < 0) {
				py = 20;
			}
			else if (py > 600) {
				py = 580;
			}
			for (int i = 0; i < 99; i++) {
				if (drop[i].x < px + 20 && drop[i].x + drop[i].w > px && drop[i].y < py + 20 && drop[i].y + drop[i].h > py) {
					paused = true;
					break;
				}
				if (drop[i].y < 601 && paused == false) {
					drop[i].y += (500 * fElapsedTime);
					DrawRect(drop[i].x, drop[i].y, drop[i].w, drop[i].h);
				}
				if (drop[i].y == 601) {
					drop[i].y = 800;
				}

			}
			DrawRect(px, py, 20, 20, olc::BLUE);
			DrawString(700, 20, "Score: " + s);
			if (paused) {
				DrawString(400, 200, "You died. Your score is: " + s, olc::RED);
				DrawString(400, 160, "Press space to start again", olc::RED);
				if (GetKey(olc::Key::SPACE).bHeld) {
					paused = false;
					death();
				}
			}
			return true;
		}
		else {
			return true;
		}
	}

};



int main() {
	srand(time(NULL));
	BoxD box;
	if (box.Construct(800, 600, 1, 1, false, false)) {
		box.Start();
	}
	return 0;
}