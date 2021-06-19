//ヘッダーファイル--------------------------------------------------------------------------------------------------------------------
#include "DxLib.h"
#include <math.h>
#include <time.h>

//色指定(共通）-----------------------------------------------------------------------------------------------------------------------
#define Wh GetColor(255, 255, 255) //白
#define Bk GetColor(0, 0, 0)       //黒9
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
#define Rd GetColor(255, 0, 0)	   //赤色
#define Normal GetColor(192, 192, 192) //ノーマル
#define Fire GetColor(255, 119, 51) //火
#define Water GetColor(0, 170, 255) //水
#define Grass GetColor(41, 204, 41) //草
#define Light GetColor(255, 255, 0) //光
#define Dark GetColor(85, 61, 204) //闇

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

//技の数
#define MoveNum 100
//ポケモンの数
#define PokeNum 10
//所持ポケモン
#define MAXPokemon 6

//カーソル（座標）
int MouseX, MouseY, MouseInput, LogType;

//技の選択
int MoveNumber = 1;

//場のポケモン
int MyPokemonNumber = 0;
int EnemyPokemonNumber = 0;

//属性
enum Element{ノーマル,火,水,草,光,闇};

//技の名前、効果
typedef struct {
	char name[100];
	int Type;
	int MoveCount;
	int MAXMoveCount;
	int Hitrate;
	float ATKscale;
	int ATKcount;
	int other;
}Move;

//ポケモン構造体宣言
typedef struct {
	char name[20];
	int ELE;
	int MAXHP;
	int HP;
	int ATK;
	int DEF;
	int SPD;
	Move MV1;
	Move MV2;
	Move MV3;
	Move MV4;
}Pokemon;

typedef struct {
	Pokemon pokemon;
}MyPokemon;

typedef struct {
	Pokemon pokemon;
}EnemyPokemon;

//トレーナーの名前
typedef struct {
	char name[20];
}Trainer;


//プロトタイプ宣言--------------------------------------------------------------------------------------------------------------------
void ScreenReset();
void MoveCreate(Move* Move_Machine);
void PokemonCreate(Pokemon *pokemon, Move* Move_Machine);
void EnemyTextHpgauge(EnemyPokemon* enemypokemon);
void MyTextHpgauge(MyPokemon* mypokemon);
int Select1(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select2(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Screen1();
void Screen2(MyPokemon* mypokemon);
void Screen3(MyPokemon* mypokemon);
void TurnCheck(MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void PokemonChange(int MyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Myturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon);
void Enemyturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon);
void Abutton();
int CalcCenterX(int x1, int x2, const char* str);
void MySelectMove(EnemyPokemon* enemypokemon, MyPokemon* mypokemon, Move* MyMove);
void EnemySelectMove(MyPokemon* enemypokemon, EnemyPokemon* mypokemon, Move* MyMove);
unsigned int  ColorChange(int ColorNumber);

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

	//技生成
	Move Move_Machine[MoveNum];
	MoveCreate(Move_Machine);

	//ポケモン生成
	Pokemon pokemon[PokeNum];
	PokemonCreate(pokemon,Move_Machine);

	MyPokemon mypokemon[MAXPokemon] = { pokemon[7],pokemon[0],pokemon[4],pokemon[5],pokemon[6],pokemon[1] };
	EnemyPokemon enemypokemon[MAXPokemon] = { pokemon[4],pokemon[3],pokemon[9],pokemon[9],pokemon[9],pokemon[9] };

	//トレーナーの名前生成
	Trainer TrainerName[2] = { "野獣先輩","せいじ" };
	SetFontSize(22);
	DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "ポケモントレーナーの %sが\nしょうぶを しかけてきた！", TrainerName[0]);
	Abutton();
	SetFontSize(22);
	DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
	DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "ポケモントレーナーの %sは\n%sを くりだした！", TrainerName[0], enemypokemon[EnemyPokemonNumber].pokemon.name);
	EnemyTextHpgauge(enemypokemon);
	Abutton();
	SetFontSize(22);
	DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
	DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "ゆけっ！ %s！", mypokemon[MyPokemonNumber].pokemon.name);
	MyTextHpgauge(mypokemon);
	Abutton();
	SetFontSize(22);

	//画面選択(初期画面(1))
	int flg = 1;

	//戦闘
	while(1) {
		
		SetFontSize(22);
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);

		//flg=0の時、戦闘終了
		if (flg == 0 || enemypokemon[EnemyPokemonNumber].pokemon.HP <= 0) {
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
			Screen2(mypokemon);
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
	LoadGraphScreen(UP_x1 + 30, UP_y1 + 100, "ゆきぷりん.png",TRUE);
	LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, "オラポン.png", TRUE);
	LoadGraphScreen(400,150, "show.png", TRUE);
	DrawBoxAA(UP_x1, UP_y1+190, UP_x2, UP_y2, Bk, TRUE);	//テキスト部
	DrawBoxAA(UP_x1+5, UP_y1 + 190+5, UP_x2-5, UP_y2-5, Wh, TRUE);	//テキスト部
	DrawBoxAA(UP_x1, UP_y1, UP_x2, UP_y2, Bk, FALSE);	//上画面内枠

	//スピーカー
	float i, j, x, y;
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

void MoveCreate(Move* Move_Machine) {
	Move_Machine[0] = {"-",ノーマル,0,0,0,0,0,0 };
	Move_Machine[1] = {"へんしん",ノーマル,1,1,100,0,1,1};	//技を４つ入れ替え
	Move_Machine[2] = {"ちょうはつ",ノーマル,15,15,80,0,1,2};	//相手の攻撃1.2倍、防御下がる1.2倍
	Move_Machine[3] = {"キック",ノーマル,20,20,90,1.1,1,0};
	Move_Machine[4] = {"ビルドアップ",ノーマル,10,10,60,0,1,3};	//味方の攻撃1.5倍
	Move_Machine[5] = {"つるぎのまい",草,10,10,90,1,1,0};
	Move_Machine[6] = {"ソーラービーム",草,15,15,70,2,1,0};
	Move_Machine[7] = {"はっぱカッター",草,25,25,95,1.2,1,0};
	Move_Machine[8] = {"からにこもる",水,30,30,100,0,1,4};	//味方の防御1.2倍
	Move_Machine[9] = {"あなをほる",ノーマル,25,25,80,1.1,1,0};
	Move_Machine[10] = {"スペシウム",光};
	Move_Machine[11] = {"スクライド",ノーマル};
	Move_Machine[12] = {"ヘブンミックス",ノーマル};
	Move_Machine[13] = {"エンブレイス",光};
	Move_Machine[14] = {"ブレス",火,25,25,90,1.1,1,0};
	Move_Machine[15] = {"こうそくいどう",ノーマル,15,15,90,0,1,5};	//味方のスピード1.3倍
	Move_Machine[16] = {"ものまね",ノーマル,10,10,90,0,1,6};		//相手の攻撃パクリ
	Move_Machine[17] = {"アイアンクロー",闇,15,15,70,1.3,1,0};
	Move_Machine[18] = {"メロメロ",ノーマル,10,10,70,0,1,7};		//相手をこんらん（仮）状態にする
	Move_Machine[19] = { "水風呂",水,30,30,10,0,1,4 };		//味方の防御1.2倍
	Move_Machine[20] = { "配達",ノーマル,99,99,10,0.2,1,0};	
	Move_Machine[21] = { "住所特定",ノーマル,1,1,70,0,1,7 };	//味方の的中率100%敵の防御0.8倍
	Move_Machine[22] = { "おしゃべり",ノーマル,50,50,50,0,1,8 };		//敵の防御力0.5倍
	Move_Machine[23] = { "はねる",ノーマル,99,99,10,3.5,1,0};
}

void PokemonCreate(Pokemon* pokemon, Move* Move_Machine) {
	pokemon[0] = { "メタゴン",ノーマル,500,500,200,30,500,Move_Machine[1],Move_Machine[0],Move_Machine[0],Move_Machine[0] };
	pokemon[1] = { "ゴールドシップ",光,800,800,180,20,1000,Move_Machine[2],Move_Machine[3],Move_Machine[4],Move_Machine[5] };
	pokemon[2] = { "かぼやん",草,800,800,95,90,350,Move_Machine[6],Move_Machine[7],Move_Machine[8],Move_Machine[9] };
	pokemon[3] = { "ビリオン", 光,1,1,1,1,1,Move_Machine[10],Move_Machine[11],Move_Machine[12],Move_Machine[13] };
	pokemon[4] = { "オラポン", 火,600,600,135,30,600,Move_Machine[3],Move_Machine[14],Move_Machine[15],Move_Machine[16] };
	pokemon[5] = { "クロネコ", 闇,440,440,170,50,770,Move_Machine[17],Move_Machine[2],Move_Machine[15],Move_Machine[18] };
	pokemon[6] = { "せいじ",水,1000,1000,1,1,900,Move_Machine[19],Move_Machine[20] ,Move_Machine[21] ,Move_Machine[22] };
	pokemon[7] = { "ゆきぷりん",光,750,750,140,40,600, Move_Machine[23],Move_Machine[0],Move_Machine[0],Move_Machine[0] };
	pokemon[9] = { "-" };
}

//画面1描画処理--------------------------------------------------------------------------------------------------------
void Screen1() {

	//下画面内枠
	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//下画面外枠
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
void Screen2(MyPokemon* mypokemon) {

	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//下画面外枠
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh,TRUE);	//下画面内枠
	
	//・上のほうの背景
	DrawBoxAA(160, 400, 490, 412.5, Ld, TRUE);

	//x1,x2,str
	int x,x1, x2;
	char* str;

	unsigned int Color;

	//・技左上座標
	x1 = 170;
	x2 = 315;
	str = mypokemon[MyPokemonNumber].pokemon.MV1.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV1.Type);
	DrawBoxAA(165, 435, 320, 510, Color, TRUE);
	DrawBoxAA(165, 435, 320, 510, Bk, FALSE);
	DrawBoxAA(170, 440, 315, 505, Ld, TRUE);
	DrawFormatString(x, 445, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV1.name);
	DrawFormatString(205, 475, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV1.MoveCount,mypokemon[MyPokemonNumber].pokemon.MV1.MAXMoveCount);
	
	//・技右上座標
	x1 = 335;
	x2 = 480;
	str = mypokemon[MyPokemonNumber].pokemon.MV2.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV2.Type);
	DrawBoxAA(330, 435, 485, 510, Color, TRUE);
	DrawBoxAA(330, 435, 485, 510, Bk, FALSE);
	DrawBoxAA(335, 440, 480, 505, Ld, TRUE);
	DrawFormatString(x, 445, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV2.name);
	DrawFormatString(370, 475, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV2.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV2.MAXMoveCount);
	
	//・技左下座標
	x1 = 170;
	x2 = 315;
	str = mypokemon[MyPokemonNumber].pokemon.MV3.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV3.Type);
	DrawBoxAA(165, 520, 320, 595, Color, TRUE);
	DrawBoxAA(165, 520, 320, 595, Bk, FALSE);
	DrawBoxAA(170, 525, 315, 590, Ld, TRUE);
	DrawFormatString(x, 530, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV3.name);
	DrawFormatString(205, 560, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV3.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV3.MAXMoveCount);
	
	//・技右下座標
	x1 = 335;
	x2 = 480;
	str = mypokemon[MyPokemonNumber].pokemon.MV4.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV4.Type);
	DrawBoxAA(330, 520, 485, 595, Color, TRUE);
	DrawBoxAA(330, 520, 485, 595, Bk, FALSE);
	DrawBoxAA(335, 525, 480, 590, Ld, TRUE);
	DrawFormatString(x, 530, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV4.name);
	DrawFormatString(370, 560, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV4.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV4.MAXMoveCount);
	
	//・もどる座標
	DrawBoxAA(160, 600, 490, 650, Lb, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Db, TRUE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//下画面内枠

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 610, " もどる ", Wh);

}

//画面3描画処理--------------------------------------------------------------------------------------------------------
void Screen3(MyPokemon* mypokemon) {

	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//下画面外枠
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
	LoadGraphScreen(168, 420, "ゆきぷりん_小.png", TRUE);
	LoadGraphScreen(168, 485, "オラポン_小.png", TRUE);
	LoadGraphScreen(168, 550, "せいじ_小.png", TRUE);
	LoadGraphScreen(335, 435, "メタゴン_小.png", TRUE);
	LoadGraphScreen(335, 495, "クロネコ_小.png", TRUE);
	LoadGraphScreen(335, 560, "ゴールドシップ_小.png", TRUE);
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
					break;
				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					cnt = 1;
					break;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
					break;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
					break;
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
			//DrawBoxAA(180, 450, 470, 580, Yl, FALSE);
			LoadGraphScreen(180 - 18, 450 - 18, "カーソル左上.png", TRUE);
			LoadGraphScreen(470 - 2, 450 - 18, "カーソル右上.png", TRUE);
			LoadGraphScreen(180 - 18, 580 - 2, "カーソル左下.png", TRUE);
			LoadGraphScreen(470 - 2, 580 - 2, "カーソル右下.png", TRUE);
		}

		else if (cursor == 2) {
			Screen1();
			//DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
			LoadGraphScreen(160 , 600 , "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 20 , 600 , "カーソル右上.png", TRUE);
			LoadGraphScreen(160 , 650 - 20 , "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 20 , 650 - 20 , "カーソル右下.png", TRUE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

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

			double deg = 0, len = 18, rad = 0, x = 0, y = 0;
			int cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//もし、その座標がAボタンの位置だったら
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {

					if (cursor == 1) {
						if (mypokemon[MyPokemonNumber].pokemon.MV1.MoveCount != 0) {
							flg = 1;
							cnt = 1;
							MoveNumber = 1;
							TurnCheck(mypokemon, enemypokemon);
							break;
						}
						else {
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！",mypokemon[MyPokemonNumber].pokemon.name ,mypokemon[MyPokemonNumber].pokemon.MV1.name);
							Abutton();
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
						}
					}

					else if (cursor == 2) {
						if (mypokemon[MyPokemonNumber].pokemon.MV2.MoveCount != 0) {
							flg = 1;
							cnt = 1;
							MoveNumber = 2;
							TurnCheck(mypokemon, enemypokemon);
							break;
						}
						else {
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
							Abutton();
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
						}
					}

					else if (cursor == 3) {
						if (mypokemon[MyPokemonNumber].pokemon.MV3.MoveCount != 0) {
							flg = 1;
							cnt = 1;
							MoveNumber = 3;
							TurnCheck(mypokemon, enemypokemon);
							break;
						}
						else {
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
							Abutton();
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
						}
					}

					else if (cursor == 4) {
						if (mypokemon[MyPokemonNumber].pokemon.MV4.MoveCount != 0) {
							flg = 1;
							cnt = 1;
							MoveNumber = 4;
							TurnCheck(mypokemon, enemypokemon);
							break;
						}
						else {
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
							Abutton();
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
						}
					}

					else if (cursor == 5) {
						flg = 1;
						cnt = 1;
					}

				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
					break;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
					break;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
					break;
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
				if (mypokemon[MyPokemonNumber].pokemon.MV1.MoveCount != 0) {
					MoveNumber = 1;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV1.name);
					Abutton();
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
				}
			}

			//もし、その座標が技2だったら
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 435 && MouseY <= 510) {
				if (mypokemon[MyPokemonNumber].pokemon.MV2.MoveCount != 0) {
					MoveNumber = 2;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
					Abutton();
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
				}

			}

			//もし、その座標が技3だったら
			else if (MouseX >= 165 && MouseX <= 320 && MouseY >= 520 && MouseY <= 595) {
				if (mypokemon[MyPokemonNumber].pokemon.MV3.MoveCount != 0) {
					MoveNumber = 3;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;	
					flg = 1;
				}
				else {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
				
				}
			}

			//もし、その座標が技4だったら
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 520 && MouseY <= 595) {
				if (mypokemon[MyPokemonNumber].pokemon.MV4.MoveCount != 0) {
					MoveNumber = 4;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sは　ポイントぎれで\n%sが　くりだせない！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
					Abutton();
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sはどうする？", mypokemon[MyPokemonNumber].pokemon.name);
				}
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
			Screen2(mypokemon);
			//DrawBoxAA(165, 435, 320, 510, Yl, FALSE);
			LoadGraphScreen(165 - 5 , 435 - 5 , "カーソル左上.png", TRUE);
			LoadGraphScreen(320 - 15, 435 - 5, "カーソル右上.png", TRUE);
			LoadGraphScreen(165 - 5 , 510 - 15, "カーソル左下.png", TRUE);
			LoadGraphScreen(320 - 15, 510 - 15, "カーソル右下.png", TRUE);
		}

		else if (cursor == 2) {
			Screen2(mypokemon);
			//DrawBoxAA(330, 435, 485, 510, Yl, FALSE);
			LoadGraphScreen(330 - 5, 435 - 5, "カーソル左上.png", TRUE);
			LoadGraphScreen(485 - 15, 435 - 5, "カーソル右上.png", TRUE);
			LoadGraphScreen(330 - 5, 510 - 15, "カーソル左下.png", TRUE);
			LoadGraphScreen(485 - 15, 510 - 15, "カーソル右下.png", TRUE);
		}

		else if (cursor == 3) {
			Screen2(mypokemon);
			//DrawBoxAA(165, 520, 320, 595, Yl, FALSE);
			LoadGraphScreen(165 - 5, 520 - 5, "カーソル左上.png", TRUE);
			LoadGraphScreen(320 - 15, 520 - 5, "カーソル右上.png", TRUE);
			LoadGraphScreen(165 - 5, 595 - 15, "カーソル左下.png", TRUE);
			LoadGraphScreen(320 - 15, 595 - 15, "カーソル右下.png", TRUE);
		}

		else if (cursor == 4) {
			Screen2(mypokemon);
			//DrawBoxAA(330, 520, 485, 595, Yl, FALSE);
			LoadGraphScreen(330 - 5, 520 - 5, "カーソル左上.png", TRUE);
			LoadGraphScreen(485 - 15, 520 - 5, "カーソル右上.png", TRUE);
			LoadGraphScreen(330 - 5, 595 - 15, "カーソル左下.png", TRUE);
			LoadGraphScreen(485 - 15, 595 - 15, "カーソル右下.png", TRUE);
		}

		else if (cursor == 5) {
			Screen2(mypokemon);
			//DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
			LoadGraphScreen(160, 600, "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 20, 600, "カーソル右上.png", TRUE);
			LoadGraphScreen(160, 650 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 20, 650 - 20, "カーソル右下.png", TRUE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

	}

	return flg;

}

int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;
	
	//交換するときわたす引数
	int MyPokemonChangeNumber = 0;

	//カーソル位置(1)
	int cursor = 1;

	while (1) {

		// マウス判定
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//もし、マウスのボタンが押されているかつ、押されているボタンが左だったら
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0;
			int cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//もし、その座標がAボタンの位置だったら
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {

					flg = 1;

					if (cursor == 1) {
						MyPokemonChangeNumber = 0;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber,mypokemon, enemypokemon);
						}
					}

					else if (cursor == 2) {
						MyPokemonChangeNumber = 1;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
						}
					}

					else if (cursor == 3) {
						MyPokemonChangeNumber = 2;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
						}
					}

					else if (cursor == 4) {
						MyPokemonChangeNumber = 3;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
						}
					}

					else if (cursor == 5) {
						MyPokemonChangeNumber = 4;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
						}
					}

					else if (cursor == 6) {
						MyPokemonChangeNumber = 5;
						if (MyPokemonNumber != MyPokemonChangeNumber) {
							PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
						}
					}

					else if (cursor == 7) {
						flg = 1;
					}

					cnt = 1;
					break;
				}

				//もし、その座標がBボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
					break;
				}

				//もし、その座標がXボタンの位置だったら
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
					break;
				}

				//もし、その座標がYボタンの位置だったら
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
					break;
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
				MyPokemonChangeNumber = 0;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
			}

			//もし、その座標がポケモン2だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 420 && MouseY <= 480) {
				MyPokemonChangeNumber = 1;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
			}

			//もし、その座標がポケモン3だったら
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 475 && MouseY <= 535) {
				MyPokemonChangeNumber = 2;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
			}

			//もし、その座標がポケモン4だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 485 && MouseY <= 545) {
				MyPokemonChangeNumber = 3;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
			}

			//もし、その座標がポケモン5だったら
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 540 && MouseY <= 600) {
				MyPokemonChangeNumber =	4;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
			}

			//もし、その座標がポケモン6だったら
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 550 && MouseY <= 610) {
				MyPokemonChangeNumber = 5;
				if (MyPokemonNumber != MyPokemonChangeNumber) {
					PokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
				}
				cnt = 1;
				flg = 1;
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
			//DrawBoxAA(160, 410, 323, 470, Yl, FALSE);
			LoadGraphScreen(160 , 410 , "カーソル左上.png", TRUE);
			LoadGraphScreen(323 - 20, 410 , "カーソル右上.png", TRUE);
			LoadGraphScreen(160 , 470 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(323 - 20, 470 - 20, "カーソル右下.png", TRUE);

		}

		else if (cursor == 2) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 420, 490, 480, Yl, FALSE);
			LoadGraphScreen(327, 420, "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 20, 420, "カーソル右上.png", TRUE);
			LoadGraphScreen(327, 480 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 20, 480 - 20, "カーソル右下.png", TRUE);
		}

		else if (cursor == 3) {
			Screen3(mypokemon);
			//DrawBoxAA(160, 475, 323, 535, Yl, FALSE);
			LoadGraphScreen(160, 475, "カーソル左上.png", TRUE);
			LoadGraphScreen(323 - 20, 475, "カーソル右上.png", TRUE);
			LoadGraphScreen(160, 535 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(323 - 20, 535 - 20, "カーソル右下.png", TRUE);
		}

		else if (cursor == 4) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 485, 490, 545, Yl, FALSE);
			LoadGraphScreen(327, 485, "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 20, 485, "カーソル右上.png", TRUE);
			LoadGraphScreen(327, 545 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 20, 545 - 20, "カーソル右下.png", TRUE);
		}

		else if (cursor == 5) {
			Screen3(mypokemon);
			//DrawBoxAA(160, 540, 323, 600, Yl, FALSE);
			LoadGraphScreen(160, 540, "カーソル左上.png", TRUE);
			LoadGraphScreen(323 - 20, 540, "カーソル右上.png", TRUE);
			LoadGraphScreen(160, 600 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(323 - 20, 600 - 20, "カーソル右下.png", TRUE);
		}

		else if (cursor == 6) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 550, 490, 610, Yl, FALSE);
			LoadGraphScreen(327, 550, "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 20, 550, "カーソル右上.png", TRUE);
			LoadGraphScreen(327, 610 - 20, "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 20, 610 - 20, "カーソル右下.png", TRUE);
		}

		else if (cursor == 7) {
			Screen3(mypokemon);
			//DrawBoxAA(410, 620, 490, 650, Yl, FALSE);
			LoadGraphScreen(410 - 5, 620 - 5, "カーソル左上.png", TRUE);
			LoadGraphScreen(490 - 15, 620 - 5, "カーソル右上.png", TRUE);
			LoadGraphScreen(410 - 5, 650 - 15, "カーソル左下.png", TRUE);
			LoadGraphScreen(490 - 15, 650 - 15, "カーソル右下.png", TRUE);
		}

		//初期化
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

	}

	return flg;

}

void TurnCheck(MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	srand((unsigned)time(NULL));
	float MySPD = ((rand() % 600 * 0.001) + 0.7) * (mypokemon[0].pokemon.SPD);
	float EnemySPD = ((rand() % 600 * 0.001) + 0.7) * (enemypokemon[EnemyPokemonNumber].pokemon.SPD);

	if (MySPD >= EnemySPD) {
		Myturn(mypokemon, enemypokemon);
		Enemyturn(mypokemon, enemypokemon);
	}

	else if (EnemySPD > MySPD) {
		Enemyturn(mypokemon, enemypokemon);
		Myturn(mypokemon, enemypokemon);
	}

}

void PokemonChange(int MyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {
	
	if (mypokemon[MyPokemonChangeNumber].pokemon.MAXHP != 0) {

		SetFontSize(22);
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s こうたい！\nもどれ！", mypokemon[MyPokemonNumber].pokemon.name);
		Abutton();

		MyPokemonNumber = MyPokemonChangeNumber;

		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "ゆけっ！ %s！", mypokemon[MyPokemonNumber].pokemon.name);
		MyTextHpgauge(mypokemon);
		Abutton();

		Enemyturn(mypokemon, enemypokemon);

	}

}

void Myturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {

	SetFontSize(22);
	
	if(MoveNumber == 1){
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV1.name); 
		Abutton();
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV1);
	}

	else if (MoveNumber == 2) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
		Abutton();
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV2);
	}

	else if (MoveNumber == 3) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
		Abutton(); 
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV3);
	}

	else if (MoveNumber == 4) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
		Abutton(); 
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV4);
	}

}

void Enemyturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {

	srand((unsigned)time(NULL));
	int EnemyMoveNumber;
	EnemyMoveNumber = rand() % 4 + 1;

	SetFontSize(22);

	if (EnemyMoveNumber == 1) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV1.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV1);
	}

	else if (EnemyMoveNumber == 2) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV2.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV2);
	}

	else if (EnemyMoveNumber == 3) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV3.name);
		Abutton();
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV3);
	}

	else if (EnemyMoveNumber == 4) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s の\n%s！", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV4.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV4);
	}

}

void Abutton() {

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	int	a = 0;

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
					a = 1;
				}
			}
		}

		if (a == 1) {
			break;
		}
	}

	//初期化
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

}

int CalcCenterX(int x1,int x2,const char *str)
{
    int StrLen,StrWidth;
    StrLen=(int)strlen(str);
    StrWidth= GetDrawStringWidth(str,StrLen);
    return (int)((x1+((x2-x1)/2))-(StrWidth/2));
}

void MySelectMove(EnemyPokemon* enemypokemon,MyPokemon* mypokemon,Move* MyMove) {

	int AtkNum = 0;
	float ElementEffect = 0;

	srand((unsigned)time(NULL));

	//命中率
	if (MyMove->Hitrate - ((rand() % 100) + 1) >= 0) {

		if ((MyMove->Type == 1 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 2) ||
			(MyMove->Type == 2 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 3) ||
			(MyMove->Type == 3 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 1) ||
			(MyMove->Type == 4 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 5) ||
			(MyMove->Type == 5 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 4)) {
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "こうかは　いまひとつのようだ...");
			ElementEffect = 0.7;
		}

		else if ((MyMove->Type == 2 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 1) ||
			(MyMove->Type == 3 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 2) ||
			(MyMove->Type == 1 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 3) ||
			(MyMove->Type == 5 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 4) ||
			(MyMove->Type == 4 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 5)) {
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "こうかは　ばつぐんだ！");
			ElementEffect = 1.3;
		}

		else {
			ElementEffect = 1.0;
		}
		
		//9%の確率できゅうしょに当たる
		if ((rand() % 100) >= 90) {
			Sleep(500);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "きゅうしょにあたった！");
			ElementEffect += 1.5;
		}

		//ポケモンの技
		switch (MyMove->other) {
			//通常攻撃の時
			case 0:
				AtkNum = ((mypokemon[MyPokemonNumber].pokemon.ATK * MyMove->ATKscale - enemypokemon[EnemyPokemonNumber].pokemon.DEF) * ElementEffect * ((rand() % 700 * 0.001) + 0.7));
		}

		//HPを減らす
		int HPCount = AtkNum;

		while (HPCount >= 1) {
			enemypokemon[EnemyPokemonNumber].pokemon.HP = enemypokemon[EnemyPokemonNumber].pokemon.HP - 1;
			EnemyTextHpgauge(enemypokemon);
			HPCount--;
		}
	}

	else {

		//当たらなかった。
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sには\n当たらなかった！", enemypokemon[EnemyPokemonNumber].pokemon.name);
	}

	Abutton();

	//技のカウントを減らす
	MyMove->MoveCount--;

	Sleep(500);

}

void EnemySelectMove(MyPokemon* enemypokemon, EnemyPokemon* mypokemon, Move* MyMove) {

	int AtkNum = 0;
	float ElementEffect = 0;

	srand((unsigned)time(NULL));

	//命中率
	if (MyMove->Hitrate - ((rand() % 100) + 1) >= 0) {

		if ((MyMove->Type == 1 && enemypokemon[MyPokemonNumber].pokemon.ELE == 2) ||
			(MyMove->Type == 2 && enemypokemon[MyPokemonNumber].pokemon.ELE == 3) ||
			(MyMove->Type == 3 && enemypokemon[MyPokemonNumber].pokemon.ELE == 1) ||
			(MyMove->Type == 4 && enemypokemon[MyPokemonNumber].pokemon.ELE == 5) ||
			(MyMove->Type == 5 && enemypokemon[MyPokemonNumber].pokemon.ELE == 4)) {
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "こうかは　いまひとつのようだ...");
			ElementEffect = 0.7;
		}

		else if ((MyMove->Type == 2 && enemypokemon[MyPokemonNumber].pokemon.ELE == 1) ||
			(MyMove->Type == 3 && enemypokemon[MyPokemonNumber].pokemon.ELE == 2) ||
			(MyMove->Type == 1 && enemypokemon[MyPokemonNumber].pokemon.ELE == 3) ||
			(MyMove->Type == 5 && enemypokemon[MyPokemonNumber].pokemon.ELE == 4) ||
			(MyMove->Type == 4 && enemypokemon[MyPokemonNumber].pokemon.ELE == 5)) {
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "こうかは　ばつぐんだ！");
			ElementEffect = 1.3;
		}

		else {
			ElementEffect = 1.0;
		}

		//9%の確率できゅうしょに当たる
		if ((rand() % 100) >= 90) {
			Sleep(500);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "きゅうしょにあたった！");
			ElementEffect += 1.5;
		}

		//ポケモンの技
		switch (MyMove->other) {
			//通常攻撃の時
			case 0:
				AtkNum = ((mypokemon[EnemyPokemonNumber].pokemon.ATK * MyMove->ATKscale - enemypokemon[MyPokemonNumber].pokemon.DEF) * ElementEffect * ((rand() % 700 * 0.001) + 0.7));
		}

		//HPを減らす
		int HPCount = AtkNum;

		while (HPCount >= 1) {
			enemypokemon[MyPokemonNumber].pokemon.HP = enemypokemon[MyPokemonNumber].pokemon.HP - 1;
			MyTextHpgauge(enemypokemon);
			HPCount--;
		}
	}

	else {

		//当たらなかった。
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//テキスト部
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%sには\n当たらなかった！", enemypokemon[MyPokemonNumber].pokemon.name);
	}

	Abutton();

	Sleep(500);

	//技のカウントを減らす
	MyMove->MoveCount--;
}

void EnemyTextHpgauge(EnemyPokemon* enemypokemon) {

	ScreenFlip();// 裏画面データを表画面へ反映
	SetDrawScreen(DX_SCREEN_BACK); // 描画先を裏画面に設定

	SetFontSize(20);
	SetFontThickness(9);
	DrawBoxAA(UP_x1, UP_y1 + 25, UP_x1 + 140, UP_y1 + 65, Wh, TRUE);
	DrawBoxAA(UP_x1, UP_y1 + 25, UP_x1 + 140, UP_y1 + 65, Bk, FALSE);
	DrawTriangle(UP_x1 + 140, UP_y1 + 45, UP_x1 + 140, UP_y1 + 55, UP_x1 + 155, UP_y1 + 50, Bk, TRUE);
	DrawFormatString(UP_x1+5, UP_y1 + 25, Bk, "%s", enemypokemon[EnemyPokemonNumber].pokemon.name);

	//HPバー
	int  a, b;
	float HPzahyo;
	a = enemypokemon[EnemyPokemonNumber].pokemon.HP;
	b = enemypokemon[EnemyPokemonNumber].pokemon.MAXHP;
	
	HPzahyo = ((float)a/(float)b) * 70.0;

	if (HPzahyo >= (70 * 0.5)) {
		DrawBoxAA(UP_x1 + 65, UP_y1 + 50, UP_x1 + 65 + HPzahyo, UP_y1 + 60, Gr, TRUE);
	}
	else if (HPzahyo >= (70 * 0.2)) {
		DrawBoxAA(UP_x1 + 65, UP_y1 + 50, UP_x1 + 65 + HPzahyo, UP_y1 + 60, Yl, TRUE);
	}
	else {
		DrawBoxAA(UP_x1 + 65, UP_y1 + 50, UP_x1 + 65 + HPzahyo, UP_y1 + 60, Rd, TRUE);
	}

	DrawBoxAA(UP_x1 + 65, UP_y1 + 50, UP_x1 + 135, UP_y1 + 60, Bk, FALSE);
	SetFontSize(7);
	SetFontThickness(9);
	DrawBoxAA(UP_x1 + 45, UP_y1 + 50, UP_x1 + 65, UP_y1 + 60, Bk, TRUE);
	DrawFormatString(UP_x1 + 50, UP_y1 + 51, Wh, "HP");

	ScreenFlip();// 裏画面データを表画面へ反映
	SetDrawScreen(DX_SCREEN_FRONT); // 描画先を裏画面に設定
}

void MyTextHpgauge(MyPokemon* mypokemon) {

	ScreenFlip();// 裏画面データを表画面へ反映
	SetDrawScreen(DX_SCREEN_BACK); // 描画先を裏画面に設定

	SetFontSize(20);
	SetFontThickness(9);
	DrawBoxAA(UP_x2 - 140, UP_y2 - 115, UP_x2, UP_y2 - 65, Wh, TRUE);
	DrawBoxAA(UP_x2 - 140, UP_y2 - 115, UP_x2, UP_y2 - 65, Bk, FALSE);
	DrawTriangle(UP_x2 - 140, UP_y2 - 85, UP_x2 - 140, UP_y2 - 75, UP_x2 - 155, UP_y2 - 80, Bk, TRUE);
	DrawFormatString(UP_x2 - 135, UP_y2 - 115, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.name);

	//HPバー
	int  a, b;
	float HPzahyo;
	a = mypokemon[MyPokemonNumber].pokemon.HP;
	b = mypokemon[MyPokemonNumber].pokemon.MAXHP;

	HPzahyo = ((float)a / (float)b) * 70.0;

	if (HPzahyo >= (70 * 0.5)) {
		DrawBoxAA(UP_x2 - 75, UP_y2 - 90, UP_x2 - 75 + HPzahyo, UP_y2 - 80, Gr, TRUE);
	}
	else if (HPzahyo >= (70 * 0.2)) {
		DrawBoxAA(UP_x2 - 75, UP_y2 - 90, UP_x2 - 75 + HPzahyo, UP_y2 - 80, Yl, TRUE);
	}
	else {
		DrawBoxAA(UP_x2 - 75, UP_y2 - 90, UP_x2 - 75 + HPzahyo, UP_y2 - 80, Rd, TRUE);
	}

	DrawBoxAA(UP_x2 - 75, UP_y2 - 90, UP_x2 -5, UP_y2 - 80, Bk, FALSE);
	SetFontSize(7);
	SetFontThickness(9);
	DrawBoxAA(UP_x2 - 95, UP_y2 - 90, UP_x2 - 75, UP_y2 - 80, Bk, TRUE);
	DrawFormatString(UP_x2 - 90, UP_y2 - 89, Wh, "HP");

	//HPの残り表示
	SetFontSize(14);
	DrawFormatString(UP_x2 - 70, UP_y2 - 80, Bk, "%4d / %4d", mypokemon[MyPokemonNumber].pokemon.HP, mypokemon[MyPokemonNumber].pokemon.MAXHP);

	ScreenFlip();// 裏画面データを表画面へ反映
	SetDrawScreen(DX_SCREEN_FRONT); // 描画先を裏画面に設定

}

unsigned int  ColorChange(int ColorNumber) {
	unsigned int Color;

	switch (ColorNumber) {
		case ノーマル:
			Color = Normal;
			break;

		case 火:
			Color = Fire;
			break;

		case 水:
			Color = Water;
			break;

		case 草:
			Color = Grass;
			break;

		case 光:
			Color = Light;
			break;

		case 闇:
			Color = Dark;
			break;

	}


	return Color;
}