﻿#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//가위바위보 상수 정의
#define rock 0
#define scissors 1
#define paper 2

// 카지노
SceneID home_scene, enhance_scene, mining_scene, casino_main_scene, rsp_scene, odd_scene;

ObjectID menu1, menu2, rsp_image, odd_image, casino_image, pickax;
ObjectID enter_casino_button, enter_enhance_button, enter_mining_button, enter_rsp_button, enter_odd_button;
ObjectID home_button, mining_button, enhance_button, rsp_restart_button, odd_restart_button, rsp_exit_button, odd_exit_button, odd_result_button;
ObjectID rsp_machine, introduction, randEnemy;
ObjectID select_rsp_button[3], percentT[3], percentF[3], handMe[3], resultT[4], resultF[4], handEnermy[3];
ObjectID betting_odd, betting_even, back, front[11], percentTT[6], percentFF[6];

SoundID casino_BGM, button_sound, win_sound, draw_sound, lose_sound;

TimerID mining_timer, mining_wait_timer, mining_wait_space_timer;

int axLevel = 1;
int money = 0, enhanceMoney = 200;

int magnification[10] = { 1,2,3,1,1,2,2,1,5,20 };
int magnification2[10] = { 1,2,1,5,1,2,3,1,3,2 };
int battingPercent = 10, enermy;
float cost;
int checking;
int mining_wait_space_time = 0;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

SoundID createSound(const char* sound, bool play) {
	SoundID music = createSound(sound);

	if (play) playSound(music);

	return music;
}

void game_option() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
}

void object_state(bool state) {
	if (state) {
		showObject(enter_casino_button);
		showObject(enter_enhance_button);
		showObject(enter_mining_button);
	}
	else {
		hideObject(enter_casino_button);
		hideObject(enter_enhance_button);
		hideObject(enter_mining_button);
	}
}

void create_scene() {
	// 메인화면
	home_scene = createScene("", "source/picture/main/background.png");

	// 강화
	enhance_scene = createScene("", "source/picture/main/background.png");

	// 채광
	mining_scene = createScene("", "source/picture/main/background.png");

	// 도박
	casino_main_scene = createScene("", "source/picture/casino/pub.png");
	rsp_scene = createScene("", "source/picture/casino/rspBackground.png");
	odd_scene = createScene("", "source/picture/casino/oddBackground.png");

}

void create_object() {
	// 메인화면
	enter_casino_button = createObject("source/picture/casino/ladderButton.png", home_scene, 10, 0, true);
	scaleObject(enter_casino_button, 0.1f);
	enter_enhance_button = createObject("source/picture/casino/ladderButton.png", home_scene, 40, 0, true);
	scaleObject(enter_enhance_button, 0.1f);
	enter_mining_button = createObject("source/picture/casino/ladderButton.png", home_scene, 70, 0, true);
	scaleObject(enter_mining_button, 0.1f);

	// 강화
	enhance_button = createObject("source/picture/enhance/enhanceBtn.jpg", enhance_scene, 800, 400, true);
	pickax = createObject("source/picture/enhance/pickax1.jpg", enhance_scene, 200, 200, true);
	scaleObject(pickax, 0.3f);

	// 채광

	// 도박
	casino_image = createObject("source/picture/casino/casino.png", casino_main_scene, 20, 35, true);

	menu1 = createObject("source/picture/casino/gamemachine.png", casino_main_scene, 298, 150, true);
	scaleObject(menu1, 0.3f);
	menu2 = createObject("source/picture/casino/gamemachine.png", casino_main_scene, 700, 150, true);
	scaleObject(menu2, 0.3f);

	rsp_image = createObject("source/picture/casino/RSP.png", casino_main_scene, 360, 261, true);
	scaleObject(rsp_image, 0.25f);
	odd_image = createObject("source/picture/casino/odd.png", casino_main_scene, 775, 261, true);
	scaleObject(odd_image, 0.25f);

	enter_rsp_button = createObject("source/picture/casino/rspButton.png", casino_main_scene, 298, 65, true);
	scaleObject(enter_rsp_button, 0.8f);
	enter_odd_button = createObject("source/picture/casino/oddButton.png", casino_main_scene, 704, 65, true);
	scaleObject(enter_odd_button, 0.8f);

	rsp_machine = createObject("source/picture/casino/rspMachine.png", rsp_scene, 70, 80, true);
	scaleObject(rsp_machine, 1.3f);

	select_rsp_button[0] = createObject("source/picture/casino/mookButton.png", rsp_scene, 200, 110, true);
	select_rsp_button[1] = createObject("source/picture/casino/jjiButton.png", rsp_scene, 360, 110, true);
	select_rsp_button[2] = createObject("source/picture/casino/bbaButton.png", rsp_scene, 520, 110, true);
	for (int i = 0; i < 3; i++) scaleObject(select_rsp_button[i], 0.5f);

	home_button = createObject("source/picture/casino/exit.png", casino_main_scene, 5, 5, true);
	scaleObject(home_button, 0.06f);

	rsp_exit_button = createObject("source/picture/casino/exit.png", rsp_scene, 5, 5, true);
	scaleObject(rsp_exit_button, 0.06f);

	introduction = createObject("source/picture/casino/introduction.png", rsp_scene, 800, 370, true);
	scaleObject(introduction, 0.5f);

	percentT[0] = createObject("source/picture/casino/10%T.png", rsp_scene, 800, 150, true);
	percentT[1] = createObject("source/picture/casino/50%T.png", rsp_scene, 950, 150, false);
	percentT[2] = createObject("source/picture/casino/100%T.png", rsp_scene, 1100, 150, false);
	for (int i = 0; i < 3; i++) scaleObject(percentT[i], 0.5f);

	percentF[0] = createObject("source/picture/casino/10%F.png", rsp_scene, 800, 150, false);
	percentF[1] = createObject("source/picture/casino/50%F.png", rsp_scene, 945, 150, true);
	percentF[2] = createObject("source/picture/casino/100%F.png", rsp_scene, 1100, 150, true);
	for (int i = 0; i < 3; i++) scaleObject(percentF[i], 0.5f);

	handEnermy[rock] = createObject("source/picture/casino/mook3.png", rsp_scene, 230, 350, false);
	handEnermy[scissors] = createObject("source/picture/casino/jji3.png", rsp_scene, 230, 350, false);
	handEnermy[paper] = createObject("source/picture/casino/bba3.png", rsp_scene, 230, 340, false);

	handMe[rock] = createObject("source/picture/casino/mook2.png", rsp_scene, 560, 380, false);
	handMe[scissors] = createObject("source/picture/casino/jji2.png", rsp_scene, 550, 380, false);
	handMe[paper] = createObject("source/picture/casino/bba2.png", rsp_scene, 560, 380, false);
	for (int i = 0; i < 3; i++) scaleObject(handMe[i], 0.5f);

	resultT[0] = createObject("source/picture/casino/winT.png", rsp_scene, 160, 530, false);
	resultT[1] = createObject("source/picture/casino/winT.png", rsp_scene, 420, 530, false);
	resultT[2] = createObject("source/picture/casino/drawT.png", rsp_scene, 160, 270, false);
	resultT[3] = createObject("source/picture/casino/loseT.png", rsp_scene, 420, 270, false);
	for (int i = 0; i < 4; i++) scaleObject(resultT[i], 0.25f);

	resultF[0] = createObject("source/picture/casino/winF.png", rsp_scene, 160, 530, true);
	resultF[1] = createObject("source/picture/casino/winF.png", rsp_scene, 420, 530, true);
	resultF[2] = createObject("source/picture/casino/drawF.png", rsp_scene, 160, 270, true);
	resultF[3] = createObject("source/picture/casino/loseF.png", rsp_scene, 420, 270, true);
	for (int i = 0; i < 4; i++) scaleObject(resultF[i], 0.25f);

	rsp_restart_button = createObject("source/picture/casino/restart.png", rsp_scene, 115, 110, false);

	// 홀짝
	betting_odd = createObject("source/picture/casino/hol.png", odd_scene, 150, 360, true);
	scaleObject(betting_odd, 2.3f);
	betting_even = createObject("source/picture/casino/chak.png", odd_scene, 970, 360, true);
	scaleObject(betting_even, 2.3f);

	back = createObject("source/picture/casino/back.png", odd_scene, 333, 70, true);
	odd_result_button = createObject("source/picture/casino/flip.png", odd_scene, 450, 70, false);
	scaleObject(odd_result_button, 1.2f);

	odd_restart_button = createObject("source/picture/casino/restart.png", odd_scene, 330, 10, false);

	odd_exit_button = createObject("source/picture/casino/exit.png", odd_scene, 5, 5, true);
	scaleObject(odd_exit_button, 0.06f);

	char frontnum[50];
	for (int i = 1; i < 11; i++) {
		sprintf_s(frontnum, "%d.png", i);
		front[i] = createObject(frontnum, odd_scene, 408, 95, false);
		scaleObject(front[i], 1.02f);
	}

	percentTT[0] = createObject("source/picture/casino/10%T.png", odd_scene, 15, 200, false);
	percentTT[1] = createObject("source/picture/casino/50%T.png", odd_scene, 160, 200, false);
	percentTT[2] = createObject("source/picture/casino/100%T.png", odd_scene, 315, 200, false);
	for (int i = 0; i < 3; i++) scaleObject(percentTT[i], 0.5f);

	percentFF[0] = createObject("source/picture/casino/10%F.png", odd_scene, 15, 200, false);
	percentFF[1] = createObject("source/picture/casino/50%F.png", odd_scene, 160, 200, false);
	percentFF[2] = createObject("source/picture/casino/100%F.png", odd_scene, 315, 200, false);
	for (int i = 0; i < 3; i++) scaleObject(percentFF[i], 0.5f);

	percentTT[3] = createObject("source/picture/casino/10%T.png", odd_scene, 837, 200, false);
	percentTT[4] = createObject("source/picture/casino/50%T.png", odd_scene, 982, 200, false);
	percentTT[5] = createObject("source/picture/casino/100%T.png", odd_scene, 1137, 200, false);
	for (int i = 3; i < 6; i++) scaleObject(percentTT[i], 0.5f);

	percentFF[3] = createObject("source/picture/casino/10%F.png", odd_scene, 837, 200, false);
	percentFF[4] = createObject("source/picture/casino/50%F.png", odd_scene, 982, 200, false);
	percentFF[5] = createObject("source/picture/casino/100%F.png", odd_scene, 1137, 200, false);
	for (int i = 3; i < 6; i++) scaleObject(percentFF[i], 0.5f);
}

void create_sound() {
	// 도박
	casino_BGM = createSound("source/sound/CasinoBGM.mp3", true);
	button_sound = createSound("source/sound/buttonSound.mp3", false);
	win_sound = createSound("source/sound/win.mp3", false);
	draw_sound = createSound("source/sound/draw.mp3", false);
	lose_sound = createSound("source/sound/lose.mp3", false);
}

void create_timer() {
	// 채광
	mining_timer = createTimer(30.0f);
	setTimer(mining_timer, 30.0f);

	mining_wait_timer = createTimer(180.0f);
	setTimer(mining_wait_timer, 180.0f);

	mining_wait_space_timer = createTimer(1.0f);
	setTimer(mining_wait_timer, 1.0f);
}

void game_init() {
	game_option();

	srand(unsigned(time(NULL)));

	create_scene();
	create_object();
	create_sound();
	create_timer();

	// 메인화면

	// 강화

	// 채광

	// 도박
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 강화

void pickax_enhance() {
	if (money >= enhanceMoney) {
		money -= enhanceMoney;

		int probNum = rand() % 10 + 1;      //확률 조정?

		if (probNum % 2 == 0) {				//강화성공의 경우
			axLevel++;

			char buf[50];
			sprintf_s(buf, "Images/pickax%d.jpg", axLevel);
			setObjectImage(pickax, buf);

			showMessage("Success!!");

			enhanceMoney *= 2;              //단계별 강화비용 조작
		}
		else showMessage("Fail!!");

		if (axLevel == 10) hideObject(enhance_button);
	}
	else showMessage("You have not enough Money!!!!");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 채광

void start_mining() {
	showMessage("채광은 30초동안만 가능합니다. 최대한 많은 돈을 벌어보세요!");

	startTimer(mining_timer);
}
void end_mining() {
	stopTimer(mining_timer);

	showMessage("시간 종료!");

	printf("현재 잔액 = %d", money);

	startTimer(mining_wait_timer);
}

int mining(int axLevel) {
	for (int i = 1; i <= 10; i++) {
		if (axLevel == i) {
			float cost = pow(2, i - 1);

			money += int(cost);
		}
	}

	return money;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 카지노

//배팅 비율별 비용 계산 함수
void calculusCost() {
	if (battingPercent == 10) {
		cost = money * 0.1f;
	}
	if (battingPercent == 50) {
		cost = money * 0.5f;
	}
	if (battingPercent == 100) {
		cost = money;
	}
}

//가위바위보 승패 판별 함수
int RspGame(int j, int enermy) {
	if (j == enermy) return 0;
	else return (j == rock && enermy == scissors) || (j == paper && enermy == rock) || (j == scissors && enermy == paper) ? 1 : -1;
}

//보상 계산 함수(차감)
void minusMoney() {
	money -= cost;
	showMessage("돈을 잃었습니다.");

	if (money == 0) {
		enterScene(home_scene);
		showMessage("광질 열심히 하라우~");
	}
}

//보상 계산 함수(증가)
void plusMoney() {
	money -= cost;

	int randomIndex = rand() % 10;
	money += cost * magnification[randomIndex];

	if (magnification[randomIndex] == 1) showMessage("1배에 당첨됐습니다!");
	else if (magnification[randomIndex] == 2) showMessage("2배에 당첨됐습니다!");
	else if (magnification[randomIndex] == 3) showMessage("3배에 당첨됐습니다!");
	else if (magnification[randomIndex] == 5) showMessage("5배에 당첨됐습니다!");
	else if (magnification[randomIndex] == 20) showMessage("20배에 당첨됐습니다!");
}

//홀짝 보상 계산 함수(증가)
void plusMoney2() {
	money -= cost;

	int randomIndex = rand() % 10;
	money += cost * magnification2[randomIndex];

	if (magnification2[randomIndex] == 1) showMessage("1배에 당첨됐습니다!");
	if (magnification2[randomIndex] == 2) showMessage("2배에 당첨됐습니다!");
	if (magnification2[randomIndex] == 3) showMessage("3배에 당첨됐습니다!");
	if (magnification2[randomIndex] == 5) showMessage("5배에 당첨됐습니다!");
}

//가위바위보 게임 종료 함수
void endRsp() {
	for (int i = 0; i < 3; i++) {
		hideObject(select_rsp_button[i]);
	}
	showObject(rsp_restart_button);
}

//가위바위보 결과 처리 함수
void resulProcess(int result) {
	switch (result) {
	case -1:
		hideObject(resultF[3]);
		showObject(resultT[3]);
		printf("before = %d\n", money);
		minusMoney();
		printf("after = %d\n", money);
		playSound(lose_sound);
		break;
	case 0:
		hideObject(resultF[2]);
		showObject(resultT[2]);
		playSound(draw_sound);
		printf("%d\n", money);
		showMessage("다행히 돈을 잃진 않았습니다!");
		break;
	case 1:
		hideObject(resultF[0]);
		hideObject(resultF[1]);
		showObject(resultT[0]);
		showObject(resultT[1]);
		printf("before = %d\n", money);
		plusMoney();
		printf("after = %d\n", money);
		playSound(win_sound);
		break;
	}

	endRsp();
}

//홀짝 승패 판별 함수
int calnum(int num, int ob) {
	if (num % 2 == 0 && ob == 2) {
		return 1;
	}
	else if (num % 2 == 1 && ob == 1) {
		return 1;
	}
	else {
		return -1;
	}
}

//홀짝 결과 처리 함수
void resulPProcess(int result) {
	switch (result) {
	case -1:
		printf("before = %d\n", money);
		minusMoney();
		printf("after = %d\n", money);
		playSound(lose_sound);
		break;
	case 1:
		printf("before = %d\n", money);
		plusMoney2();
		printf("after = %d\n", money);
		playSound(win_sound);
		break;
	}
}

//홀짝 게임 종료 함수
void endOdd() {
	hideObject(betting_odd);
	hideObject(betting_even);

	for (int i = 0; i < 6; i++) {
		hideObject(percentTT[i]);
		hideObject(percentFF[i]);
	}

	showObject(odd_restart_button);
}

//홀짝 숫자뽑기 및 게임 진행 함수(결과, 종료 등)
void randomnum(int j) {
	int randomTemp = rand() % 10 + 1;

	for (int i = 1; i < 11; i++) {
		if (randomTemp == i) {
			hideObject(back);
			showObject(front[i]);
			int result = calnum(i, j);
			resulPProcess(result);
		}
	}

	endOdd();
}

void percent_state(int a, int b, int c, int d, int e, int f, int g, int type) {
	switch (type) {
	case 0:
		hideObject(percentF[a]);
		hideObject(percentT[b]);
		hideObject(percentT[c]);
		showObject(percentT[d]);
		showObject(percentF[e]);
		showObject(percentF[f]);
		battingPercent = g;

		break;
	case 1:
		hideObject(percentFF[a]);
		hideObject(percentTT[b]);
		hideObject(percentTT[c]);
		showObject(percentTT[d]);
		showObject(percentFF[e]);
		showObject(percentFF[f]);
		battingPercent = g;

		break;
	}
}

void handMe_state(int a, int b, int c, int d) {
	showObject(handMe[a]);
	hideObject(handMe[b]);
	hideObject(handMe[c]);
	showObject(handEnermy[enermy]);

	int result = RspGame(d, enermy);
	resulProcess(result);
}

//타이머 콜백 함수
void timerCallback(TimerID timer) {
	if (timer == mining_timer) end_mining();
	else if (timer == mining_wait_timer) {
		stopTimer(mining_wait_timer);

		printf("이제 채광이 가능합니다!");
	}

	while (mining_wait_space_time < 180) {
		if (timer == mining_wait_space_timer) {
			mining_wait_space_time += 1;
		}
	}
}

//사운드 콜백 함수 
void soundCallback(SoundID sound) {
	if (sound == casino_BGM) {
		playSound(casino_BGM);
	}
}

//마우스 콜백 함수
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == home_button) {
		enterScene(home_scene);
		object_state(true);
	}

	if (object == enter_casino_button) {
		enterScene(casino_main_scene);
		object_state(false);
	}
	else if (object == enter_enhance_button) {
		enterScene(enhance_scene);
		object_state(false);
	}
	else if (object == enter_mining_button) {
		if (mining_wait_space_time != 180) {
			char buf[50];
			sprintf_s(buf, "남은 시간 = %d초", 180 - mining_wait_space_time);
			showMessage(buf);
		}
		else if (mining_wait_space_time == 180) {
			enterScene(mining_scene);
			start_mining();

			mining_wait_space_time = 0;
		}

		object_state(false);
	}

	// 강화
	if (object == enhance_button) pickax_enhance();

	// 채광
	if (object == mining_button) mining(axLevel);

	// 도박
	if (object == (enter_odd_button || enter_rsp_button)) {
		battingPercent = 10;

		if (object == enter_odd_button) enterScene(odd_scene);
		else enterScene(rsp_scene);
	}

	if (object == rsp_exit_button) {	//가위바위보 게임 관련 코드
		enterScene(casino_main_scene);

		for (int i = 0; i < 3; i++) {
			hideObject(handMe[i]);
			hideObject(handEnermy[i]);
		}
		for (int i = 0; i < 4; i++) {
			hideObject(resultT[i]);
			showObject(resultF[i]);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (object == percentF[i]) {
			playSound(button_sound);

			if (i == 0) percent_state(i, i + 1, i + 2, i, i + 1, i + 2, 10, 0);
			else if (i == 1) percent_state(i, i - 1, i + 1, i, i - 1, i + 1, 50, 0);
			else if (i == 2) percent_state(i, i - 1, i - 2, i, i - 1, i - 2, 100, 0);
		}
	}

	for (int j = 0; j < 3; j++) {
		if (object == select_rsp_button[j]) {
			calculusCost();
			hideObject(handEnermy[enermy]);
			enermy = rand() % 3;

			if (j == 0) handMe_state(j, j + 1, j + 2, j);
			else if (j == 1) handMe_state(j, j - 1, j + 1, j);
			else if (j == 2) handMe_state(j, j - 1, j - 2, j);
		}
	}

	if (object == rsp_restart_button) {
		playSound(button_sound);
		hideObject(rsp_restart_button);

		for (int i = 0; i < 3; i++) {
			showObject(select_rsp_button[i]);
			hideObject(handMe[i]);
			hideObject(handEnermy[i]);
		}
		for (int i = 0; i < 4; i++) {
			hideObject(resultT[i]);
			showObject(resultF[i]);
		}
	}

	/* 홀짝 관련 코드*/
	if (object == betting_odd) {
		playSound(button_sound);
		showObject(odd_result_button);

		for (int i = 1; i < 3; i++) {
			hideObject(percentFF[0]);
			hideObject(percentTT[i]);
			showObject(percentTT[0]);
			showObject(percentFF[i]);
		}
		for (int i = 3; i < 6; i++) {
			hideObject(percentTT[i]);
			hideObject(percentFF[i]);
		}
		checking = 1;
	}

	if (object == betting_even) {
		playSound(button_sound);
		showObject(odd_result_button);

		for (int i = 4; i < 6; i++) {
			hideObject(percentFF[3]);
			hideObject(percentTT[i]);
			showObject(percentTT[3]);
			showObject(percentFF[i]);
		}
		for (int i = 0; i < 3; i++) {
			hideObject(percentTT[i]);
			hideObject(percentFF[i]);
		}
		checking = 2;
	}

	for (int i = 0; i < 6; i++) {
		if (object == percentFF[i]) {
			playSound(button_sound);
			if (i == 0 || i == 3) percent_state(i, i + 1, i + 2, i, i + 1, i + 2, 10, 1);
			else if (i == 1 || i == 4) percent_state(i, i - 1, i + 1, i, i - 1, i + 1, 50, 1);
			else if (i == 2 || i == 5) percent_state(i, i - 1, i - 2, i, i - 1, i - 2, 100, 1);
		}
	}

	if (object == odd_result_button) {
		calculusCost();
		randomnum(checking);
		hideObject(odd_result_button);
	}
	else if (object == odd_restart_button) {
		playSound(button_sound);
		showObject(betting_odd);
		showObject(betting_even);

		for (int i = 1; i < 11; i++) {
			hideObject(front[i]);
		}
		for (int i = 0; i < 6; i++) {
			hideObject(percentFF[i]);
			hideObject(percentTT[i]);
		}

		showObject(back);
		hideObject(odd_restart_button);
	}
	if (object == odd_exit_button) {
		enterScene(casino_main_scene);

		for (int i = 1; i < 11; i++) {
			hideObject(front[i]);
		}
		for (int i = 0; i < 6; i++) {
			hideObject(percentTT[i]);
			hideObject(percentFF[i]);
		}

		hideObject(odd_restart_button);
		showObject(back);
		showObject(betting_odd);
		showObject(betting_even);
	}
}

//메인 함수
int main() {
	game_init();

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);

	startGame(home_scene);
}