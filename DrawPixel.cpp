//ヘッダーファイル--------------------------------------------------------------------------------------------------------------------
#include "DxLib.h"
#include <math.h>
#include <time.h>

//色指定(共通）-----------------------------------------------------------------------------------------------------------------------
unsigned int Bk, Bl, Wh, Gl, Gr, Dr, Lg, Dg, Ld, Lr, Lb, Db, LLb, Pi, Ly, Yl;
#define Wh GetColor(255, 255, 255) //白
#define Bk GetColor(0, 0, 0)       //黒
#define Gl GetColor(102, 102, 102) //灰色
#define Bl GetColor(179, 255, 255) //青
#define LLb GetColor(25, 180, 255) //LimeLblue
#define Lb GetColor(30, 125, 200)  //Limeblue
#define Db GetColor(5, 60, 125)    //Darkblue
#define Gr GetColor(150, 230, 0)   //緑
#define Lg GetColor(120, 200, 60)  //LimeGreen
#define Dg GetColor(0, 100, 0)     //DarkGreen
#define Lr GetColor(180, 30, 20)   //明るめ赤
#define Dr GetColor(135, 42, 42)   //ダークレッド
#define Ld GetColor(230, 230, 250) //ラベンダー
#define Pi GetColor(255, 153, 238) //ピンク
#define Ly GetColor(255, 255, 200) //明るい黄色
#define Yl GetColor(255, 255, 0)   //黄色	

//画面大きさ（横×縦）330×250 pixel　（共通）----------------------------------------------------------------------------------------
//上画面
#define UP_x1 160
#define UP_y1 40
#define UP_x2 490
#define UP_y2 290
#define UPCE_x 325
#define UPCE_y 165

//下画面
#define DW_x1 160
#define DW_y1 400
#define DW_x2 490
#define DW_y2 650
#define DWCE_x 325
#define DWCE_y 525

//ポケモンの数
#define PokeNum 10
//所持ポケモン
#define MAXPokemon 6

//カーソル（座標）
int MouseX, MouseY, MouseInput, LogType;

//属性
enum Element{ノーマル,火,水,草,光,闇};

//技
enum Move{なし,へんしん,ちょうはつ,キック,ビルドアップ,つるぎのまい,ソーラービーム, はっぱカッター, からにこもる, あなをほる,
			スペシウム,スクライド,ヘブンミックス,エンブレイス,ブレス,こうそくいどう,ものまね};

//ポケモン構造体宣言
typedef struct {
	char name[20];
	int ELE;
	int MAXHP;
	int HP;
	int ATK;
	int DEF;
	int SPD;
	int MV1;
	int MV2;
	int MV3;
	int MV4;
	int CNT1;
	int CNT2;
	int CNT3;
	int CNT4;
}Pokemon;

typedef struct {
	Pokemon pokemon;
}MyPokemon;

typedef struct {
	Pokemon pokemon;
}EnemyPokemon;

//プロトタイプ宣言--------------------------------------------------------------------------------------------------------------------
void ScreenReset();
void PokemonCreate(Pokemon *pokemon);
int Select1(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select2(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Screen1();
void Screen2();
void Screen3(MyPokemon* mypokemon);
void TurnCheck(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void PokemonChange(MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Myturn(int flg, MyPokemon *mypokemon, EnemyPokemon *enemypokemon);
void Enemyturn(int flg, MyPokemon *mypokemon, EnemyPokemon *enemypokemon);

//上記共通のため変更しない------------------------------------------------------------------------------------------------------------



// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//初期画面（変更しない）------------------------------------------------------------------
	SetMainWindowText("Nintendo DS Lite");
	ChangeWindowMode(TRUE);//ウィンドウモードで起動
	SetGraphMode(650, 700, 32); //画面の解像度指定　　
	SetBackgroundColor(255, 255, 255);
	SetWindowSizeChangeEnableFlag(FALSE); //画面サイズ変更不可

	// ＤＸライブラリ初期化処理---------------------------------------------------------------
	if (DxLib_Init() == -1)
	{
		return -1;    // エラーが起きたら直ちに終了
	}

	//起動音----------------------------------------------------------------------------------
	PlaySoundFile("DS起動音.mp3", DX_PLAYTYPE_BACK);

	//画面初期化------------------------------------------------------------------------------
	ScreenReset();

	//ポケモン生成
	Pokemon pokemon[PokeNum];
	PokemonCreate(pokemon);

	MyPokemon mypokemon[MAXPokemon] = { pokemon[1],pokemon[2],pokemon[4],0,0,0 };
	EnemyPokemon enemypokemon[MAXPokemon] = { pokemon[0],pokemon[3],0,0,0,0 };

	//画面選択(初期画面(1))
	int flg = 1;

	//戦闘
	while(1) {

		//flg=0の時、戦闘終了
		if (flg == 0 or enemypokemon[0].pokemon.HP <= 0) {
			SetFontSize(180);
			SetFontThickness(9);
			ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
			DrawString(0, 60, "あなたの", Bk);
			DrawString(0, 430, "勝利です", Bk);
			break;
		}

		//flg=1の時、画面1に移動
		else if (flg == 1) {
			Screen1();
			flg = Select1(flg, mypokemon, enemypokemon);
		}

		//flg=2の時、画面2に移動
		else if (flg == 2) {
			Screen2();
			flg = Select2(flg, mypokemon, enemypokemon);
		}

		//flg=3の時、画面3に移動
		else if (flg == 3) {
			Screen3(mypokemon);
			flg = Select3(flg, mypokemon, enemypokemon);
		}

	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();
	return 0; 
}

void ScreenReset() {

	//外枠描画--------------------------------------------------------------
	DrawBoxAA(0, 0, 650, 700, Bk, FALSE);

	//上画面描画------------------------------------------------------------
	DrawBoxAA(150, 30, 500, 300, Bk, FALSE);	//上画面外枠
	LoadGraphScreen(UP_x1, UP_y1, "ビーチ.jpg", FALSE);
	LoadGraphScreen(UP_x1+30, UP_y1+100, "ビリオン.png",TRUE);
	LoadGraphScreen(UP_x1 + 100, UP_y1 + 90, "めんつゆ.png", TRUE);
	LoadGraphScreen(UP_x1 + 200, UP_y1 + 30, "かぼやん.png", TRUE);
	DrawBoxAA(UP_x1, UP_y1, UP_x2, UP_y2, Bk, FALSE);	//上画面内枠

	//スピーカー
	int i, j, x, y;
	for (i = 0; i <= 2; i++) {
		for (j = 0; j <= 1; j++) {
			x = i * 25;
			y = j * 25;
			DrawCircleAA(50 + x, 150 + y, 3, 32, Bk, TRUE);
			DrawCircleAA(600 - x, 150 + y, 3, 32, Bk, TRUE);
		}
	}

	//真ん中の描画---------------------------------------------------------
	DrawBoxAA(0, 330, 650, 370, Bk, FALSE);		//真ん中の線
	DrawBoxAA(75, 330, 575, 370, Bk, FALSE);	//折り畳み線？
	DrawBoxAA(323, 342, 327, 358, Bk, TRUE);	//マイク真ん中
	DrawBoxAA(610, 335, 614, 365, Gl, TRUE);	//ライト左
	DrawBoxAA(620, 335, 624, 365, Gr, TRUE);	//ライト右

	//下画面描画------------------------------------------------------------
	DrawBoxAA(150, 390, 500, 660, Bk, FALSE);	//下画面外枠

	DrawBoxAA(63, 460, 87, 540, Bk, FALSE);		//十字縦
	DrawBoxAA(35, 488, 115, 512, Bk, FALSE);	//十字横
	DrawBoxAA(63, 488, 87, 512, Wh, FALSE);		//十字真ん中

	DrawBoxAA(74, 470, 76, 488, Gl, TRUE);		//十字線（上）
	DrawBoxAA(74, 512, 76, 530, Gl, TRUE);		//十字線（下）
	DrawBoxAA(45, 499, 63, 501, Gl, TRUE);		//十字線（右）
	DrawBoxAA(87, 499, 105, 501, Gl, TRUE);		//十字線（左）

	DrawCircleAA(575, 513, 18, 32, Bk, FALSE);	//Bボタン
	DrawCircleAA(575, 435, 18, 32, Bk, FALSE);	//Xボタン
	DrawCircleAA(536, 474, 18, 32, Bk, FALSE);	//Yボタン
	DrawCircleAA(614, 474, 18, 32, Bk, FALSE);	//Aボタン

	DrawCircleAA(532, 600, 9, 32, Bk, FALSE);	//STARTボタン
	DrawCircleAA(532, 640, 9, 32, Bk, FALSE);	//SELECTボタン

	//文字表示------------------------------------------------------------
	SetFontSize(12);
	SetFontThickness(9);
	DrawString(550, 595, "START", Gl);
	DrawString(550, 635, "SELECT", Gl);
	SetFontThickness(7);
	DrawString(334, 344, "MIC", Gl);
	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(569, 422, "X", Gl);
	DrawString(569, 500, "B", Gl);
	DrawString(530, 461, "Y", Gl);
	DrawString(608, 461, "A", Gl);
}

void PokemonCreate(Pokemon* pokemon) {
	pokemon[0] = { "メタモン",ノーマル,255,255,255,255,255,へんしん,なし,なし,なし,0,0,0,0 };
	pokemon[1] = { "ゴールドシップ",ノーマル,255,255,200,200,255,ちょうはつ,キック,ビルドアップ,つるぎのまい,0,0,0,0 };
	pokemon[2] = { "かぼやん",草,200,200,130,220,30,ソーラービーム,はっぱカッター,からにこもる,あなをほる,0,0,0,0 };
	pokemon[3] = { "ビリオン", 光,150,150,180,200,80,スペシウム,スクライド,ヘブンミックス,エンブレイス,0,0,0,0 };
	pokemon[4] = { "オラポン", 火,500,500,120,80,50,キック,ブレス,こうそくいどう,ものまね,0,0,0,0 };
}


//画面1描画処理--------------------------------------------------------------------------------------------------------
void Screen1() {

	//下画面内枠
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh, TRUE);	

	//・上のほうの背景
	DrawBoxAA(160, 400, 490, 412.5, Ld, TRUE);
	//・技左上座標
	DrawBoxAA(165, 435, 320, 510, Ld, TRUE);
	DrawBoxAA(165, 435, 320, 510, Bk, FALSE);
	DrawBoxAA(170, 440, 315, 505, Gl, TRUE);
	//・技右上座標
	DrawBoxAA(330, 435, 485, 510, Ld, TRUE);
	DrawBoxAA(330, 435, 485, 510, Bk, FALSE);
	DrawBoxAA(335, 440, 480, 505, Gl, TRUE);
	//・技左下座標
	DrawBoxAA(165, 520, 320, 595, Ld, TRUE);
	DrawBoxAA(165, 520, 320, 595, Bk, FALSE);
	DrawBoxAA(170, 525, 315, 590, Gl, TRUE);
	//・技右下座標
	DrawBoxAA(330, 520, 485, 595, Ld, TRUE);
	DrawBoxAA(330, 520, 485, 595, Bk, FALSE);
	DrawBoxAA(335, 525, 480, 590, Gl, TRUE);
	//・たたかう座標
	DrawBoxAA(180, 450, 470, 580, Lr, TRUE);
	DrawBoxAA(180, 450, 470, 580, Bk, FALSE);
	DrawBoxAA(195, 460, 455, 570, Dr, TRUE);
	//・ポケモン座標
	DrawBoxAA(160, 600, 490, 650, Lg, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Dg, TRUE);
	//・モンスターボール味方座標
	LoadGraphScreen(165, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(190, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(215, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(240, 405, "ボール.png", TRUE);
	LoadGraphScreen(265, 405, "ボール.png", TRUE);
	LoadGraphScreen(290, 405, "ボール.png", TRUE);
	//・モンスターボール敵座標
	LoadGraphScreen(395, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(410, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(425, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(440, 405, "ボール右上.png", TRUE);
	LoadGraphScreen(455, 405, "ボール右上.png", TRUE);
	LoadGraphScreen(470, 405, "ボール右上.png", TRUE);

	//下画面内枠
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 500, "たたかう", Wh);
	DrawString(295, 610, "ポケモン", Wh);

}

//画面2描画処理--------------------------------------------------------------------------------------------------------
void Screen2() {

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh,TRUE);	//下画面内枠
	
	//・上のほうの背景
	DrawBoxAA(160, 400, 490, 412.5, Ld, TRUE);
	//・技左上座標
	DrawBoxAA(165, 435, 320, 510, Ld, TRUE);
	DrawBoxAA(165, 435, 320, 510, Bk, FALSE);
	DrawBoxAA(170, 440, 315, 505, Gl, TRUE);
	//・技右上座標
	DrawBoxAA(330, 435, 485, 510, Ld, TRUE);
	DrawBoxAA(330, 435, 485, 510, Bk, FALSE);
	DrawBoxAA(335, 440, 480, 505, Gl, TRUE);
	//・技左下座標
	DrawBoxAA(165, 520, 320, 595, Ld, TRUE);
	DrawBoxAA(165, 520, 320, 595, Bk, FALSE);
	DrawBoxAA(170, 525, 315, 590, Gl, TRUE);
	//・技右下座標
	DrawBoxAA(330, 520, 485, 595, Ld, TRUE);
	DrawBoxAA(330, 520, 485, 595, Bk, FALSE);
	DrawBoxAA(335, 525, 480, 590, Gl, TRUE);
	//・もどる座標
	DrawBoxAA(160, 600, 490, 650, Lb, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Db, TRUE);

	//・モンスターボール味方座標
	LoadGraphScreen(165, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(190, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(215, 405, "モンスターボール.png", TRUE);
	LoadGraphScreen(240, 405, "ボール.png", TRUE);
	LoadGraphScreen(265, 405, "ボール.png", TRUE);
	LoadGraphScreen(290, 405, "ボール.png", TRUE);
	//・モンスターボール敵座標
	LoadGraphScreen(395, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(410, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(425, 405, "モンスターボール右上.png", TRUE);
	LoadGraphScreen(440, 405, "ボール右上.png", TRUE);
	LoadGraphScreen(455, 405, "ボール右上.png", TRUE);
	LoadGraphScreen(470, 405, "ボール右上.png", TRUE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//下画面内枠

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 610, " もどる ", Wh);

}

//画面3描画処理--------------------------------------------------------------------------------------------------------
void Screen3(MyPokemon* mypokemon) {

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Ld, TRUE);	//下画面内枠

	//・ポケモン1体目左座標
	DrawBoxAA(160, 410, 323, 470, LLb, TRUE);
	//・ポケモン2体目左座標
	DrawBoxAA(160, 475, 323, 535, LLb, TRUE);
	//・ポケモン3体目左座標
	DrawBoxAA(160, 540, 323, 600, LLb, TRUE);

	//・ポケモン1体目右座標
	DrawBoxAA(327, 420, 490, 480, LLb, TRUE);
	//・ポケモン2体目右座標
	DrawBoxAA(327, 485, 490, 545, LLb, TRUE);
	//・ポケモン3体目右座標
	DrawBoxAA(327, 550, 490, 610, LLb, TRUE);

	//・モンスターボール座標
	LoadGraphScreen(160, 410, "モンスターボール.png", TRUE);
	LoadGraphScreen(160, 475, "モンスターボール.png", TRUE);
	LoadGraphScreen(160, 540, "モンスターボール.png", TRUE);
	LoadGraphScreen(327, 420, "モンスターボール.png", TRUE);
	LoadGraphScreen(327, 485, "モンスターボール.png", TRUE);
	LoadGraphScreen(327, 550, "モンスターボール.png", TRUE);
	//ポケモン名前
	DrawString(205, 415, mypokemon[0].pokemon.name, Wh);
	DrawString(205, 480, mypokemon[2].pokemon.name, Wh);
	DrawString(205, 545, mypokemon[4].pokemon.name, Wh);
	DrawString(372, 425, mypokemon[1].pokemon.name, Wh);
	DrawString(372, 490, mypokemon[3].pokemon.name, Wh);
	DrawString(372, 555, mypokemon[5].pokemon.name, Wh);
	//ポケモンHP
	DrawFormatString(220, 445, Wh, "%d / %d", mypokemon[0].pokemon.HP, mypokemon[0].pokemon.MAXHP);
	DrawFormatString(220, 510, Wh, "%d / %d", mypokemon[2].pokemon.HP, mypokemon[2].pokemon.MAXHP);
	DrawFormatString(220, 575, Wh, "%d / %d", mypokemon[4].pokemon.HP, mypokemon[4].pokemon.MAXHP);
	DrawFormatString(387, 455, Wh, "%d / %d", mypokemon[1].pokemon.HP, mypokemon[1].pokemon.MAXHP);
	DrawFormatString(387, 520, Wh, "%d / %d", mypokemon[3].pokemon.HP, mypokemon[3].pokemon.MAXHP);
	DrawFormatString(387, 585, Wh, "%d / %d", mypokemon[5].pokemon.HP, mypokemon[5].pokemon.MAXHP);



	//・ポケモンコメ座標
	DrawBoxAA(160, 620, 405, 650, Pi, TRUE);
	DrawBoxAA(160, 620, 405, 650, Bk, FALSE);
	DrawBoxAA(165, 625, 380, 650, Wh, TRUE);
	//・もどる座標
	DrawBoxAA(410, 620, 490, 650, Lb, TRUE);
	DrawBoxAA(410, 620, 490, 650, Bk, FALSE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//下画面内枠

	SetFontSize(18);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(175, 628, " ポケモンを　えらんでください ", Bk);
	DrawString(428, 626, " もどる ", Wh);

}

int Select1(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//カーソル位置(1)
	int cursor = 1;

	while (1) {

		// マウス判定
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//もし、マウスのボタンが押されているかつ、押されているボタンが左だったら
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0, cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//もし、その座標がAボタンの位置だったら
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					if (cursor == 1) {
						flg = 2;
					}

					else if (cursor == 2) {
						flg = 3;
					}

					cnt = 1;
				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					cnt = 1;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
				}

			}

			//もし十字の上だったら
			if (MouseX >= (63) && MouseX <= (87) && MouseY >= (460) && MouseY <= (488)) {
				cursor = 1;
			}

			//もし十字の下だったら
			else if (MouseX >= (63) && MouseX <= (87) && MouseY >= (512) && MouseY <= (540)) {
				cursor = 2;
			}

			//もし十字の左だったら
			else if (MouseX >= (35) && MouseX <= (63) && MouseY >= (488) && MouseY <= (512)) {

			}

			//もし十字の右だったら
			else if (MouseX >= (87) && MouseX <= (115) && MouseY >= (488) && MouseY <= (512)) {

			}

			//もし、その座標がたたかうだったら
			else if (MouseX >= 180 && MouseX <= 470 && MouseY >= 450 && MouseY <= 580) {
				flg = 2;
				cnt = 1;
			}

			//もし、その座標がポケモンだったら
			else if (MouseX >= 160 && MouseX <= 490 && MouseY >= 600 && MouseY <= 650) {
				flg = 3;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//カーソル移動
		if (cursor == 1) {
			Screen1();
			DrawBoxAA(180, 450, 470, 580, Yl, FALSE);
		}

		else if (cursor == 2) {
			Screen1();
			DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;
		Sleep(100);

	}

	return flg;

}

int Select2(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//カーソル位置(1)
	int cursor = 1;

	while (1) {

		// マウス判定
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//もし、マウスのボタンが押されているかつ、押されているボタンが左だったら
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0, cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//もし、その座標がAボタンの位置だったら
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {

					flg = 1;

					if (cursor == 1) {

						TurnCheck(flg, mypokemon, enemypokemon);
					}

					else if (cursor == 2) {

						TurnCheck(flg, mypokemon, enemypokemon);

					}

					else if (cursor == 3) {

						TurnCheck(flg, mypokemon, enemypokemon);

					}

					else if (cursor == 4) {

						TurnCheck(flg, mypokemon, enemypokemon);

					}

					cnt = 1;
				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
				}

			}

			//もし十字の上だったら
			if (MouseX >= (63) && MouseX <= (87) && MouseY >= (460) && MouseY <= (488)) {
				if (cursor == 3) {
					cursor = 1;
				}
				else if (cursor == 4) {
					cursor = 2;
				}
				else if (cursor == 5) {
					cursor = 4;
				}
			}

			//もし十字の下だったら
			else if (MouseX >= (63) && MouseX <= (87) && MouseY >= (512) && MouseY <= (540)) {
				if (cursor == 1) {
					cursor = 3;
				}
				else if (cursor == 2) {
					cursor = 4;
				}
				else if (cursor == 3) {
					cursor = 5;
				}
				else if (cursor == 4) {
					cursor = 5;
				}
			}

			//もし十字の左だったら
			else if (MouseX >= (35) && MouseX <= (63) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 2) {
					cursor = 1;
				}
				else if (cursor == 4) {
					cursor = 3;
				}
			}

			//もし十字の右だったら
			else if (MouseX >= (87) && MouseX <= (115) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 1) {
					cursor = 2;
				}
				else if (cursor == 3) {
					cursor = 4;
				}
			}

			//もし、その座標が技1だったら
			else if (MouseX >= 165 && MouseX <= 320 && MouseY >= 435 && MouseY <= 510) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標が技2だったら
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 435 && MouseY <= 510) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標が技3だったら
			else if (MouseX >= 165 && MouseX <= 520 && MouseY >= 520 && MouseY <= 595) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標が技4だったら
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 520 && MouseY <= 595) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がもどるだったら
			else if (MouseX >= 160 && MouseX <= 490 && MouseY >= 600 && MouseY <= 650 ){
				flg = 1;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//カーソル移動
		if (cursor == 1) {
			Screen2();
			DrawBoxAA(165, 435, 320, 510, Yl, FALSE);
		}

		else if (cursor == 2) {
			Screen2();
			DrawBoxAA(330, 435, 485, 510, Yl, FALSE);
		}

		else if (cursor == 3) {
			Screen2();
			DrawBoxAA(165, 520, 320, 595, Yl, FALSE);
		}

		else if (cursor == 4) {
			Screen2();
			DrawBoxAA(330, 520, 485, 595, Yl, FALSE);
		}

		else if (cursor == 5) {
			Screen2();
			DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;
		Sleep(100);

	}

	return flg;

}

int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//カーソル位置(1)
	int cursor = 1;

	while (1) {

		// マウス判定
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//もし、マウスのボタンが押されているかつ、押されているボタンが左だったら
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0, cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//もし、その座標がAボタンの位置だったら
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {

					flg = 1;

					if (cursor == 1) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 2) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 3) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 4) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 5) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 6) {
						PokemonChange(mypokemon, enemypokemon);
					}

					else if (cursor == 7) {
						flg = 1;
					}

					cnt = 1;
				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
				}
			}

			//もし十字の上だったら
			if (MouseX >= (63) && MouseX <= (87) && MouseY >= (460) && MouseY <= (488)) {
				if (cursor == 3) {
					cursor = 1;
				}

				else if (cursor == 4) {
					cursor = 2;
				}

				else if (cursor == 5) {
					cursor = 3;
				}

				else if (cursor == 6) {
					cursor = 4;
				}

				else if (cursor == 7) {
					cursor = 6;
				}
			}

			//もし十字の下だったら
			else if (MouseX >= (63) && MouseX <= (87) && MouseY >= (512) && MouseY <= (540)) {
				if (cursor == 1) {
					cursor = 3;
				}

				else if (cursor == 2) {
					cursor = 4;
				}

				else if (cursor == 3) {
					cursor = 5;
				}

				else if (cursor == 4) {
					cursor = 6;
				}

				else if (cursor == 5) {
					cursor = 7;
				}

				else if (cursor == 6) {
					cursor = 7;
				}
			}

			//もし十字の左だったら
			else if (MouseX >= (35) && MouseX <= (63) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 2) {
					cursor = 1;
				}

				else if (cursor == 4) {
					cursor = 3;
				}

				else if (cursor == 6) {
					cursor = 5;
				}
			}

			//もし十字の右だったら
			else if (MouseX >= (87) && MouseX <= (115) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 1) {
					cursor = 2;
				}

				else if (cursor == 3) {
					cursor = 4;
				}

				else if (cursor == 5) {
					cursor = 6;
				}
			}

			//もし、その座標がポケモン1だったら
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 410 && MouseY <= 470) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がポケモン2だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 420 && MouseY <= 480) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がポケモン3だったら
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 475 && MouseY <= 535) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がポケモン4だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 485 && MouseY <= 545) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がポケモン5だったら
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 540 && MouseY <= 600) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がポケモン6だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 550 && MouseY <= 610) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//もし、その座標がもどるだったら
			else if (MouseX >= 410 && MouseX <= 490 && MouseY >= 620 && MouseY <= 650) {
				flg = 1;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//カーソル移動
		if (cursor == 1) {
			Screen3(mypokemon);
			DrawBoxAA(160, 410, 323, 470, Yl, FALSE);
		}

		else if (cursor == 2) {
			Screen3(mypokemon);
			DrawBoxAA(327, 420, 490, 480, Yl, FALSE);
		}

		else if (cursor == 3) {
			Screen3(mypokemon);
			DrawBoxAA(160, 475, 323, 535, Yl, FALSE);
		}

		else if (cursor == 4) {
			Screen3(mypokemon);
			DrawBoxAA(327, 485, 490, 545, Yl, FALSE);
		}

		else if (cursor == 5) {
			Screen3(mypokemon);
			DrawBoxAA(160, 540, 323, 600, Yl, FALSE);
		}

		else if (cursor == 6) {
			Screen3(mypokemon);
			DrawBoxAA(327, 550, 490, 610, Yl, FALSE);
		}

		else if (cursor == 7) {
			Screen3(mypokemon);
			DrawBoxAA(410, 620, 490, 650, Yl, FALSE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;
		Sleep(100);

	}

	return flg;

}

void TurnCheck(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	srand(time(NULL));
	int MySPD = ((rand() % 700 * 0.001) + 0.7) * (mypokemon[0].pokemon.SPD);
	int EnemySPD = ((rand() % 700 * 0.001) + 0.7) * (enemypokemon[0].pokemon.SPD);

	if (MySPD >= EnemySPD) {
		Myturn(flg, mypokemon, enemypokemon);
		Enemyturn(flg, mypokemon, enemypokemon);
	}

	else if (EnemySPD > MySPD) {
		Enemyturn(flg, mypokemon, enemypokemon);
		Myturn(flg, mypokemon, enemypokemon);
	}

}

void PokemonChange(MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

}

void Myturn(int flg, MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {
	enemypokemon[0].pokemon.HP -= mypokemon[0].pokemon.ATK - enemypokemon[0].pokemon.DEF;
}

void Enemyturn(int flg, MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {
	mypokemon[0].pokemon.HP -= enemypokemon[0].pokemon.ATK - mypokemon[0].pokemon.DEF;
}