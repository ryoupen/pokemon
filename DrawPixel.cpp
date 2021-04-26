//�w�b�_�[�t�@�C��--------------------------------------------------------------------------------------------------------------------
#include "DxLib.h"
#include <math.h>
#include <time.h>

//�F�w��(���ʁj-----------------------------------------------------------------------------------------------------------------------
unsigned int Bk, Bl, Wh, Gl, Gr, Dr, Lg, Dg, Ld, Lr, Lb, Db, LLb, Pi, Ly, Yl;
#define Wh GetColor(255, 255, 255) //��
#define Bk GetColor(0, 0, 0)       //��
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

//�|�P�����̐�
#define PokeNum 10
//�����|�P����
#define MAXPokemon 6

//�J�[�\���i���W�j
int MouseX, MouseY, MouseInput, LogType;

//����
enum Element{�m�[�}��,��,��,��,��,��};

//�Z
enum Move{�Ȃ�,�ւ񂵂�,���傤�͂�,�L�b�N,�r���h�A�b�v,�邬�̂܂�,�\�[���[�r�[��, �͂��σJ�b�^�[, ����ɂ�����, ���Ȃ��ق�,
			�X�y�V�E��,�X�N���C�h,�w�u���~�b�N�X,�G���u���C�X,�u���X,�����������ǂ�,���̂܂�};

//�|�P�����\���̐錾
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

//�v���g�^�C�v�錾--------------------------------------------------------------------------------------------------------------------
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

//��L���ʂ̂��ߕύX���Ȃ�------------------------------------------------------------------------------------------------------------



// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//������ʁi�ύX���Ȃ��j------------------------------------------------------------------
	SetMainWindowText("Nintendo DS Lite");
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�ŋN��
	SetGraphMode(650, 700, 32); //��ʂ̉𑜓x�w��@�@
	SetBackgroundColor(255, 255, 255);
	SetWindowSizeChangeEnableFlag(FALSE); //��ʃT�C�Y�ύX�s��

	// �c�w���C�u��������������---------------------------------------------------------------
	if (DxLib_Init() == -1)
	{
		return -1;    // �G���[���N�����璼���ɏI��
	}

	//�N����----------------------------------------------------------------------------------
	PlaySoundFile("DS�N����.mp3", DX_PLAYTYPE_BACK);

	//��ʏ�����------------------------------------------------------------------------------
	ScreenReset();

	//�|�P��������
	Pokemon pokemon[PokeNum];
	PokemonCreate(pokemon);

	MyPokemon mypokemon[MAXPokemon] = { pokemon[1],pokemon[2],pokemon[4],0,0,0 };
	EnemyPokemon enemypokemon[MAXPokemon] = { pokemon[0],pokemon[3],0,0,0,0 };

	//��ʑI��(�������(1))
	int flg = 1;

	//�퓬
	while(1) {

		//flg=0�̎��A�퓬�I��
		if (flg == 0 or enemypokemon[0].pokemon.HP <= 0) {
			SetFontSize(180);
			SetFontThickness(9);
			ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
			DrawString(0, 60, "���Ȃ���", Bk);
			DrawString(0, 430, "�����ł�", Bk);
			break;
		}

		//flg=1�̎��A���1�Ɉړ�
		else if (flg == 1) {
			Screen1();
			flg = Select1(flg, mypokemon, enemypokemon);
		}

		//flg=2�̎��A���2�Ɉړ�
		else if (flg == 2) {
			Screen2();
			flg = Select2(flg, mypokemon, enemypokemon);
		}

		//flg=3�̎��A���3�Ɉړ�
		else if (flg == 3) {
			Screen3(mypokemon);
			flg = Select3(flg, mypokemon, enemypokemon);
		}

	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
	return 0; 
}

void ScreenReset() {

	//�O�g�`��--------------------------------------------------------------
	DrawBoxAA(0, 0, 650, 700, Bk, FALSE);

	//���ʕ`��------------------------------------------------------------
	DrawBoxAA(150, 30, 500, 300, Bk, FALSE);	//���ʊO�g
	LoadGraphScreen(UP_x1, UP_y1, "�r�[�`.jpg", FALSE);
	LoadGraphScreen(UP_x1+30, UP_y1+100, "�r���I��.png",TRUE);
	LoadGraphScreen(UP_x1 + 100, UP_y1 + 90, "�߂��.png", TRUE);
	LoadGraphScreen(UP_x1 + 200, UP_y1 + 30, "���ڂ��.png", TRUE);
	DrawBoxAA(UP_x1, UP_y1, UP_x2, UP_y2, Bk, FALSE);	//���ʓ��g

	//�X�s�[�J�[
	int i, j, x, y;
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

void PokemonCreate(Pokemon* pokemon) {
	pokemon[0] = { "���^����",�m�[�}��,255,255,255,255,255,�ւ񂵂�,�Ȃ�,�Ȃ�,�Ȃ�,0,0,0,0 };
	pokemon[1] = { "�S�[���h�V�b�v",�m�[�}��,255,255,200,200,255,���傤�͂�,�L�b�N,�r���h�A�b�v,�邬�̂܂�,0,0,0,0 };
	pokemon[2] = { "���ڂ��",��,200,200,130,220,30,�\�[���[�r�[��,�͂��σJ�b�^�[,����ɂ�����,���Ȃ��ق�,0,0,0,0 };
	pokemon[3] = { "�r���I��", ��,150,150,180,200,80,�X�y�V�E��,�X�N���C�h,�w�u���~�b�N�X,�G���u���C�X,0,0,0,0 };
	pokemon[4] = { "�I���|��", ��,500,500,120,80,50,�L�b�N,�u���X,�����������ǂ�,���̂܂�,0,0,0,0 };
}


//���1�`�揈��--------------------------------------------------------------------------------------------------------
void Screen1() {

	//����ʓ��g
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
	LoadGraphScreen(165, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(190, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(215, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(240, 405, "�{�[��.png", TRUE);
	LoadGraphScreen(265, 405, "�{�[��.png", TRUE);
	LoadGraphScreen(290, 405, "�{�[��.png", TRUE);
	//�E�����X�^�[�{�[���G���W
	LoadGraphScreen(395, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(410, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(425, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(440, 405, "�{�[���E��.png", TRUE);
	LoadGraphScreen(455, 405, "�{�[���E��.png", TRUE);
	LoadGraphScreen(470, 405, "�{�[���E��.png", TRUE);

	//����ʓ��g
	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 500, "��������", Wh);
	DrawString(295, 610, "�|�P����", Wh);

}

//���2�`�揈��--------------------------------------------------------------------------------------------------------
void Screen2() {

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Wh,TRUE);	//����ʓ��g
	
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
	//�E���ǂ���W
	DrawBoxAA(160, 600, 490, 650, Lb, TRUE);
	DrawBoxAA(160, 600, 490, 650, Bk, FALSE);
	DrawBoxAA(170, 605, 480, 645, Db, TRUE);

	//�E�����X�^�[�{�[���������W
	LoadGraphScreen(165, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(190, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(215, 405, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(240, 405, "�{�[��.png", TRUE);
	LoadGraphScreen(265, 405, "�{�[��.png", TRUE);
	LoadGraphScreen(290, 405, "�{�[��.png", TRUE);
	//�E�����X�^�[�{�[���G���W
	LoadGraphScreen(395, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(410, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(425, 405, "�����X�^�[�{�[���E��.png", TRUE);
	LoadGraphScreen(440, 405, "�{�[���E��.png", TRUE);
	LoadGraphScreen(455, 405, "�{�[���E��.png", TRUE);
	LoadGraphScreen(470, 405, "�{�[���E��.png", TRUE);

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Gl, FALSE);	//����ʓ��g

	SetFontSize(22);
	SetFontThickness(9);
	ChangeFont("Yu Gothic UI", DX_CHARSET_DEFAULT);
	DrawString(295, 610, " ���ǂ� ", Wh);

}

//���3�`�揈��--------------------------------------------------------------------------------------------------------
void Screen3(MyPokemon* mypokemon) {

	DrawBoxAA(DW_x1, DW_y1, DW_x2, DW_y2, Ld, TRUE);	//����ʓ��g

	//�E�|�P����1�̖ڍ����W
	DrawBoxAA(160, 410, 323, 470, LLb, TRUE);
	//�E�|�P����2�̖ڍ����W
	DrawBoxAA(160, 475, 323, 535, LLb, TRUE);
	//�E�|�P����3�̖ڍ����W
	DrawBoxAA(160, 540, 323, 600, LLb, TRUE);

	//�E�|�P����1�̖ډE���W
	DrawBoxAA(327, 420, 490, 480, LLb, TRUE);
	//�E�|�P����2�̖ډE���W
	DrawBoxAA(327, 485, 490, 545, LLb, TRUE);
	//�E�|�P����3�̖ډE���W
	DrawBoxAA(327, 550, 490, 610, LLb, TRUE);

	//�E�����X�^�[�{�[�����W
	LoadGraphScreen(160, 410, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(160, 475, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(160, 540, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(327, 420, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(327, 485, "�����X�^�[�{�[��.png", TRUE);
	LoadGraphScreen(327, 550, "�����X�^�[�{�[��.png", TRUE);
	//�|�P�������O
	DrawString(205, 415, mypokemon[0].pokemon.name, Wh);
	DrawString(205, 480, mypokemon[2].pokemon.name, Wh);
	DrawString(205, 545, mypokemon[4].pokemon.name, Wh);
	DrawString(372, 425, mypokemon[1].pokemon.name, Wh);
	DrawString(372, 490, mypokemon[3].pokemon.name, Wh);
	DrawString(372, 555, mypokemon[5].pokemon.name, Wh);
	//�|�P����HP
	DrawFormatString(220, 445, Wh, "%d / %d", mypokemon[0].pokemon.HP, mypokemon[0].pokemon.MAXHP);
	DrawFormatString(220, 510, Wh, "%d / %d", mypokemon[2].pokemon.HP, mypokemon[2].pokemon.MAXHP);
	DrawFormatString(220, 575, Wh, "%d / %d", mypokemon[4].pokemon.HP, mypokemon[4].pokemon.MAXHP);
	DrawFormatString(387, 455, Wh, "%d / %d", mypokemon[1].pokemon.HP, mypokemon[1].pokemon.MAXHP);
	DrawFormatString(387, 520, Wh, "%d / %d", mypokemon[3].pokemon.HP, mypokemon[3].pokemon.MAXHP);
	DrawFormatString(387, 585, Wh, "%d / %d", mypokemon[5].pokemon.HP, mypokemon[5].pokemon.MAXHP);



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
				}

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					cnt = 1;
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
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
			Screen1();
			DrawBoxAA(180, 450, 470, 580, Yl, FALSE);
		}

		else if (cursor == 2) {
			Screen1();
			DrawBoxAA(160, 600, 490, 650, Yl, FALSE);
		}

		//������
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;
		Sleep(100);

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

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
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
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���Z2��������
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 435 && MouseY <= 510) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���Z3��������
			else if (MouseX >= 165 && MouseX <= 520 && MouseY >= 520 && MouseY <= 595) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���Z4��������
			else if (MouseX >= 330 && MouseX <= 485 && MouseY >= 520 && MouseY <= 595) {
				TurnCheck(flg, mypokemon, enemypokemon);
				cnt = 1;
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

		//������
		MouseX = 0;
		MouseY = 0;
		MouseInput = 0;
		LogType = 0;
		Sleep(100);

	}

	return flg;

}

int Select3(int flg, MyPokemon* mypokemon, EnemyPokemon* enemypokemon) {

	//������
	MouseX = 0;
	MouseY = 0;
	MouseInput = 0;
	LogType = 0;

	//�J�[�\���ʒu(1)
	int cursor = 1;

	while (1) {

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

				//�����A���̍��W��B�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (513 - y) && MouseY <= (513 + y)) {
					flg = 1;
					cnt = 1;
				}

				//�����A���̍��W��X�{�^���̈ʒu��������
				else if (MouseX >= (575 - x) && MouseX <= (575 + x) && MouseY >= (435 - y) && MouseY <= (435 + y)) {
					cnt = 1;
				}

				//�����A���̍��W��Y�{�^���̈ʒu��������
				else if (MouseX >= (536 - x) && MouseX <= (536 + x) && MouseY >= (474 - y) && MouseY <= (474 + y)) {
					cnt = 1;
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
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���|�P����2��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 420 && MouseY <= 480) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���|�P����3��������
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 475 && MouseY <= 535) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���|�P����4��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 485 && MouseY <= 545) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���|�P����5��������
			else if (MouseX >= 160 && MouseX <= 323 && MouseY >= 540 && MouseY <= 600) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W���|�P����6��������
			else if (MouseX >= 327 && MouseX <= 490 && MouseY >= 550 && MouseY <= 610) {
				PokemonChange(mypokemon, enemypokemon);
				cnt = 1;
			}

			//�����A���̍��W�����ǂ邾������
			else if (MouseX >= 410 && MouseX <= 490 && MouseY >= 620 && MouseY <= 650) {
				flg = 1;
				cnt = 1;
			}

			if (cnt == 1) {
				break;
			}

		}

		//�J�[�\���ړ�
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

		//������
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