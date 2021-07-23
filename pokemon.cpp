//�w�b�_�[�t�@�C��--------------------------------------------------------------------------------------------------------------------
#include "DxLib.h"
#include <math.h>
#include <time.h>

//�F�w��(���ʁj-----------------------------------------------------------------------------------------------------------------------
#define Wh GetColor(255, 255, 255) //��
#define Bk GetColor(0, 0, 0)       //��9
#define Gl GetColor(102, 102, 102) //�D�F
#define Bl GetColor(179, 255, 255) //��
#define LLb GetColor(25, 180, 255) //LimeLblue
#define Lb GetColor(30, 125, 200)  //Limeblue
#define Db GetColor(5, 60, 125)    //Darkblue
#define Gr GetColor(150, 230, 0)   //��
#define Lg GetColor(120, 200, 60)  //LimeGreen
#define Dg GetColor(0, 100, 0)     //DarkGreen
#define Lr GetColor(180, 30, 20)   //����ߐ�
#define Dr GetColor(135, 42, 42)   //�_�[�N���b�h
#define Ld GetColor(230, 230, 250) //���x���_�[
#define Pi GetColor(255, 153, 238) //�s���N
#define Ly GetColor(255, 255, 200) //���邢���F
#define Yl GetColor(255, 255, 0)   //���F	
#define Rd GetColor(255, 0, 0)	   //�ԐF
#define Normal GetColor(192, 192, 192) //�m�[�}��
#define Fire GetColor(255, 119, 51) //��
#define Water GetColor(0, 170, 255) //��
#define Grass GetColor(41, 204, 41) //��
#define Light GetColor(255, 255, 0) //��
#define Dark GetColor(85, 61, 204) //��
#define Orange GetColor(230, 120, 30) //�I�����W

//��ʑ傫���i���~�c�j330�~250 pixel�@�i���ʁj----------------------------------------------------------------------------------------
//����
#define UP_x1 160
#define UP_y1 40
#define UP_x2 490
#define UP_y2 290
#define UPCE_x 325
#define UPCE_y 165

//�����
#define DW_x1 160
#define DW_y1 400
#define DW_x2 490
#define DW_y2 650
#define DWCE_x 325
#define DWCE_y 525

//�Z�̐�
#define MoveNum 100
//�|�P�����̐�
#define PokeNum 10
//�����|�P����
#define MAXPokemon 6

//�J�[�\���i���W�j
int MouseX, MouseY, MouseInput, LogType;

//�Z�̑I��
int MoveNumber = 1;
int EnemyMoveNumber = 1;

//��̃|�P����
int MyPokemonNumber = 0;
int EnemyPokemonNumber = 0;

int MyPokemon_flg = 0;
int EnemyPokemon_flg = 0;

//�����|�P���������񂾂Ƃ��̃|�P�����������t���O
int mypokemon_die_flg = 0;

//�v���O�����I��
int end_flg = 0;


//����
enum Element{�m�[�}��,��,��,��,��,��};

//�Z�̖��O�A����
typedef struct {
	char name[100];
	int Type;
	int MoveCount;
	int MAXMoveCount;
	int Hitrate;
	float ATKscale;
	int MoveCategory;
}Move;

//�|�P�����\���̐錾
typedef struct {
	char name[20];
	char big_picture[256];
	char sml_picture[256];
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

//�g���[�i�[�̖��O
typedef struct {
	char name[20];
}Trainer;

//�g���[�i�[�̖��O
Trainer TrainerName[2] = { "���������","�v���C���[" };


//�v���g�^�C�v�錾--------------------------------------------------------------------------------------------------------------------
void ScreenReset(MyPokemon* mypokemon,EnemyPokemon* enemypokemon);
void UpInitScreen(MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void MoveCreate(Move* Move_Machine);
void PokemonCreate(Pokemon *pokemon, Move* Move_Machine);
void EnemyTextHpgauge(EnemyPokemon* enemypokemon);
void MyTextHpgauge(MyPokemon* mypokemon);
int Select1(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select2(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Screen1(MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Screen2(MyPokemon* mypokemon);
void Screen3(MyPokemon* mypokemon);
void TurnCheck(MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void MyPokemonChange(int MyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void EnemyPokemonChange(int EnemyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon);
void Myturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon);
void Enemyturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon);
void Abutton();
int CalcCenterX(int x1, int x2, const char* str);
void MySelectMove(EnemyPokemon* enemypokemon, MyPokemon* mypokemon, Move* MyMove);
void EnemySelectMove(MyPokemon* enemypokemon, EnemyPokemon* mypokemon, Move* MyMove);
unsigned int  ColorChange(int ColorNumber);

//��L���ʂ̂��ߕύX���Ȃ�------------------------------------------------------------------------------------------------------------


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	
	//������ʁi�ύX���Ȃ��j------------------------------------------------------------------
	SetOutApplicationLogValidFlag(FALSE); //���O�o�͖���
	SetMainWindowText("Nintendo DS Lite");
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�ŋN��
	SetGraphMode(650, 700, 32); //��ʂ̉𑜓x�w��@�@
	SetBackgroundColor(255, 255, 255);
	SetWindowSizeChangeEnableFlag(FALSE); //��ʃT�C�Y�ύX�s��
	

	// �c�w���C�u��������������---------------------------------------------------------------
	if (DxLib_Init() == -1)
	{
		return EXIT_FAILURE;    // �G���[���N�����璼���ɏI��
	}

	while (ProcessMessage() == 0 && end_flg == 0) {

		//[�~]�{�^���ŏ����Ȃ��悤�ɂ���i�^�X�N���c�邽�߁j
		SetWindowUserCloseEnableFlag(FALSE);

		//�N����----------------------------------------------------------------------------------
		PlaySoundFile("DS�N����.mp3", DX_PLAYTYPE_BACK);

		//�Z����
		Move Move_Machine[MoveNum];
		MoveCreate(Move_Machine);

		//�|�P��������
		Pokemon pokemon[PokeNum];
		PokemonCreate(pokemon, Move_Machine);

		MyPokemon mypokemon[MAXPokemon] = { pokemon[0],pokemon[1],pokemon[2],pokemon[3],pokemon[9],pokemon[9] };
		EnemyPokemon enemypokemon[MAXPokemon] = { pokemon[4],pokemon[5],pokemon[6],pokemon[7],pokemon[9],pokemon[9] };

		//��ʏ�����------------------------------------------------------------------------------
		ScreenReset(mypokemon, enemypokemon);

		SetFontSize(22);
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�|�P�����g���[�i�[�� %s��\n���傤�Ԃ� �������Ă����I", TrainerName[0]);
		Abutton();

		SetFontSize(22);
		LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�G�̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�|�P�����g���[�i�[�� %s��\n%s�� ���肾�����I", TrainerName[0], enemypokemon[EnemyPokemonNumber].pokemon.name);
		EnemyTextHpgauge(enemypokemon);
		Abutton();

		SetFontSize(22);
		LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�䂯���I %s�I", mypokemon[MyPokemonNumber].pokemon.name);
		MyTextHpgauge(mypokemon);
		Abutton();

		SetFontSize(22);

		//��ʑI��(�������(1))
		int flg = 1;

		//�퓬���[�v
		while (1) {
			
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);

			//�v���O�����I��
			if (flg == 0) {
				end_flg = 1;
				break;
			}

			//flg=1�̎��A���1�Ɉړ�
			else if (flg == 1) {
				Screen1(mypokemon, enemypokemon);
				flg = Select1(flg, mypokemon, enemypokemon);
			}

			//flg=2�̎��A���2�Ɉړ�
			else if (flg == 2) {
				Screen2(mypokemon);
				flg = Select2(flg, mypokemon, enemypokemon);
			}

			//flg=3�̎��A���3�Ɉړ�
			else if (flg == 3) {
				Screen3(mypokemon);
				flg = Select3(flg, mypokemon, enemypokemon);
			}

		}

	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
	return EXIT_SUCCESS; 
}

void ScreenReset(MyPokemon* mypokemon,EnemyPokemon* enemypokemon){

	//�O�g�`��--------------------------------------------------------------
	DrawBoxAA(0, 0, 650, 700, Bk, FALSE);

	//���ʕ`��------------------------------------------------------------
	DrawBoxAA(150, 30, 500, 300, Bk, FALSE);	//���ʊO�g
	LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
	UpInitScreen(mypokemon, enemypokemon);	//���ʃe�L�X�g��
	DrawBoxAA(UP_x1, UP_y1, UP_x2, UP_y2, Bk, FALSE);	//���ʓ��g

	//�X�s�[�J�[
	float i, j, x, y;
	for (i = 0; i <= 2; i++) {
		for (j = 0; j <= 1; j++) {
			x = i * 25;
			y = j * 25;
			DrawCircleAA(50 + x, 150 + y, 3, 32, Bk, TRUE);
			DrawCircleAA(600 - x, 150 + y, 3, 32, Bk, TRUE);
		}
	}

	//�^�񒆂̕`��---------------------------------------------------------
	DrawBoxAA(0, 330, 650, 370, Bk, FALSE);		//�^�񒆂̐�
	DrawBoxAA(75, 330, 575, 370, Bk, FALSE);	//�܂��ݐ��H
	DrawBoxAA(323, 342, 327, 358, Bk, TRUE);	//�}�C�N�^��
	DrawBoxAA(610, 335, 614, 365, Gl, TRUE);	//���C�g��
	DrawBoxAA(620, 335, 624, 365, Gr, TRUE);	//���C�g�E

	//����ʕ`��------------------------------------------------------------
	DrawBoxAA(150, 390, 500, 660, Bk, FALSE);	//����ʊO�g
	LoadGraphScreen(DW_x1, DW_y1, "picture/�U���������.jpg", FALSE);	//�w�i�摜
	DrawBoxAA(63, 460, 87, 540, Bk, FALSE);		//�\���c
	DrawBoxAA(35, 488, 115, 512, Bk, FALSE);	//�\����
	DrawBoxAA(63, 488, 87, 512, Wh, FALSE);		//�\���^��

	DrawBoxAA(74, 470, 76, 488, Gl, TRUE);		//�\�����i��j
	DrawBoxAA(74, 512, 76, 530, Gl, TRUE);		//�\�����i���j
	DrawBoxAA(45, 499, 63, 501, Gl, TRUE);		//�\�����i�E�j
	DrawBoxAA(87, 499, 105, 501, Gl, TRUE);		//�\�����i���j

	DrawCircleAA(575, 513, 18, 32, Bk, FALSE);	//B�{�^��
	DrawCircleAA(575, 435, 18, 32, Bk, FALSE);	//X�{�^��
	DrawCircleAA(536, 474, 18, 32, Bk, FALSE);	//Y�{�^��
	DrawCircleAA(614, 474, 18, 32, Bk, FALSE);	//A�{�^��

	DrawCircleAA(532, 600, 9, 32, Bk, FALSE);	//START�{�^��
	DrawCircleAA(532, 640, 9, 32, Bk, FALSE);	//SELECT�{�^��

	//�����\��------------------------------------------------------------
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
	//�@�@�@�@�@�@�@�@�Z���@�@�@�@�@�@�@�@�@�@�@�����@�@�@�@�񐔁@�@�������@�{���@�@���
	Move_Machine[0] = {"-",						�m�[�}��,	0,	0,	0,		0,		0};
	Move_Machine[1] = {"�ւ񂵂�",				�m�[�}��,	1,	1,	100,	0,		1};	//�Z���S����ւ�
	Move_Machine[2] = {"���傤�͂�",			�m�[�}��,	10,	10,	70,		0,		2};	//����̍U��1.2�{�A�h��0.8�{
	Move_Machine[3] = {"�L�b�N",				�m�[�}��,	20,	20,	90,		1.2	,	0};
	Move_Machine[4] = {"�r���h�A�b�v",			�m�[�}��,	10,	10,	50,		0,		3};	//�����̍U��1.5�{
	Move_Machine[5] = {"�邬�̂܂�",			��,			20,	20,	90,		1.2,	0};
	Move_Machine[6] = {"�\�[���[�r�[��",		��,			10,	10,	50,		1.5,	0};
	Move_Machine[7] = {"�͂��σJ�b�^�[",		��,			25,	25,	95,		1.1,	0};
	Move_Machine[8] = {"����ɂ�����",			��,			10,	10,	75,		0,		4};	//�����̖h��1.2�{
	Move_Machine[9] = {"���b�N�N���C��",		�m�[�}��,	10,	10,	70,		1.35,	0};
	Move_Machine[10] = {"�X�y�V�E��",			��,			20,	20,	90,		1.15,	0}; 
	Move_Machine[11] = {"�X�N���C�h",			�m�[�}��,	15,	15,	80,		1.2,	0};
	Move_Machine[12] = {"�w�u���~�b�N�X",		�m�[�}��,	15,	15,	80,		1.2,	0};
	Move_Machine[13] = {"�G���u���C�X",			��,			10,	10,	50,		1.45,	0};
	Move_Machine[14] = {"�u���X",				��,			25,	25,	95,		1.1,	0};
	Move_Machine[15] = {"�����������ǂ�",		�m�[�}��,	10,	10,	90,		0,		5};	//�����̃X�s�[�h1.3�{
	Move_Machine[16] = {"���̂܂�",				�m�[�}��,	20,	20,	90,		0,		6};	//����̍U���p�N��
	Move_Machine[17] = {"�A�C�A���N���[",		��,			10,	10,	70,		1.35,	0};
	Move_Machine[18] = {"�߂��Ƃ�",			�m�[�}��,	10,	10,	75,		0,		4}; //�����̖h��1.2�{
	Move_Machine[19] = {"�T�E�i",				��,			10,	10,	0,		0,		4};	//�����̖h��1.2�{
	Move_Machine[20] = {"�͂�����",				�m�[�}��,	99,	99,	0,		0.5,	0};	
	Move_Machine[21] = {"���イ����Ƃ��Ă�",	�m�[�}��,	1,	1,	70,		0,		7};	//�����̓I����100%�A�G�̖h��0.8�{
	Move_Machine[22] = {"������ׂ�",			�m�[�}��,	2,	2,	0,		0,		8};	//�G�̖h���0.5�{
	Move_Machine[23] = {"�͂˂�",				�m�[�}��,	50,	50,	10,		5,		0};
}

void PokemonCreate(Pokemon* pokemon, Move* Move_Machine) {
	pokemon[0] = { "���^�S��",		"picture/���^�S��.png",			"picture/���^�S��_��.png",		�m�[�}��,	250,250, 95,50,200,Move_Machine[1], Move_Machine[0], Move_Machine[0], Move_Machine[0] };
	pokemon[1] = { "�S�[���h�V�b�v","picture/�S�[���h�V�b�v.png",	"picture/�S�[���h�V�b�v_��.png",��,			280,280,120,30,300,Move_Machine[2], Move_Machine[3], Move_Machine[4], Move_Machine[5] };
	pokemon[2] = { "���ڂ��",		"picture/���ڂ��.png",			"picture/���ڂ��_��.png",		��,			290,290, 80,65,250,Move_Machine[6], Move_Machine[7], Move_Machine[8], Move_Machine[9] };
	pokemon[3] = { "�r���I��",		"picture/�r���I��.png",			"picture/�r���I��_��.png",		��,			275,275, 90,50,275,Move_Machine[10],Move_Machine[11],Move_Machine[12],Move_Machine[13] };
	pokemon[4] = { "�I���|��",		"picture/�I���|��.png",			"picture/�I���|��_��.png",		��,			250,250, 80,40,285,Move_Machine[3], Move_Machine[14],Move_Machine[15],Move_Machine[16] };
	pokemon[5] = { "�N���l�R",		"picture/�N���l�R.png",			"picture/�N���l�R_��.png",		��,			220,220, 85,40,290,Move_Machine[17],Move_Machine[2], Move_Machine[15],Move_Machine[18] };
	pokemon[6] = { "������",		"picture/������.png",			"picture/������_��.png",		��,			100,100, 90, 1,300,Move_Machine[19],Move_Machine[20],Move_Machine[21],Move_Machine[22] };
	pokemon[7] = { "�䂫�Ղ��",	"picture/�䂫�Ղ��.png",		"picture/�䂫�Ղ��_��.png",	��,			295,295,120,40,250,Move_Machine[23],Move_Machine[0], Move_Machine[0], Move_Machine[0] };
	pokemon[9] = { "-",				"picture/��.png",				"picture/��.png", };
}

//���1�`�揈��--------------------------------------------------------------------------------------------------------
void Screen1(MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//����ʓ��g
	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//����ʊO�g
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh, TRUE);	

	//�E��̂ق��̔w�i
	DrawBoxAA(160, 400, 490, 412.5, Ld, TRUE);
	//�E�Z������W
	DrawBoxAA(165, 435, 320, 510, Ld, TRUE);
	DrawBoxAA(165, 435, 320, 510, Bk, FALSE);
	DrawBoxAA(170, 440, 315, 505, Gl, TRUE);
	//�E�Z�E����W
	DrawBoxAA(330, 435, 485, 510, Ld, TRUE);
	DrawBoxAA(330, 435, 485, 510, Bk, FALSE);
	DrawBoxAA(335, 440, 480, 505, Gl, TRUE);
	//�E�Z�������W
	DrawBoxAA(165, 520, 320, 595, Ld, TRUE);
	DrawBoxAA(165, 520, 320, 595, Bk, FALSE);
	DrawBoxAA(170, 525, 315, 590, Gl, TRUE);
	//�E�Z�E�����W
	DrawBoxAA(330, 520, 485, 595, Ld, TRUE);
	DrawBoxAA(330, 520, 485, 595, Bk, FALSE);
	DrawBoxAA(335, 525, 480, 590, Gl, TRUE);
	//�E�����������W
	DrawBoxAA(180, 450, 470, 580, Lr, TRUE);
	DrawBoxAA(180, 450, 470, 580, Bk, FALSE);
	DrawBoxAA(195, 460, 455, 570, Dr, TRUE);
	//�E�|�P�������W
	DrawBoxAA(160, 600, 490, 650, Lg, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Dg, TRUE);

	//�E�����X�^�[�{�[���������W
	int x1 = 0;
	int x2 = 0;
	for (int poke_no = 0; poke_no < 6; poke_no++) {
		if (mypokemon[poke_no].pokemon.HP != 0) {
			LoadGraphScreen(165 + x1, 405, "picture/�����X�^�[�{�[��.png", TRUE);
		}
		else {
			LoadGraphScreen(165 + x1, 405, "picture/�{�[��.png", TRUE);
		}

		if (enemypokemon[poke_no].pokemon.HP != 0) {
			LoadGraphScreen(395 + x2, 405, "picture/�����X�^�[�{�[���E��.png", TRUE);
		}
		else {
			LoadGraphScreen(395 + x2, 405, "picture/�{�[���E��.png", TRUE);
		}
		x1 += 25;
		x2 += 15;
	}

	//����ʓ��g
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 500, "��������", Wh);
	DrawString(295, 610, "�|�P����", Wh);

}

//���2�`�揈��--------------------------------------------------------------------------------------------------------
void Screen2(MyPokemon* mypokemon) {

	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//����ʊO�g
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh,TRUE);	//����ʓ��g
	
	//�E��̂ق��̔w�i
	DrawBoxAA(160, 400, 490, 412.5, Ld, TRUE);

	//x1,x2,str
	int x,x1, x2;
	char* str;

	unsigned int Color;

	//�E�Z������W
	x1 = 170;
	x2 = 315;
	str = mypokemon[MyPokemonNumber].pokemon.MV1.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV1.Type);
	DrawBoxAA(165, 435, 320, 510, Color, TRUE);
	DrawBoxAA(165, 435, 320, 510, Bk, FALSE);
	DrawBoxAA(170, 440, 315, 505, Ld, TRUE);
	SetFontSize(22); 
	DrawFormatString(x, 445, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV1.name);
	DrawFormatString(205, 475, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV1.MoveCount,mypokemon[MyPokemonNumber].pokemon.MV1.MAXMoveCount);
	
	//�E�Z�E����W
	x1 = 335;
	x2 = 480;
	str = mypokemon[MyPokemonNumber].pokemon.MV2.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV2.Type);
	DrawBoxAA(330, 435, 485, 510, Color, TRUE);
	DrawBoxAA(330, 435, 485, 510, Bk, FALSE);
	DrawBoxAA(335, 440, 480, 505, Ld, TRUE);
	SetFontSize(22); 
	DrawFormatString(x, 445, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV2.name);
	DrawFormatString(370, 475, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV2.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV2.MAXMoveCount);
	
	//�E�Z�������W
	x1 = 170;
	x2 = 315;
	str = mypokemon[MyPokemonNumber].pokemon.MV3.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV3.Type);
	DrawBoxAA(165, 520, 320, 595, Color, TRUE);
	DrawBoxAA(165, 520, 320, 595, Bk, FALSE);
	DrawBoxAA(170, 525, 315, 590, Ld, TRUE);
	SetFontSize(22); 
	DrawFormatString(x, 530, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV3.name);
	DrawFormatString(205, 560, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV3.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV3.MAXMoveCount);
	
	//�E�Z�E�����W
	x1 = 335;
	x2 = 480;
	str = mypokemon[MyPokemonNumber].pokemon.MV4.name;
	x = CalcCenterX(x1, x2, str);
	Color = ColorChange(mypokemon[MyPokemonNumber].pokemon.MV4.Type);
	DrawBoxAA(330, 520, 485, 595, Color, TRUE);
	DrawBoxAA(330, 520, 485, 595, Bk, FALSE);
	DrawBoxAA(335, 525, 480, 590, Ld, TRUE);
	SetFontSize(22); 
	DrawFormatString(x, 530, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.MV4.name);
	DrawFormatString(370, 560, Bk, "PP %2d/%2d", mypokemon[MyPokemonNumber].pokemon.MV4.MoveCount, mypokemon[MyPokemonNumber].pokemon.MV4.MAXMoveCount);
	
	//�E���ǂ���W
	DrawBoxAA(160, 600, 490, 650, Lb, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Db, TRUE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//����ʓ��g

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 610, " ���ǂ� ", Wh);

}

//���3�`�揈��--------------------------------------------------------------------------------------------------------
void Screen3(MyPokemon* mypokemon) {

	DrawBoxAA(151, 391, 499, 659, Wh, TRUE);	//����ʊO�g
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Ld, TRUE);	//����ʓ��g

	//�E�|�P����1�̖ڍ����W
	if (mypokemon[0].pokemon.HP == 0){
		DrawBoxAA(160, 410, 323, 470, Orange, TRUE);
	}
	else {
		DrawBoxAA(160, 410, 323, 470, LLb, TRUE);
	}

	//�E�|�P����2�̖ڍ����W
	if (mypokemon[2].pokemon.HP == 0) {
		DrawBoxAA(160, 475, 323, 535, Orange, TRUE);
	}
	else {
		DrawBoxAA(160, 475, 323, 535, LLb, TRUE);
	}

	//�E�|�P����3�̖ڍ����W
	if (mypokemon[4].pokemon.HP == 0) {
		DrawBoxAA(160, 540, 323, 600, Orange, TRUE);
	}
	else {
		DrawBoxAA(160, 540, 323, 600, LLb, TRUE);
	}
	
	//�E�|�P����1�̖ډE���W
	if (mypokemon[1].pokemon.HP == 0) {
		DrawBoxAA(327, 420, 490, 480, Orange, TRUE);
	}
	else {
		DrawBoxAA(327, 420, 490, 480, LLb, TRUE);
	}
	
	//�E�|�P����2�̖ډE���W
	if (mypokemon[3].pokemon.HP == 0) {
		DrawBoxAA(327, 485, 490, 545, Orange, TRUE);
	}
	else {
		DrawBoxAA(327, 485, 490, 545, LLb, TRUE);
	}

	//�E�|�P����3�̖ډE���W
	if (mypokemon[5].pokemon.HP == 0) {
		DrawBoxAA(327, 550, 490, 610, Orange, TRUE);
	}
	else {
		DrawBoxAA(327, 550, 490, 610, LLb, TRUE);
	}
	

	//�E�����X�^�[�{�[�����W
	int height,SizeX, SizeY, GrHandle;

	GrHandle = LoadGraph(mypokemon[0].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(168, 425 + height, mypokemon[0].pokemon.sml_picture, TRUE);

	GrHandle = LoadGraph(mypokemon[2].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(168, 490 + height, mypokemon[2].pokemon.sml_picture, TRUE);

	GrHandle = LoadGraph(mypokemon[4].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(168, 555 + height, mypokemon[4].pokemon.sml_picture, TRUE);

	GrHandle = LoadGraph(mypokemon[1].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(335, 435 + height, mypokemon[1].pokemon.sml_picture, TRUE);

	GrHandle = LoadGraph(mypokemon[3].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(335, 500 + height, mypokemon[3].pokemon.sml_picture, TRUE);

	GrHandle = LoadGraph(mypokemon[5].pokemon.sml_picture);
	GetGraphSize(GrHandle, &SizeX, &SizeY);
	height = (int)((30 - SizeY) / 2);
	LoadGraphScreen(335, 565 + height, mypokemon[5].pokemon.sml_picture, TRUE);

	//�|�P�������O
	SetFontSize(18); 
	DrawString(215, 420, mypokemon[0].pokemon.name, Wh);
	DrawString(215, 485, mypokemon[2].pokemon.name, Wh);
	DrawString(215, 550, mypokemon[4].pokemon.name, Wh);
	DrawString(382, 430, mypokemon[1].pokemon.name, Wh);
	DrawString(382, 495, mypokemon[3].pokemon.name, Wh);
	DrawString(382, 560, mypokemon[5].pokemon.name, Wh);
	//�|�P����HP
	DrawFormatString(223, 445, Wh, "%d / %d", mypokemon[0].pokemon.HP, mypokemon[0].pokemon.MAXHP);
	DrawFormatString(223, 510, Wh, "%d / %d", mypokemon[2].pokemon.HP, mypokemon[2].pokemon.MAXHP);
	DrawFormatString(223, 575, Wh, "%d / %d", mypokemon[4].pokemon.HP, mypokemon[4].pokemon.MAXHP);
	DrawFormatString(390, 455, Wh, "%d / %d", mypokemon[1].pokemon.HP, mypokemon[1].pokemon.MAXHP);
	DrawFormatString(390, 520, Wh, "%d / %d", mypokemon[3].pokemon.HP, mypokemon[3].pokemon.MAXHP);
	DrawFormatString(390, 585, Wh, "%d / %d", mypokemon[5].pokemon.HP, mypokemon[5].pokemon.MAXHP);



	//�E�|�P�����R�����W
	DrawBoxAA(160, 620, 405, 650, Pi, TRUE);
	DrawBoxAA(160, 620, 405, 650, Bk, FALSE);
	DrawBoxAA(165, 625, 380, 650, Wh, TRUE);
	//�E���ǂ���W
	DrawBoxAA(410, 620, 490, 650, Lb, TRUE);
	DrawBoxAA(410, 620, 490, 650, Bk, FALSE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//����ʓ��g

	SetFontSize(18);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(175, 628, " �|�P�������@�����ł������� ", Bk);
	DrawString(428, 626, " ���ǂ� ", Wh);

}

int Select1(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//������
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//�J�[�\���ʒu(1)
	int cursor = 1;

	while (1) {

		//[�~]�{�^������
		if (GetWindowUserCloseFlag(TRUE)) {
			if (MessageBox(GetMainWindowHandle(), _T("�I�����܂����H"), _T("�m�F"), MB_YESNO) == IDYES) {
				flg = 0;
				return flg;
			}
		}

		// �}�E�X����
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//�����A�}�E�X�̃{�^����������Ă��邩�A������Ă���{�^��������������
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0, cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//�����A���̍��W��A�{�^���̈ʒu��������
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

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					cnt = 1;
					break;
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
					break;
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
					break;
				}

			}

			//�����\���̏ゾ������
			if (MouseX >= (63) && MouseX <= (87) && MouseY >= (460) && MouseY <= (488)) {
				cursor = 1;
			}

			//�����\���̉���������
			else if (MouseX >= (63) && MouseX <= (87) && MouseY >= (512) && MouseY <= (540)) {
				cursor = 2;
			}

			//�����\���̍���������
			else if (MouseX >= (35) && MouseX <= (63) && MouseY >= (488) && MouseY <= (512)) {

			}

			//�����\���̉E��������
			else if (MouseX >= (87) && MouseX <= (115) && MouseY >= (488) && MouseY <= (512)) {

			}

			//�����A���̍��W������������������
			else if (MouseX >= 180 && MouseX <= 470 && MouseY >= 450 && MouseY <= 580) {
				flg = 2;
				cnt = 1;
			}

			//�����A���̍��W���|�P������������
			else if (MouseX >= 160 && MouseX <= 490 && MouseY >= 600 && MouseY <= 650) {
				flg = 3;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//�J�[�\���ړ�
		if (cursor == 1) {
			Screen1(mypokemon, enemypokemon);
			//DrawBoxAA(180, 450, 470, 580, Yl, FALSE);
			LoadGraphScreen(180 - 18, 450 - 18, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(470 - 2, 450 - 18, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(180 - 18, 580 - 2, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(470 - 2, 580 - 2, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 2) {
			Screen1(mypokemon, enemypokemon);
			//DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
			LoadGraphScreen(160 , 600 , "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20 , 600 , "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(160 , 650 - 20 , "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20 , 650 - 20 , "picture/�J�[�\���E��.png", TRUE);
		}

		//������
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

	}

	return flg;

}

int Select2(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//������
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//�J�[�\���ʒu(1)
	int cursor = 1;

	while (1) {

		//[�~]�{�^������
		if (GetWindowUserCloseFlag(TRUE)) {
			if (MessageBox(GetMainWindowHandle(), _T("�I�����܂����H"), _T("�m�F"), MB_YESNO) == IDYES) {
				flg = 0; 
				return flg;
			}
		}

		// �}�E�X����
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//�����A�}�E�X�̃{�^����������Ă��邩�A������Ă���{�^��������������
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0;
			int cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//�����A���̍��W��A�{�^���̈ʒu��������
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
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I",mypokemon[MyPokemonNumber].pokemon.name ,mypokemon[MyPokemonNumber].pokemon.MV1.name);
							Abutton();
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
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
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
							Abutton();
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
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
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
							Abutton();
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
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
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
							Abutton();
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
						}
					}

					else if (cursor == 5) {
						flg = 1;
						cnt = 1;
					}

				}

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
					break;
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
					break;
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
					break;
				}

			}

			//�����\���̏ゾ������
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

			//�����\���̉���������
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

			//�����\���̍���������
			else if (MouseX >= (35) && MouseX <= (63) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 2) {
					cursor = 1;
				}
				else if (cursor == 4) {
					cursor = 3;
				}
			}

			//�����\���̉E��������
			else if (MouseX >= (87) && MouseX <= (115) && MouseY >= (488) && MouseY <= (512)) {
				if (cursor == 1) {
					cursor = 2;
				}
				else if (cursor == 3) {
					cursor = 4;
				}
			}

			//�����A���̍��W���Z1��������
			else if (MouseX >= 165 && MouseX <= 320 && MouseY >= 435 && MouseY <= 510) {
				if (mypokemon[MyPokemonNumber].pokemon.MV1.MoveCount != 0) {
					MoveNumber = 1;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV1.name);
					Abutton();
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
				}
			}

			//�����A���̍��W���Z2��������
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 435 && MouseY <= 510) {
				if (mypokemon[MyPokemonNumber].pokemon.MV2.MoveCount != 0) {
					MoveNumber = 2;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
					Abutton();
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
				}

			}

			//�����A���̍��W���Z3��������
			else if (MouseX >= 165 && MouseX <= 320 && MouseY >= 520 && MouseY <= 595) {
				if (mypokemon[MyPokemonNumber].pokemon.MV3.MoveCount != 0) {
					MoveNumber = 3;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;	
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
				
				}
			}

			//�����A���̍��W���Z4��������
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 520 && MouseY <= 595) {
				if (mypokemon[MyPokemonNumber].pokemon.MV4.MoveCount != 0) {
					MoveNumber = 4;
					TurnCheck(mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�́@�|�C���g�����\n%s���@���肾���Ȃ��I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
					Abutton();
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂ǂ�����H", mypokemon[MyPokemonNumber].pokemon.name);
				}
			}

			//�����A���̍��W�����ǂ邾������
			else if (MouseX >= 160 && MouseX <= 490 && MouseY >= 600 && MouseY <= 650 ){
				flg = 1;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//�J�[�\���ړ�
		if (cursor == 1) {
			Screen2(mypokemon);
			//DrawBoxAA(165, 435, 320, 510, Yl, FALSE);
			LoadGraphScreen(165 - 5 , 435 - 5 , "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(320 - 15, 435 - 5, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(165 - 5 , 510 - 15, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(320 - 15, 510 - 15, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 2) {
			Screen2(mypokemon);
			//DrawBoxAA(330, 435, 485, 510, Yl, FALSE);
			LoadGraphScreen(330 - 5, 435 - 5, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(485 - 15, 435 - 5, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(330 - 5, 510 - 15, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(485 - 15, 510 - 15, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 3) {
			Screen2(mypokemon);
			//DrawBoxAA(165, 520, 320, 595, Yl, FALSE);
			LoadGraphScreen(165 - 5, 520 - 5, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(320 - 15, 520 - 5, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(165 - 5, 595 - 15, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(320 - 15, 595 - 15, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 4) {
			Screen2(mypokemon);
			//DrawBoxAA(330, 520, 485, 595, Yl, FALSE);
			LoadGraphScreen(330 - 5, 520 - 5, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(485 - 15, 520 - 5, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(330 - 5, 595 - 15, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(485 - 15, 595 - 15, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 5) {
			Screen2(mypokemon);
			//DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
			LoadGraphScreen(160, 600, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 600, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(160, 650 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 650 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		//������
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

	}

	return flg;

}

int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//������
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;
	
	//��������Ƃ��킽������
	int MyPokemonChangeNumber = 0;

	//�J�[�\���ʒu(1)
	int cursor = 1;

	while (1) {

		//[�~]�{�^������
		if (GetWindowUserCloseFlag(TRUE)) {
			if (MessageBox(GetMainWindowHandle(), _T("�I�����܂����H"), _T("�m�F"), MB_YESNO) == IDYES) {
				flg = 0;
				return flg;
			}
		}

		// �}�E�X����
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//�����A�}�E�X�̃{�^����������Ă��邩�A������Ă���{�^��������������
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0;
			int cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//�����A���̍��W��A�{�^���̈ʒu��������
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {

					if (cursor == 1) {
						MyPokemonChangeNumber = 0;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber,mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 2) {
						MyPokemonChangeNumber = 1;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 3) {
						MyPokemonChangeNumber = 2;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 4) {
						MyPokemonChangeNumber = 3;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 5) {
						MyPokemonChangeNumber = 4;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 6) {
						MyPokemonChangeNumber = 5;
						if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
							MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
							flg = 1;
							cnt = 1;
						}
						else {
							SetFontSize(22); 
							DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
							DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
						}
					}

					else if (cursor == 7 && mypokemon_die_flg != 1) {
						flg = 1;
						cnt = 1;
					}
				}

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					if (mypokemon_die_flg != 1) {
						flg = 1;
						cnt = 1;
					}
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
				}
			}

			//�����\���̏ゾ������
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

			//�����\���̉���������
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

			//�����\���̍���������
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

			//�����\���̉E��������
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

			//�����A���̍��W���|�P����1��������
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 410 && MouseY <= 470) {
				MyPokemonChangeNumber = 0;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W���|�P����2��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 420 && MouseY <= 480) {
				MyPokemonChangeNumber = 1;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W���|�P����3��������
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 475 && MouseY <= 535) {
				MyPokemonChangeNumber = 2;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W���|�P����4��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 485 && MouseY <= 545) {
				MyPokemonChangeNumber = 3;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W���|�P����5��������
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 540 && MouseY <= 600) {
				MyPokemonChangeNumber =	4;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W���|�P����6��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 550 && MouseY <= 610) {
				MyPokemonChangeNumber = 5;
				if (MyPokemonNumber != MyPokemonChangeNumber && mypokemon[MyPokemonChangeNumber].pokemon.HP != 0) {
					MyPokemonChange(MyPokemonChangeNumber, mypokemon, enemypokemon);
					cnt = 1;
					flg = 1;
				}
				else {
					SetFontSize(22); 
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���񂽂�����Ă���|�P������\n�ւ񂱂��ł��܂���B");
				}
			}

			//�����A���̍��W�����ǂ邾������
			else if (MouseX >= 410 && MouseX <= 490 && MouseY >= 620 && MouseY <= 650) {
				if (mypokemon_die_flg != 1){
					flg = 1;
					cnt = 1;
				}
			}

			if (cnt == 1) {
				break;
			}

		}

		//�J�[�\���ړ�
		if (cursor == 1) {
			Screen3(mypokemon);
			//DrawBoxAA(160, 410, 323, 470, Yl, FALSE);
			LoadGraphScreen(160 , 410 , "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 410 , "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(160 , 470 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 470 - 20, "picture/�J�[�\���E��.png", TRUE);

		}

		else if (cursor == 2) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 420, 490, 480, Yl, FALSE);
			LoadGraphScreen(327, 420, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 420, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(327, 480 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 480 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 3) {
			Screen3(mypokemon);
			//DrawBoxAA(160, 475, 323, 535, Yl, FALSE);
			LoadGraphScreen(160, 475, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 475, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(160, 535 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 535 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 4) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 485, 490, 545, Yl, FALSE);
			LoadGraphScreen(327, 485, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 485, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(327, 545 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 545 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 5) {
			Screen3(mypokemon);
			//DrawBoxAA(160, 540, 323, 600, Yl, FALSE);
			LoadGraphScreen(160, 540, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 540, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(160, 600 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(323 - 20, 600 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 6) {
			Screen3(mypokemon);
			//DrawBoxAA(327, 550, 490, 610, Yl, FALSE);
			LoadGraphScreen(327, 550, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 550, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(327, 610 - 20, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 20, 610 - 20, "picture/�J�[�\���E��.png", TRUE);
		}

		else if (cursor == 7) {
			Screen3(mypokemon);
			//DrawBoxAA(410, 620, 490, 650, Yl, FALSE);
			LoadGraphScreen(410 - 5, 620 - 5, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 15, 620 - 5, "picture/�J�[�\���E��.png", TRUE);
			LoadGraphScreen(410 - 5, 650 - 15, "picture/�J�[�\������.png", TRUE);
			LoadGraphScreen(490 - 15, 650 - 15, "picture/�J�[�\���E��.png", TRUE);
		}

		//������
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;

	}

	return flg;

}

void TurnCheck(MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	LoadGraphScreen(DW_x1, DW_y1, "picture/�U���������.jpg", TRUE);	//�w�i�摜

	srand((unsigned)time(NULL));
	float MySPD = ((rand() % 600 * 0.001) + 0.7) * (mypokemon[MyPokemonNumber].pokemon.SPD);
	float EnemySPD = ((rand() % 600 * 0.001) + 0.7) * (enemypokemon[EnemyPokemonNumber].pokemon.SPD);

	srand((unsigned)time(NULL));
	
	//�G�̍U���I��
	EnemyMoveNumber = rand() % 4 + 1;
	while(1){
		if (EnemyMoveNumber == 1) {
			if (enemypokemon[EnemyPokemonNumber].pokemon.MV1.MoveCount != 0) {
				break;
			}
			EnemyMoveNumber = 2;
		}

		else if (EnemyMoveNumber == 2) {
			if (enemypokemon[EnemyPokemonNumber].pokemon.MV2.MoveCount != 0) {
				break;
			}
			EnemyMoveNumber = 3;
		}

		else if (EnemyMoveNumber == 3) {
			if (enemypokemon[EnemyPokemonNumber].pokemon.MV3.MoveCount != 0) {
				break;
			}
			EnemyMoveNumber = 4;
		}

		else if (EnemyMoveNumber == 4) {
			if (enemypokemon[EnemyPokemonNumber].pokemon.MV4.MoveCount != 0) {
				break;
			}
			EnemyMoveNumber = 1;
		}
	}
	

	if (MySPD >= EnemySPD) {
		Myturn(mypokemon, enemypokemon);
		if (EnemyPokemon_flg == 1) {
			EnemyPokemon_flg = 0;
		}
		else {
			Enemyturn(mypokemon, enemypokemon);
		}
	}

	else if (EnemySPD > MySPD) {
		Enemyturn(mypokemon, enemypokemon);
		if (MyPokemon_flg == 1) {
			MyPokemon_flg = 0;
		}
		else {
			Myturn(mypokemon, enemypokemon);
		}
	}

}

void MyPokemonChange(int MyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {
	
	LoadGraphScreen(DW_x1, DW_y1, "picture/�U���������.jpg", TRUE);	//�w�i�摜

	if (mypokemon[MyPokemonChangeNumber].pokemon.MAXHP != 0) {

		if (mypokemon_die_flg != 1) {
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ���������I\n���ǂ�I", mypokemon[MyPokemonNumber].pokemon.name);
			Abutton();
		}

		MyPokemonNumber = MyPokemonChangeNumber;

		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
		LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
		LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�G�̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		SetFontSize(22); 
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�䂯���I %s�I", mypokemon[MyPokemonNumber].pokemon.name);
		MyTextHpgauge(mypokemon);
		EnemyTextHpgauge(enemypokemon);
		Abutton();

		if (mypokemon_die_flg != 1) {
			Enemyturn(mypokemon, enemypokemon);
		}
	}

}

void EnemyPokemonChange(int EnemyPokemonChangeNumber, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	LoadGraphScreen(DW_x1, DW_y1, "picture/�U���������.jpg", TRUE);	//�w�i�摜

	if (enemypokemon[EnemyPokemonChangeNumber].pokemon.MAXHP != 0) {

		EnemyPokemonNumber++;

		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
		LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
		LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�G�̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		SetFontSize(22); 
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�䂯���I %s�I", enemypokemon[EnemyPokemonNumber].pokemon.name);
		MyTextHpgauge(mypokemon);
		EnemyTextHpgauge(enemypokemon);
		Abutton();

	}
}

void Myturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {

	SetFontSize(22);
	
	if(MoveNumber == 1){
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV1.name); 
		Abutton();
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV1);
	}

	else if (MoveNumber == 2) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV2.name);
		Abutton();
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV2);
	}

	else if (MoveNumber == 3) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV3.name);
		Abutton(); 
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV3);
	}

	else if (MoveNumber == 4) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, mypokemon[MyPokemonNumber].pokemon.MV4.name);
		Abutton(); 
		MySelectMove(enemypokemon, mypokemon, &mypokemon[MyPokemonNumber].pokemon.MV4);
	}

	//�G��HP��0�ɂȂ����Ƃ�
	if (enemypokemon[EnemyPokemonNumber].pokemon.HP <= 0) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
		LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		SetFontSize(22);
		MyTextHpgauge(mypokemon);
		SetFontSize(22);
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂����ꂽ�I", enemypokemon[EnemyPokemonNumber].pokemon.name);
		Abutton();
		EnemyPokemonNumber++;
		//�G���S�ł����Ƃ�
		if (enemypokemon[EnemyPokemonNumber].pokemon.HP == 0) {
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̃|�P�����͂���߂���", TrainerName[0]);
			Abutton();
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�ɂ��傤�肵��!!", TrainerName[0]);
			Abutton();
			DxLib_End();
			exit(0);
		}
		else {
			EnemyPokemonNumber--;
			EnemyPokemonChange(EnemyPokemonNumber, mypokemon, enemypokemon);
		}
		EnemyPokemon_flg = 1;	//�^�[�����΂�
	}
}

void Enemyturn(MyPokemon *mypokemon, EnemyPokemon *enemypokemon) {

	SetFontSize(22);

	if (EnemyMoveNumber == 1) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV1.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV1);
	}

	else if (EnemyMoveNumber == 2) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV2.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV2);
	}

	else if (EnemyMoveNumber == 3) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV3.name);
		Abutton();
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV3);
	}

	else if (EnemyMoveNumber == 4) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", enemypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV4.name);
		Abutton(); 
		EnemySelectMove(mypokemon, enemypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV4);
	}

	//������HP���ɂȂ����Ƃ�
	if (mypokemon[MyPokemonNumber].pokemon.HP <= 0) {
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
		LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
		UpInitScreen(mypokemon, enemypokemon);
		SetFontSize(22);
		EnemyTextHpgauge(enemypokemon);
		SetFontSize(22);
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂����ꂽ�I", mypokemon[MyPokemonNumber].pokemon.name);
		Abutton();

		int no_HP_cnt = 0;
		int i;

		//��������ł���|�P�����̐��J�E���g
		for (i = 0; i < MAXPokemon; i++){
			if (mypokemon[i].pokemon.HP == 0) {
				no_HP_cnt++;
			}
		}

		//�������S�ł����Ƃ�
		if (no_HP_cnt == MAXPokemon) {
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�͂߂̂܂����܂�����ɂȂ���...", TrainerName[1]);
			Abutton();
			SetFontSize(88);
			DrawBoxAA(0, 0, 650, 700, Bk, TRUE);
			DrawFormatString(0,0,Wh, "[�~]�{�^���ŏI����");
			Abutton();
			DxLib_End();
			exit(0);
		}
		//�܂������̃|�P�������c���Ă���Ƃ�
		else {
			mypokemon_die_flg = 1;
			SetFontSize(22);
			DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
			DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���肾���|�P���������񂽂����Ă�������");
			Select3(3, mypokemon, enemypokemon);
			mypokemon_die_flg = 0;
			MyPokemon_flg = 1;
		}
	}

}

void Abutton() {

	//������
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	int	a = 0;

	while (1) {

		//[�~]�{�^������
		if (GetWindowUserCloseFlag(TRUE)) {
			if (MessageBox(GetMainWindowHandle(), _T("�I�����܂����H"), _T("�m�F"), MB_YESNO) == IDYES) {
				DxLib_End();
				exit(0);
			}
		}

		// �}�E�X����
		GetMouseInputLog2(&MouseInput, &MouseX, &MouseY, &LogType, TRUE);

		//�����A�}�E�X�̃{�^����������Ă��邩�A������Ă���{�^��������������
		if (LogType == MOUSE_INPUT_LOG_DOWN && (MouseInput & MOUSE_INPUT_LEFT) != 0) {

			double deg = 0, len = 18, rad = 0, x = 0, y = 0, cnt = 0;

			for (int deg = 0; deg <= 90; deg++) {

				x = len * cos(deg);
				y = len * sin(deg);

				//�����A���̍��W��A�{�^���̈ʒu��������
				if (MouseX >= (614 - x) && MouseX <= (614 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					a = 1;
				}
			}
		}

		if (a == 1) {
			break;
		}
	}

	//������
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

	//��������
	srand((unsigned)time(NULL));

	//�Z�̃J�E���g�����炷
	MyMove->MoveCount--;

	//������
	if (MyMove->Hitrate - ((rand() % 100) + 1) >= 0) {

		int HPCount = 0;
		int kyusyo = 0;

		//�|�P�����̋Z
		SetFontSize(22); 		
		switch (MyMove->MoveCategory) {
			//�ʏ�U���̎�
			case 0:
				if ((MyMove->Type == 1 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 2) ||
					(MyMove->Type == 2 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 3) ||
					(MyMove->Type == 3 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 1) ||
					(MyMove->Type == 4 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 5) ||
					(MyMove->Type == 5 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 4)) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�������́@���܂ЂƂ̂悤��...");
					ElementEffect = 0.7;
				}

				else if ((MyMove->Type == 2 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 1) ||
					(MyMove->Type == 3 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 2) ||
					(MyMove->Type == 1 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 3) ||
					(MyMove->Type == 5 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 4) ||
					(MyMove->Type == 4 && enemypokemon[EnemyPokemonNumber].pokemon.ELE == 5)) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�������́@�΂��񂾁I");
					ElementEffect = 1.3;
				}

				else {
					ElementEffect = 1.0;
				}

				//9%�̊m���ł��イ����ɓ�����
				if ((rand() % 100) >= 90) {
					kyusyo = 1;
					ElementEffect += 1.5;
				}

				AtkNum = ((mypokemon[MyPokemonNumber].pokemon.ATK * MyMove->ATKscale - enemypokemon[EnemyPokemonNumber].pokemon.DEF) * ElementEffect * ((rand() % 700 * 0.001) + 0.7));
				HPCount = AtkNum;

				//HP�����炷
				while (HPCount >= 1) {
					enemypokemon[EnemyPokemonNumber].pokemon.HP -= 1;
					EnemyTextHpgauge(enemypokemon);
					HPCount--;
					if (enemypokemon[EnemyPokemonNumber].pokemon.HP <= 0) break;
				}

				if (kyusyo) {
					SetFontSize(22);
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���イ����ɂ��������I");
				}
				break;

			//�Z���S����ւ�
			case 1:
				mypokemon[MyPokemonNumber].pokemon.MV1 = enemypokemon[EnemyPokemonNumber].pokemon.MV1;
				mypokemon[MyPokemonNumber].pokemon.MV2 = enemypokemon[EnemyPokemonNumber].pokemon.MV2;
				mypokemon[MyPokemonNumber].pokemon.MV3 = enemypokemon[EnemyPokemonNumber].pokemon.MV3;
				mypokemon[MyPokemonNumber].pokemon.MV4 = enemypokemon[EnemyPokemonNumber].pokemon.MV4;
				for (int a = 0; a < 255; a++) {
					mypokemon[MyPokemonNumber].pokemon.big_picture[a] = enemypokemon[EnemyPokemonNumber].pokemon.big_picture[a];
					mypokemon[MyPokemonNumber].pokemon.sml_picture[a] = enemypokemon[EnemyPokemonNumber].pokemon.sml_picture[a];
				}
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
				LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
				LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�G�̃|�P�����摜
				UpInitScreen(mypokemon, enemypokemon);
				MyTextHpgauge(mypokemon);
				EnemyTextHpgauge(enemypokemon);
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s��%s�ɂւ񂵂񂵂�", mypokemon[MyPokemonNumber].pokemon.name,enemypokemon[EnemyPokemonNumber].pokemon.name);
				break;

			//����̍U��1.2�{�A�h��0.8�{
			case 2:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂������������������I", enemypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				enemypokemon[EnemyPokemonNumber].pokemon.ATK *= 1.2;
				enemypokemon[EnemyPokemonNumber].pokemon.DEF *= 0.8;
				break;

			//�����̍U��1.5�{
			case 3:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂������������������I", mypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[MyPokemonNumber].pokemon.ATK *= 1.5;
				break;

			//�����̖h��1.1�{
			case 4:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", mypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[MyPokemonNumber].pokemon.DEF *= 1.1;
				break;

			//�����̃X�s�[�h1.3�{
			case 5:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂��΂₳�����������I", mypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[MyPokemonNumber].pokemon.SPD *= 1.3;
				break;

			//����̍U���p�N��
			case 6:
				SetFontSize(22);
				if (EnemyMoveNumber == 1) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV1.name);
					Abutton();
					MySelectMove(enemypokemon, mypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV1);
				}

				else if (EnemyMoveNumber == 2) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV2.name);
					Abutton();
					MySelectMove(enemypokemon, mypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV2);
				}

				else if (EnemyMoveNumber == 3) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV3.name);
					Abutton();
					MySelectMove(enemypokemon, mypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV3);
				}

				else if (EnemyMoveNumber == 4) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[MyPokemonNumber].pokemon.name, enemypokemon[EnemyPokemonNumber].pokemon.MV4.name);
					Abutton();
					MySelectMove(enemypokemon, mypokemon, &enemypokemon[EnemyPokemonNumber].pokemon.MV4);
				}
				break;

			//�����̖�����100%�A�G�̖h���0.8�{
			case 7:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂߂����イ������������I", mypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				enemypokemon[EnemyPokemonNumber].pokemon.DEF *= 0.8;
				mypokemon[MyPokemonNumber].pokemon.MV1.Hitrate = 100;
				mypokemon[MyPokemonNumber].pokemon.MV2.Hitrate = 100;
				mypokemon[MyPokemonNumber].pokemon.MV3.Hitrate = 100;
				mypokemon[MyPokemonNumber].pokemon.MV4.Hitrate = 100;
				break;

			//�G�̖h���0.5�{
			case 8:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				enemypokemon[EnemyPokemonNumber].pokemon.DEF *= 0.5;
				break;
		}
	}

	else {
		SetFontSize(22);
		//������Ȃ������B
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�ɂ�\n������Ȃ������I", enemypokemon[EnemyPokemonNumber].pokemon.name);
	}

	Abutton();

	Sleep(500);

}

void EnemySelectMove(MyPokemon* enemypokemon, EnemyPokemon* mypokemon, Move* MyMove) {

	int AtkNum = 0;
	float ElementEffect = 0;

	//��������
	srand((unsigned)time(NULL));

	//�Z�̃J�E���g�����炷
	MyMove->MoveCount--;

	//������
	if (MyMove->Hitrate - ((rand() % 100) + 1) >= 0) {

		int HPCount = 0;
		int kyusyo = 0;

		//�|�P�����̋Z
		SetFontSize(22); 
		switch (MyMove->MoveCategory) {
			//�ʏ�U���̎�
			case 0:
				if ((MyMove->Type == 1 && enemypokemon[MyPokemonNumber].pokemon.ELE == 2) ||
					(MyMove->Type == 2 && enemypokemon[MyPokemonNumber].pokemon.ELE == 3) ||
					(MyMove->Type == 3 && enemypokemon[MyPokemonNumber].pokemon.ELE == 1) ||
					(MyMove->Type == 4 && enemypokemon[MyPokemonNumber].pokemon.ELE == 5) ||
					(MyMove->Type == 5 && enemypokemon[MyPokemonNumber].pokemon.ELE == 4)) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�������́@���܂ЂƂ̂悤��...");
					ElementEffect = 0.7;
				}

				else if ((MyMove->Type == 2 && enemypokemon[MyPokemonNumber].pokemon.ELE == 1) ||
					(MyMove->Type == 3 && enemypokemon[MyPokemonNumber].pokemon.ELE == 2) ||
					(MyMove->Type == 1 && enemypokemon[MyPokemonNumber].pokemon.ELE == 3) ||
					(MyMove->Type == 5 && enemypokemon[MyPokemonNumber].pokemon.ELE == 4) ||
					(MyMove->Type == 4 && enemypokemon[MyPokemonNumber].pokemon.ELE == 5)) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "�������́@�΂��񂾁I");
					ElementEffect = 1.3;
				}

				else {
					ElementEffect = 1.0;
				}

				//9%�̊m���ł��イ����ɓ�����
				if ((rand() % 100) >= 90) {
					kyusyo = 1;
					ElementEffect += 1.5;
				}

				AtkNum = ((mypokemon[EnemyPokemonNumber].pokemon.ATK * MyMove->ATKscale - enemypokemon[MyPokemonNumber].pokemon.DEF) * ElementEffect * ((rand() % 700 * 0.001) + 0.7));
				HPCount = AtkNum;

				while (HPCount >= 1) {
					enemypokemon[MyPokemonNumber].pokemon.HP -= 1;
					MyTextHpgauge(enemypokemon);
					HPCount--;
					if (enemypokemon[MyPokemonNumber].pokemon.HP <= 0) break;
				}

				if (kyusyo) {
					SetFontSize(22);
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "���イ����ɂ��������I");
				}
				break;
			
			//�Z���S����ւ�
			case 1:
				mypokemon[MyPokemonNumber].pokemon.MV1 = enemypokemon[EnemyPokemonNumber].pokemon.MV1;
				mypokemon[MyPokemonNumber].pokemon.MV2 = enemypokemon[EnemyPokemonNumber].pokemon.MV2;
				mypokemon[MyPokemonNumber].pokemon.MV3 = enemypokemon[EnemyPokemonNumber].pokemon.MV3;
				mypokemon[MyPokemonNumber].pokemon.MV4 = enemypokemon[EnemyPokemonNumber].pokemon.MV4;
				for (int a = 0; a < 255; a++) {
					mypokemon[EnemyPokemonNumber].pokemon.big_picture[a] = enemypokemon[MyPokemonNumber].pokemon.big_picture[a];
					mypokemon[EnemyPokemonNumber].pokemon.sml_picture[a] = enemypokemon[MyPokemonNumber].pokemon.sml_picture[a];
				}
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				LoadGraphScreen(UP_x1, UP_y1, "picture/�r�[�`.jpg", FALSE);	//�w�i�摜
				LoadGraphScreen(UP_x1 + 35, UP_y1 + 80, mypokemon[EnemyPokemonNumber].pokemon.big_picture, TRUE);	//�����̃|�P�����摜
				LoadGraphScreen(UP_x1 + 200, UP_y1 + 10, enemypokemon[MyPokemonNumber].pokemon.big_picture, TRUE);	//�G�̃|�P�����摜
				UpInitScreen(enemypokemon, mypokemon);
				MyTextHpgauge(enemypokemon);
				EnemyTextHpgauge(mypokemon);
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s��%s�ɂւ񂵂񂵂�", mypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[MyPokemonNumber].pokemon.name);
				break;

			//����̍U��1.2�{�A�h��0.8�{
			case 2:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂������������������I",enemypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[MyPokemonNumber].pokemon.name);
				Abutton();

				enemypokemon[MyPokemonNumber].pokemon.ATK *= 1.2;
				enemypokemon[MyPokemonNumber].pokemon.DEF *= 0.8;

				break;

			//�����̍U��1.5�{
			case 3:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂������������������I", mypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[EnemyPokemonNumber].pokemon.ATK *= 1.5;
				break;

			//�����̖h��1.2�{
			case 4:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", mypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[EnemyPokemonNumber].pokemon.DEF *= 1.1;
				break;

			//�����̃X�s�[�h1.3�{
			case 5:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂��΂₳�����������I", mypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				mypokemon[EnemyPokemonNumber].pokemon.SPD *= 1.3;
				break;

			//����̍U���p�N��
			case 6:
				SetFontSize(22);
				if (MoveNumber == 1) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[MyPokemonNumber].pokemon.MV1.name);
					Abutton();
					EnemySelectMove(enemypokemon, mypokemon, &enemypokemon[MyPokemonNumber].pokemon.MV1);
				}

				else if (MoveNumber == 2) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[MyPokemonNumber].pokemon.MV2.name);
					Abutton();
					EnemySelectMove(enemypokemon, mypokemon, &enemypokemon[MyPokemonNumber].pokemon.MV2);
				}

				else if (MoveNumber == 3) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[MyPokemonNumber].pokemon.MV3.name);
					Abutton();
					EnemySelectMove(enemypokemon, mypokemon, &enemypokemon[MyPokemonNumber].pokemon.MV3);
				}

				else if (MoveNumber == 4) {
					DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
					DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s ��\n%s�I", mypokemon[EnemyPokemonNumber].pokemon.name, enemypokemon[MyPokemonNumber].pokemon.MV4.name);
					Abutton();
					EnemySelectMove(enemypokemon, mypokemon, &enemypokemon[MyPokemonNumber].pokemon.MV4);
				}
				break;

			//�����̖�����100%�A�G�̖h���0.8�{
			case 7:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂߂����イ������������I", mypokemon[EnemyPokemonNumber].pokemon.name);
				Abutton();
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				enemypokemon[MyPokemonNumber].pokemon.DEF *= 0.8;
				mypokemon[EnemyPokemonNumber].pokemon.MV1.Hitrate = 100;
				mypokemon[EnemyPokemonNumber].pokemon.MV2.Hitrate = 100;
				mypokemon[EnemyPokemonNumber].pokemon.MV3.Hitrate = 100;
				mypokemon[EnemyPokemonNumber].pokemon.MV4.Hitrate = 100;
				break;

			//�G�̖h���0.5�{
			case 8:
				SetFontSize(22);
				DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
				DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�̂ڂ����傪���������I", enemypokemon[MyPokemonNumber].pokemon.name);
				Abutton();
				enemypokemon[MyPokemonNumber].pokemon.DEF *= 0.5;
				break;
		}
	}

	else {

		//������Ȃ������B
		DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��
		SetFontSize(22); 
		DrawFormatString(UP_x1 + 10, UP_y1 + 190 + 5, Bk, "%s�ɂ�\n������Ȃ������I", enemypokemon[MyPokemonNumber].pokemon.name);
	}

	Abutton();

	Sleep(500);
}

void EnemyTextHpgauge(EnemyPokemon* enemypokemon) {

	ScreenFlip();// ����ʃf�[�^��\��ʂ֔��f
	SetDrawScreen(DX_SCREEN_BACK); // �`���𗠉�ʂɐݒ�

	SetFontSize(20);
	SetFontThickness(9);
	DrawBoxAA(UP_x1, UP_y1 + 25, UP_x1 + 140, UP_y1 + 65, Wh, TRUE);
	DrawBoxAA(UP_x1, UP_y1 + 25, UP_x1 + 140, UP_y1 + 65, Bk, FALSE);
	DrawTriangle(UP_x1 + 140, UP_y1 + 45, UP_x1 + 140, UP_y1 + 55, UP_x1 + 155, UP_y1 + 50, Bk, TRUE);
	DrawFormatString(UP_x1+5, UP_y1 + 25, Bk, "%s", enemypokemon[EnemyPokemonNumber].pokemon.name);

	//HP�o�[
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

	ScreenFlip();// ����ʃf�[�^��\��ʂ֔��f
	SetDrawScreen(DX_SCREEN_FRONT); // �`���𗠉�ʂɐݒ�
}

void MyTextHpgauge(MyPokemon* mypokemon) {

	ScreenFlip();// ����ʃf�[�^��\��ʂ֔��f
	SetDrawScreen(DX_SCREEN_BACK); // �`���𗠉�ʂɐݒ�

	SetFontSize(20);
	SetFontThickness(9);
	DrawBoxAA(UP_x2 - 140, UP_y2 - 115, UP_x2, UP_y2 - 65, Wh, TRUE);
	DrawBoxAA(UP_x2 - 140, UP_y2 - 115, UP_x2, UP_y2 - 65, Bk, FALSE);
	DrawTriangle(UP_x2 - 140, UP_y2 - 85, UP_x2 - 140, UP_y2 - 75, UP_x2 - 155, UP_y2 - 80, Bk, TRUE);
	DrawFormatString(UP_x2 - 135, UP_y2 - 115, Bk, "%s", mypokemon[MyPokemonNumber].pokemon.name);

	//HP�o�[
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

	//HP�̎c��\��
	SetFontSize(14);
	DrawFormatString(UP_x2 - 70, UP_y2 - 80, Bk, "%4d / %4d", mypokemon[MyPokemonNumber].pokemon.HP, mypokemon[MyPokemonNumber].pokemon.MAXHP);

	ScreenFlip();// ����ʃf�[�^��\��ʂ֔��f
	SetDrawScreen(DX_SCREEN_FRONT); // �`���𗠉�ʂɐݒ�

}

unsigned int  ColorChange(int ColorNumber) {
	unsigned int Color;

	switch (ColorNumber) {
		case �m�[�}��:
			Color = Normal;
			break;

		case ��:
			Color = Fire;
			break;

		case ��:
			Color = Water;
			break;

		case ��:
			Color = Grass;
			break;

		case ��:
			Color = Light;
			break;

		case ��:
			Color = Dark;
			break;

	}


	return Color;
}

void UpInitScreen(MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	DrawBoxAA(UP_x1, UP_y1 + 190, UP_x2, UP_y2, Bk, TRUE);	//�e�L�X�g��
	DrawBoxAA(UP_x1 + 5, UP_y1 + 190 + 5, UP_x2 - 5, UP_y2 - 5, Wh, TRUE);	//�e�L�X�g��

}
