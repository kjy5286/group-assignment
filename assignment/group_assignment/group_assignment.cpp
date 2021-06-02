#include <bangtal.h>
#include <stdio.h>
#include <math.h>

SceneID init_scene, mining_scene;
ObjectID mining_point, home;
TimerID mining_timer;

int level;

void game_option() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
}

void mining_timer_set() {
	mining_timer = createTimer(25.0f);
	setTimer(mining_timer, 25.0f);
}



void timerCallback(TimerID timer) {

}

void mining_start() {

}

int mining(int level) {
	int money;

	switch (level)
	{
	case 0:
		money += 1;
	case 1:
		money += 2;
	case 2:
		money += 4;

	}
	for (int i = 0; i < 10; i++) {
		if (level == i) {
			money += pow(2, i);
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == mining_point) {
		mining(level);
	}
	else if (object == home) {
		enterScene(init_scene);
	}
}

int main() {
	game_option();

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	mining();
}