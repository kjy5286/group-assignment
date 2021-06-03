#define _CRT_SECURE_NO_WARNINGS
#include <bangtal.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//가위바위보 묵, 찌, 빠
#define mook 0
#define jji 1
#define bba 2

//카지노 게임 전역변수 선언
SceneID pubMain,rspBackground,oddBackground;
ObjectID menu1,menu2,RSP,odd,rspBut,oddBut,casino,enterLobby;
ObjectID rspMachine, exitButton, introduction,randEnemy,restart;
ObjectID rspButton[3],percentT[3], percentF[3],handMe[3],resultT[4],resultF[4], handEnermy[3];
ObjectID hol, chak, back, front[11], percentTT[6], percentFF[6], flip, regame, exitBut;
SoundID CasinoBGM,buttonSound,win,draw,lose;
int magnification[10] = { 1,2,3,1,1,2,2,1,5,20 };
int magnification2[10] = { 1,2,1,5,1,2,3,1,3,2 };
int battingPercent = 10,enermy;
double cost;
int jay;

//모두 사용할 변수(돈, 메인 화면)
double money = 100000;
SceneID Lobby;

//사운드 생성함수
SoundID createSound2(const char* sound, bool que) {
	SoundID music = createSound(sound);
	if (que == true) {
		playSound(music);
	}
	return music;
}

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
	return (j == mook && enermy == jji) || (j == bba && enermy == mook) || (j == jji && enermy == bba) ? 1 : -1;
}

//보상 계산 함수(차감)
void minusMoney() {
	money -= cost;
	showMessage("돈을 잃었습니다.");
	if (money == 0) {
		enterScene(Lobby);
		showMessage("광질 열심히 하라우~");
	}
}

//보상 계산 함수(증가)
void plusMoney() {
	money -= cost;
	int randomIndex = rand() % 10;
	money += cost * magnification[randomIndex];
	if (magnification[randomIndex] == 1) {
		showMessage("1배에 당첨됐습니다!");
	}
	if (magnification[randomIndex] == 2) {
		showMessage("2배에 당첨됐습니다!");
	}
	if (magnification[randomIndex] == 3) {
		showMessage("3배에 당첨됐습니다!");
	}
	if (magnification[randomIndex] == 5) {
		showMessage("5배에 당첨됐습니다!");
	}
	if (magnification[randomIndex] == 20) {
		showMessage("20배에 당첨됐습니다!");
	}
}

//홀짝 보상 계산 함수(증가)
void plusMoney2() {
	money -= cost;
	int randomIndex = rand() % 10;
	money += cost * magnification2[randomIndex];
	if (magnification2[randomIndex] == 1) {
		showMessage("1배에 당첨됐습니다!");
	}
	if (magnification2[randomIndex] == 2) {
		showMessage("2배에 당첨됐습니다!");
	}
	if (magnification2[randomIndex] == 3) {
		showMessage("3배에 당첨됐습니다!");
	}
	if (magnification2[randomIndex] == 5) {
		showMessage("5배에 당첨됐습니다!");
	}
}

//가위바위보 게임 종료 함수
void endRsp() {
	for (int i = 0; i < 3; i++) {
		hideObject(rspButton[i]);
	}
	showObject(restart);
}

//가위바위보 결과 처리 함수
void resulProcess(int result) {
	switch (result) {
	case -1:
		hideObject(resultF[3]);
		showObject(resultT[3]);
		printf("before%lf\n", money);
		minusMoney();
		printf("after%lf\n", money);
		playSound(lose);
		break;
	case 0:
		hideObject(resultF[2]);
		showObject(resultT[2]);
		playSound(draw);
		printf("%lf\n", money);
		showMessage("다행히 돈을 잃진 않았습니다!");
		break;
	case 1:
		hideObject(resultF[0]);
		hideObject(resultF[1]);
		showObject(resultT[0]);
		showObject(resultT[1]);
		printf("before%lf\n", money);
		plusMoney();
		printf("after%lf\n", money);
		playSound(win);
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
		printf("before%lf\n", money);
		minusMoney();
		printf("after%lf\n", money);
		playSound(lose);
		break;
	case 1:
		printf("before%lf\n", money);
		plusMoney2();
		printf("after%lf\n", money);
		playSound(win);
		break;
	}
}

//홀짝 게임 종료 함수
void endOdd() {
	hideObject(hol);
	hideObject(chak);
	for (int i = 0; i < 6; i++) {
		hideObject(percentTT[i]);
		hideObject(percentFF[i]);
	}
	showObject(regame);
}

//홀짝 숫자뽑기 및 게임 진행 함수(결과, 종료 등)
void randomnum(int j) {
	int randomTemp = rand() % 10 + 1;
	for (int i = 1; i < 11; i ++) {
		if (randomTemp == i) {
			hideObject(back);
			showObject(front[i]);
			int result = calnum(i, j);
			resulPProcess(result);
		}
	}
	endOdd();
}

//사운드 콜백 함수 
void soundCallback(SoundID sound) {
	if (sound == CasinoBGM) {
		playSound(CasinoBGM);
	}
}

//마우스 콜백 함수
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	//카지노 전체 관련 코드
	if (object == oddBut) {
		battingPercent = 10;
		enterScene(oddBackground);
	}
	if (object == rspBut) {
		battingPercent = 10;
		enterScene(rspBackground);
	}
	if (object == enterLobby) {
		enterScene(Lobby);
	}

	//가위바위보 게임 관련 코드
	if (object == exitButton) {
		enterScene(pubMain);
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
			playSound(buttonSound);
			if (i == 0) {
				hideObject(percentF[i]);
				hideObject(percentT[i + 1]);
				hideObject(percentT[i + 2]);
				showObject(percentT[i]);
				showObject(percentF[i+1]);
				showObject(percentF[i+2]);
				battingPercent = 10;
			}
			if (i == 1) {
				hideObject(percentF[i]);
				hideObject(percentT[i - 1]);
				hideObject(percentT[i + 1]);
				showObject(percentT[i]);
				showObject(percentF[i - 1]);
				showObject(percentF[i + 1]);
				battingPercent = 50;
			}
			if (i == 2) {
				hideObject(percentF[i]);
				hideObject(percentT[i - 1]);
				hideObject(percentT[i - 2]);
				showObject(percentT[i]);
				showObject(percentF[i - 1]);
				showObject(percentF[i - 2]);
				battingPercent = 100;
			}
		}
	}
	for (int j = 0; j < 3; j++) {
		if (object == rspButton[j]) {
			calculusCost();
			hideObject(handEnermy[enermy]);
			enermy = rand() % 3;
			if (j == 0) {
				showObject(handMe[j]);
				hideObject(handMe[j + 1]);	
				hideObject(handMe[j + 2]);
				showObject(handEnermy[enermy]);
				int result = RspGame(j, enermy);
				resulProcess(result);
			}
			if (j == 1) {
				showObject(handMe[j]);
				hideObject(handMe[j - 1]);
				hideObject(handMe[j + 1]);
				showObject(handEnermy[enermy]);
				int result = RspGame(j, enermy);
				resulProcess(result);
			}
			if (j == 2) {
				showObject(handMe[j]);
				hideObject(handMe[j - 1]);
				hideObject(handMe[j - 2]);
				showObject(handEnermy[enermy]);
				int result = RspGame(j, enermy);
				resulProcess(result);
			}
		}
	}
	if (object == restart) {
		playSound(buttonSound);
		hideObject(restart);
		for (int i = 0; i < 3; i++) {
			showObject(rspButton[i]);
			hideObject(handMe[i]);
			hideObject(handEnermy[i]);
		}
		for (int i = 0; i < 4; i++) {
			hideObject(resultT[i]);
			showObject(resultF[i]);
		}
	}

/* 홀짝 관련 코드*/ 
	if (object == hol) {
		showObject(flip);
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
		jay = 1;
	}
	if (object == chak) {
		showObject(flip);
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
		jay = 2;
	}
	for (int i = 0; i < 6; i++) {
		if (object == percentFF[i]) {
			playSound(buttonSound);
			if (i == 0 || i == 3) {
				hideObject(percentFF[i]);
				hideObject(percentTT[i + 1]);
				hideObject(percentTT[i + 2]);
				showObject(percentTT[i]);
				showObject(percentFF[i + 1]);
				showObject(percentFF[i + 2]);
				battingPercent = 10;
			}
			if (i == 1 || i == 4) {
				hideObject(percentFF[i]);
				hideObject(percentTT[i - 1]);
				hideObject(percentTT[i + 1]);
				showObject(percentTT[i]);
				showObject(percentFF[i - 1]);
				showObject(percentFF[i + 1]);
				battingPercent = 50;
			}
			if (i == 2 || i == 5) {
				hideObject(percentFF[i]);
				hideObject(percentTT[i - 1]);
				hideObject(percentTT[i - 2]);
				showObject(percentTT[i]);
				showObject(percentFF[i - 1]);
				showObject(percentFF[i - 2]);
				battingPercent = 100;
			}
		}
	}
	if (object == flip) {
		calculusCost();
		randomnum(jay);
		hideObject(flip);
	}
	if (object == regame) {
		showObject(hol);
		showObject(chak);
		for (int i = 1; i < 11; i++) {
			hideObject(front[i]);
		}
		for (int i = 0; i < 6; i++) {
			hideObject(percentFF[i]);
			hideObject(percentTT[i]);
		}
		showObject(back);
		hideObject(regame);
	}
	if (object == exitBut) {
		enterScene(pubMain);
		for (int i = 1; i < 11; i++) {
			hideObject(front[i]);
		}
		for (int i = 0; i < 6; i++) {
			hideObject(percentTT[i]);
			hideObject(percentFF[i]);
		}
		hideObject(regame);
		showObject(back);
		showObject(hol);
		showObject(chak);
	}
}

//오브젝트 생성 함수
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown == true) {
		showObject(object);
	}
	return object;
}

//카지노 변수 할당
void MakeCasino() {
	pubMain = createScene("", "pub.png");
	rspBackground = createScene("", "rspBackground.png");
	oddBackground = createScene("", "oddBackground.png");

	casino = createObject("casino.png", pubMain, 20, 35, true);
	menu1 = createObject("gamemachine.png", pubMain, 298, 150, true);
	scaleObject(menu1, 0.3f);
	menu2 = createObject("gamemachine.png", pubMain, 700, 150, true);
	scaleObject(menu2, 0.3f);
	RSP = createObject("RSP.png", pubMain, 360, 261, true);
	scaleObject(RSP, 0.25f);
	odd = createObject("odd.png", pubMain, 775, 261, true);
	scaleObject(odd, 0.25f);
	rspBut = createObject("rspButton.png", pubMain, 298, 65, true);
	scaleObject(rspBut, 0.8f);
	oddBut = createObject("oddButton.png", pubMain, 704, 65, true);
	scaleObject(oddBut, 0.8f);

	rspMachine = createObject("rspMachine.png", rspBackground, 70, 80, true);
	scaleObject(rspMachine, 1.3f);
	rspButton[0] = createObject("mookButton.png", rspBackground, 200, 110, true);
	rspButton[1] = createObject("jjiButton.png", rspBackground, 360, 110, true);
	rspButton[2] = createObject("bbaButton.png", rspBackground, 520, 110, true);
	for (int i = 0; i < 3; i++) {
		scaleObject(rspButton[i], 0.5f);
	}
	enterLobby = createObject("exit.png", pubMain, 5, 5, true);
	scaleObject(enterLobby, 0.06f);
	exitButton = createObject("exit.png", rspBackground, 5, 5, true);
	scaleObject(exitButton, 0.06f);
	introduction = createObject("introduction.png", rspBackground, 800, 370, true);
	scaleObject(introduction, 0.5f);

	percentT[0] = createObject("10%T.png", rspBackground, 800, 150, true);
	percentT[1] = createObject("50%T.png", rspBackground, 950, 150, false);
	percentT[2] = createObject("100%T.png", rspBackground, 1100, 150, false);
	for (int i = 0; i < 3; i++) {
		scaleObject(percentT[i], 0.5f);
	}

	percentF[0] = createObject("10%F.png", rspBackground, 800, 150, false);
	percentF[1] = createObject("50%F.png", rspBackground, 945, 150, true);
	percentF[2] = createObject("100%F.png", rspBackground, 1100, 150, true);
	for (int i = 0; i < 3; i++) {
		scaleObject(percentF[i], 0.5f);
	}

	handEnermy[mook] = createObject("mook3.png", rspBackground, 230, 350, false);
	handEnermy[jji] = createObject("jji3.png", rspBackground, 230, 350, false);
	handEnermy[bba] = createObject("bba3.png", rspBackground, 230, 340, false);

	handMe[mook] = createObject("mook2.png", rspBackground, 560, 380, false);
	handMe[jji] = createObject("jji2.png", rspBackground, 550, 380, false);
	handMe[bba] = createObject("bba2.png", rspBackground, 560, 380, false);
	for (int i = 0; i < 3; i++) {
		scaleObject(handMe[i], 0.5f);
	}

	resultT[0] = createObject("winT.png", rspBackground, 160, 530, false);
	resultT[1] = createObject("winT.png", rspBackground, 420, 530, false);
	resultT[2] = createObject("drawT.png", rspBackground, 160, 270, false);
	resultT[3] = createObject("loseT.png", rspBackground, 420, 270, false);
	for (int i = 0; i < 4; i++) {
		scaleObject(resultT[i], 0.25f);
	}

	resultF[0] = createObject("winF.png", rspBackground, 160, 530, true);
	resultF[1] = createObject("winF.png", rspBackground, 420, 530, true);
	resultF[2] = createObject("drawF.png", rspBackground, 160, 270, true);
	resultF[3] = createObject("loseF.png", rspBackground, 420, 270, true);
	for (int i = 0; i < 4; i++) {
		scaleObject(resultF[i], 0.25f);
	}

	restart = createObject("restart.png", rspBackground, 115, 110, false);

	CasinoBGM = createSound2("CasinoBGM.mp3", true);
	buttonSound = createSound2("buttonSound.mp3", false);
	win = createSound2("win.mp3", false);
	draw = createSound2("draw.mp3", false);
	lose = createSound2("lose.mp3", false);

	// 홀짝
	hol = createObject("hol.png", oddBackground, 150, 360, true);
	scaleObject(hol, 2.3f);
	chak = createObject("chak.png", oddBackground, 970, 360, true);
	scaleObject(chak, 2.3f);
	back = createObject("back.png", oddBackground, 333, 70, true);
	flip = createObject("flip.png", oddBackground, 450, 70, false);
	scaleObject(flip, 1.2f);
	regame = createObject("restart.png", oddBackground, 330, 10, false);
	exitBut = createObject("exit.png", oddBackground, 5, 5, true);
	scaleObject(exitBut, 0.06f);

	char frontnum[50];
	for (int i = 1; i < 11; i++) {
		sprintf(frontnum, "%d.png", i);
		front[i] = createObject(frontnum, oddBackground, 408, 95, false);
		scaleObject(front[i], 1.02f);
	}

	percentTT[0] = createObject("10%T.png", oddBackground, 15, 200, false);
	percentTT[1] = createObject("50%T.png", oddBackground, 160, 200, false);
	percentTT[2] = createObject("100%T.png", oddBackground, 315, 200, false);
	for (int i = 0; i < 3; i++) {
		scaleObject(percentTT[i], 0.5f);
	}

	percentFF[0] = createObject("10%F.png", oddBackground, 15, 200, false);
	percentFF[1] = createObject("50%F.png", oddBackground, 160, 200, false);
	percentFF[2] = createObject("100%F.png", oddBackground, 315, 200, false);
	for (int i = 0; i < 3; i++) {
		scaleObject(percentFF[i], 0.5f);
	}

	percentTT[3] = createObject("10%T.png", oddBackground, 837, 200, false);
	percentTT[4] = createObject("50%T.png", oddBackground, 982, 200, false);
	percentTT[5] = createObject("100%T.png", oddBackground, 1137, 200, false);
	for (int i = 3; i < 6; i++) {
		scaleObject(percentTT[i], 0.5f);
	}

	percentFF[3] = createObject("10%F.png", oddBackground, 837, 200, false);
	percentFF[4] = createObject("50%F.png", oddBackground, 982, 200, false);
	percentFF[5] = createObject("100%F.png", oddBackground, 1137, 200, false);
	for (int i = 3; i < 6; i++) {
		scaleObject(percentFF[i], 0.5f);
	}
}

//메인 함수
int main() {
	srand(time(NULL));
	setMouseCallback(mouseCallback);
	setSoundCallback(soundCallback);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	Lobby = createScene("", "");
	MakeCasino();

	startGame(pubMain);
}