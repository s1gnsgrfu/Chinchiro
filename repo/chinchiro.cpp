#include<iostream>
#include<random>
#include<time.h>
#include<stdlib.h>
#include<string>
using namespace std;

#define MONEY 50000//�ŏ��̏�����
#define KAKE_MIN 100//�q�����̍ŏ�

class Gnuplot {
	FILE* gnuplot;
	float deltTime;
	float playTime;
public:
	Gnuplot();
	~Gnuplot();
	void SetTitle(const char* title);//�^�C�g���̐ݒ�
	void SetXLabel(const char* label);//X�����x���̐ݒ�
	void SetYLabel(const char* label);//Y�����x���̐ݒ�
	void SetXRange(int lower, int higher);//X���͈̔͂�ݒ�
	void SetYRange(int lower, int higher);//Y���͈̔͂�ݒ�
	void PlotBegin(const char* str);//�v���b�g�̊J�n
	void PlotEnd();//�v���b�g�̏I��
	void AddPlotData(float x, float y);//�v���b�g�f�[�^�̒ǉ�
};

class Human {
	string name;//���O
	int suuti[3];//�U�����T�C�R���̖�
	int yaku;//���̔���
	int win;//������
	int lose;//�s�k��
	int money;//������
	int oyako;//�eor�q���� 1:�e 0:�q
	int contin;//0:������ 1:���Ȃ� 2:������ 3:������ 4:�������� 5:�ʏ핉�� 6:�ʏ폟��

public:
	Human();//�����o�[�̏�����
	void SetName(string s) { name = s; };//���O��ݒ�
	string GetName() { return name; };//���O���擾
	void SetYaku(int i) { yaku = i; };//yaku��ݒ�
	int GetYaku() { return yaku; };//yaku���擾
	void SetContin(int i) { contin = i; }//contin��ݒ�
	int GetContin() { return contin; };//contin���擾
	void SetMoney(int m) { money = m; };//money��ݒ�
	int GetMoney() { return money; };//money���擾
	void SetOyako(int i) { oyako = i; };//oyako��ݒ�
	int GetOyako() { return oyako; };//oyako���擾
	void IncWin() { win++; };//���������C���N�������g
	void IncLose() { lose++; };//�s�k�����C���N�������g
	int GetWin() { return win; };//�������̎擾
	void ShowInfo();//�����̏���\��
	int MoveMoney(int);//�q�����𔽉f
	void OyakoTyusen();//�eor�q�𒊑I
	void RandDi(int, int);//3��T�C�R����U��
	void Idet_yaku();//���𔻕�
	void ShowResult();//�������ʂ�\��
	friend ostream& operator<<(ostream& stream, Human ob);//���������\��
};

class Cpu :public Human {
	string cpuname[3] = { "BOB","DIO","JOJO" };//cpu�̖��O�̌��
public:
	Cpu() :Human() {};//�����o�[�̏�����
	void SetCpuName(int i) { SetName(string(cpuname[i])); };//cpu�̖��O��ݒ�
};

class Game {
	int game_count;//�Q�[����

public:
	Game();//�����o�[�̏�����
	void ShowRule();//���[���̕\��

};

Game::Game() {
	game_count = 0;
}

Human::Human() {
	//������
	for (int i = 0; i < 3; i++) {
		suuti[i] = 0;
	}
	yaku = win = lose = money = contin = 0;
	oyako = contin = 0;
}

Gnuplot::Gnuplot() {
	deltTime = playTime = 0;
	if ((gnuplot = _popen("gnuplot -persist", "w")) == NULL) {
		cout << "�t�@�C����������܂���" << endl;
		exit(1);
	}
}

Gnuplot::~Gnuplot() {
	fprintf(gnuplot, "exit\n");//gnuplot�̏I��
	_pclose(gnuplot);
}

void Gnuplot::SetTitle(const char* title) {
	fprintf(gnuplot, "set title '%s'\n", title);
}

void Gnuplot::SetXLabel(const char* label) {
	fprintf(gnuplot, "set xlabel '%s'\n", label);
}

void Gnuplot::SetYLabel(const char* label) {
	fprintf(gnuplot, "set ylabel '%s'\n", label);
}

void Gnuplot::SetXRange(int lower, int higher) {
	fprintf(gnuplot, "set xrange [%d:%d]\n", lower, higher);
}

void Gnuplot::SetYRange(int lower, int higher) {
	fprintf(gnuplot, "set yrange [%d:%d]\n", lower, higher);
}

void Gnuplot::PlotBegin(const char* str) {
	fprintf(gnuplot, "plot '-' %s\n", str);
}

void Gnuplot::AddPlotData(float x, float y) {
	fprintf(gnuplot, "%f\t%f\n", x, y);
}

void Gnuplot::PlotEnd() {
	fprintf(gnuplot, "e\n");
}

void Game:: ShowRule() {
	cout << "�`���`�������œq�����܂��傤�I" << endl << endl;
	cout << "���V�ѕ�" << endl;
	cout << "1.������q���܂��傤" << endl;
	cout << "2.�^�[���ɂȂ�����T�C�R����3�U��܂��傤" << endl;
	cout << "3.�T�C�R���̏o���ڂőΌ����܂�" << endl << endl;
	cout << "�����[��" << endl;
	cout << "       -----------------------------------------------" << endl;
	cout << "�@  �@ |���́@�@�@�@�o�ځ@�@�@�@�z���@�@�@�@�@ ���l�@|" << endl;
	cout << "-------|---------------------------------------------|" << endl;
	cout << "|�@���@|�s���]���@�@1 1 1�@�@�@ 5�{���炤�@�@�@������|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|   |�@|�@�@�@�@�@�@2 2 2�@�@�@�@�@�@�@ �@�@ �@�@�@�@|" << endl;
	cout << "|   |�@|�@�@�@�@�@�@3 3 3�@�@�@�@�@�@�@ �@�@�@ �@�@�@|" << endl;
	cout << "|   |�@|�]���ځ@�@�@4 4 4�@�@�@ 3�{���炤�@�@�@������|" << endl;
	cout << "|   |�@|�@�@�@�@�@�@5 5 5�@�@�@�@�@�@�@ �@�@�@�@ �@�@|" << endl;
	cout << "|   |�@|�@�@�@�@�@�@6 6 6�@�@�@�@�@�@�@ �@�@�@�@�@ �@|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|�@ |�@|�V�S���@�@�@4 5 6�@�@ �@2�{���炤�@�@�@������|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|   |�@|�@�@�@�@�@�@2 2 6�@�@�@�@�@�@�@ �@�@�@�@�@�@ |" << endl;
	cout << "|   |�@|�@�@�@�@�@�@2 2 5�@�@�@�@�@�@�@ �@�@�@�@�@�@ |" << endl;
	cout << "|   |�@|�ʏ�ځ@�@�@2 2 4�@�@�@ �o���������炤       |" << endl;
	cout << "|   |�@|�@�@�@�@�@�@2 2 3�@�@�@�@�@�@�@ �@�@�@�@�@�@ |" << endl;
	cout << "|   |�@|�@�@�@�@�@�@3 3 2�@�@�@�@�@�@�@ �@�@�@�@�@�@ |" << endl;
	cout << "|   |�@|�@�@�@�@�@�@2 2 1�@�@�@�@�@�@�@ �@�@�@�@�@�@ |" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|�@ |�@|���Ȃ��@�@�@1 5 6�Ȃǁ@ �o����������   ������|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|�@ |�@|�V�����x���@������@�@�@�o�����������@ ������|" << endl;
	cout << "|�@ |�@|�@�@�@�@�@�@���ڂ��@�@�@�@�@�@�@�@�@�@�@�@ |" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|�@ �� |�q�t�~�@�@�@1 2 3�@�@�@ 2�{�����@�@�@�@������|" << endl;
	cout << "------------------------------------------------------" << endl;
}

void Human::ShowInfo() {
	cout << "��" << GetName() << endl;
	cout << "������:" << money << "�~ ������:" << win << " �s�k��:" << lose << endl;
}

ostream& operator<<(ostream& stream, Human ob) {
	string dis[7] = {
	{"  |      @     |"}, //[0]--�T�C�R��1
	{"  | @          |"}, //[1]--�T�C�R��2_1
	{"  |          @ |"}, //[2]--�T�C�R��2_2
	{"  | @        @ |"}, //[3]--�T�C�R��4
	{"  --------------"}, //[4]--�T�C�R���㉺�g
	{"  |            |"}, //[5]--�T�C�R���c�g
	{"                "}  //[6]--��
	};
	int i = 0, c = 0;
	string dis_tmp[7];//�T�C�R���\���p�ꎞ�ϐ�

	for (c = 0; c < 3; c++) {//��������3��
		if (ob.suuti[c] > 0 && ob.suuti[c] <= 7) {//�z����̐��l�̏ꍇ
			if (ob.suuti[c] == 7) {//�V�����x���̕\��
				for (i = 0; i < 7; i++) {
					if (i == 3) {
						dis_tmp[i] += dis[4];
					}
					else {
						dis_tmp[i] += dis[6];
					}
				}
			}
			else {
				dis_tmp[0] += dis[4];//�T�C�R���㉺�g����(��)
			}

			switch (ob.suuti[c])
			{
			case 1://�ڂ�1�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 3) {
						dis_tmp[i] += dis[0];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 2://�ڂ�2�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 1) {
						dis_tmp[i] += dis[1];
					}
					else if (i == 5) {
						dis_tmp[i] += dis[2];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 3://�ڂ�3�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 1) {
						dis_tmp[i] += dis[1];
					}
					else if (i == 5) {
						dis_tmp[i] += dis[2];
					}
					else if (i == 3) {
						dis_tmp[i] += dis[0];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 4://�ڂ�4�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 1) {
						dis_tmp[i] += dis[3];
					}
					else if (i == 5) {
						dis_tmp[i] += dis[3];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 5://�ڂ�5�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 1 || i == 5) {
						dis_tmp[i] += dis[3];
					}
					else if (i == 3) {
						dis_tmp[i] += dis[0];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 6://�ڂ�6�̂�������\��
				for (i = 1; i < 6; i++) {
					if (i == 1 || i == 5 || i == 3) {
						dis_tmp[i] += dis[3];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			}
			if (ob.suuti[c] != 7) {
				dis_tmp[6] += dis[4];//�T�C�R���㉺�g����(��)
			}
		}
	}

	for (i = 0; i < 7; i++) {
		stream << dis_tmp[i] << endl;//�T�C�R����7�s����
	}
	return stream;
}

void Human::Idet_yaku() {
	sort(begin(suuti), end(suuti));//������suuti���\�[�g

	if (suuti[2] >= 7) {//�V�����x��
		cout << "  [---------------- �V�����x�� ----------------]" << endl;
		yaku = 2;
		contin = 2;

	}
	else  if (suuti[0] == 1 && suuti[1] == 1 && suuti[2] == 1) {//�s���]��
		cout << "  [----------------- �s���]�� -----------------]" << endl;
		yaku = 12;
		contin = 3;
	}
	else if (suuti[0] == suuti[1] == suuti[2]) {//�����
		cout << "  [----------------- " << suuti[0] << "�̃]���� ----------------]" << endl;
		yaku = 11;
		contin = 3;
	}
	else if (suuti[0] == 4 && suuti[1] == 5 && suuti[2] == 6) {//�V�S��
		cout << "  [------------------ �V�S�� ------------------]" << endl;
		yaku = 10;
		contin = 3;
	}
	else if (suuti[0] == suuti[1]) {//�ʏ��--���̐��l���傫���ꍇ
		cout << "  [------------------ " << suuti[2] << "�̖� -------------------]" << endl;
		switch (suuti[2])
		{
		case 6:
			yaku = 9;
			break;
		case 5:
			yaku = 8;
			break;
		case 4:
			yaku = 7;
			break;
		case 3:
			yaku = 6;
			break;
		case 2:
			yaku = 5;
			break;
		case 1:
			yaku = 4;
			break;
		}
		contin = 0;
	}
	else if (suuti[1] == suuti[2]) {//�ʏ��--���̐��l���������ꍇ
		cout << "  [------------------ " << suuti[0] << "�̖� -------------------]" << endl;
		switch (suuti[0])
		{
		case 6:
			yaku = 9;
			break;
		case 5:
			yaku = 8;
			break;
		case 4:
			yaku = 7;
			break;
		case 3:
			yaku = 6;
			break;
		case 2:
			yaku = 5;
			break;
		case 1:
			yaku = 4;
			break;
		}
		contin = 0;
	}
	else if (suuti[0] == 1 && suuti[1] == 2 && suuti[2] == 3) {//�q�t�~
		cout << "  [------------------ �q�t�~ ------------------]" << endl;
		yaku = 1;
		contin = 2;
	}
	else {//���Ȃ�
		cout << "  [------------------ ���Ȃ� ------------------]" << endl;
		yaku = 3;
		contin = 1;
	}
}

void Human::OyakoTyusen() {//�e���q�����߂�(����1������1��)
	srand(time(NULL));
	oyako = (rand() % 2);//0��1�̗���
	//1:�e 0:�q
}

void Human::RandDi(int add, int add2) {
	srand(time(NULL) + add + add2);//�����̌Œ艻�΍�
	int randtmp;
	int sum = 0;

	for (int i = 0; i < 3; i++) {
		randtmp = (1 + rand() % 7);//1����7�܂ł̗���

		if (randtmp == 7) {//�V�����x���̊m����������
			for (int i = 0; i < 10; i++) {//�������Ă�10��
				sum += (1 + rand() % 7);//1����7�܂ł̗���
			}
			int roundsum = (sum / 10 + 0.5);//sum�̕��ς��Ƃ��Ďl�̌ܓ�
			if (roundsum >= 7) {//7�ȏ�ɂȂ�����V�����x��
				suuti[i] = 7;
			}
			else {
				suuti[i] = roundsum;
			}
		}
		else {//1����6�̏ꍇ
			suuti[i] = randtmp;
		}
	}
	// 7�̓T�C�R���̖ڂɂȂ����C�V�����x���p�ɒ��I����
}

void Human::ShowResult() {
	string result;

	cout << endl;
	switch (GetContin())
	{
	case 0://�����肾�����ꍇ
		cout << "  =-=-=-=-=-=-=-=-= �^�[����� =-=-=-=-=-=-=-=-=";
		break;
	case 2://�������������ꍇ
	case 3://�������������ꍇ
	case 4://���������������ꍇ
	case 5://�ʏ핉���������ꍇ
	case 6://�ʏ폟���������ꍇ
		result = GetName();

		if (result.length() % 2 != 0) {//���O�̒��������������(���p�΍�)
			result += " ";
		}

		if (GetContin() == 2 || GetContin() == 5) {//����
			result += "�̕���";
		}
		else if (GetContin() == 3 || GetContin() == 6) {//����
			result += "�̏���";
		}
		else {
			result = "��������";
		}

		for (int i = 0; i < 3; i++) {//�g�̕\��
			for (int j = 0; j < result.length() / 2 + 4; j++) {
				if (i == 0) {
					if (j == 0 || j == result.length() / 2 + 3) {
						cout << "�Q";
					}
					else {
						cout << "�l";
					}
				}
				else if (i == 1) {
					cout << "���@" << result << "�@��";
					break;
				}
				else {
					if (j == 0 || j == result.length() / 2 + 3) {
						cout << "�P";
					}
					else {
						cout << "�x";
					}
				}
			}
			cout << endl;
		}
		break;
	}

	cout << endl;
}

int Human::MoveMoney(int kakegaku) {
	switch (yaku)
	{
	case 12://�s���]��
		kakegaku *= 5;
		break;
	case 11://�]����
		kakegaku *= 3;
		break;
	case 10://�V�S��
		kakegaku *= 2;
		break;
	case 3://���Ȃ�
	case 2://�V�����x��
		kakegaku *= -1;
		break;
	case 1://�q�t�~
		kakegaku *= -2;
		break;
	}
	//�ʏ�ڂ͓q������(1�{)���炤���߉������Ȃ�

	money += kakegaku;
	return -kakegaku;
}

int main(void) {

	Game game;
	Human person01;
	Cpu cpu;
	int kakegaku, cntd, tmp, turn_cnt, game_contin = 1, game_cnt = 0;
	int p_yaku, c_yaku;
	char input_c = ' ';
	string nametmp;

	Gnuplot gp, gp2;

	person01.SetMoney(MONEY);
	cpu.SetMoney(MONEY);

	srand(time(NULL));//�������Œ�ɂȂ�Ȃ��悤�ɐݒ�

	game.ShowRule();//���[���̕\��

	cout << endl << "�v���C���[������͂��Ă�������" << endl << ">>";
	getline(cin, nametmp);
	person01.SetName(nametmp);//���O�̐ݒ�

	tmp = rand() % 3;//0����2�܂ł̗���
	cpu.SetCpuName(tmp);//cpu�̃v���C���[���̐ݒ�

	cout << endl << "�Q�[�����J�n���܂��D�悯��ΔC�ӂ̕�������͂��Ă��������D" << endl;
	while (input_c == ' ') {//�������͂�ҋ@
		cout << ">>";
		cin >> input_c;
		cin.clear();//�G���[�t���O�N���A
		cin.ignore(256, '\n');//�o�b�t�@�N���A
	}
	cout << endl;

	//���\��
	person01.ShowInfo();
	cout << endl;
	cpu.ShowInfo();

	//Gnuplot
	gp.SetTitle("�������̐���");
	gp.SetXLabel("�Q�[���� [��]");
	gp.SetYLabel("������ [�~]");
	gp.PlotBegin("with lines lw 3 lt 1");
	gp.AddPlotData(game_cnt, person01.GetMoney());

	gp2.SetTitle("�������̐���");
	gp2.SetXLabel("�Q�[���� [��]");
	gp2.SetYLabel("������ [��]");
	gp2.PlotBegin("with lines lw 3 lt 2");
	gp2.AddPlotData(game_cnt, person01.GetWin());

	//�Q�[���J�n
	while (game_contin == 1) {
		game_cnt++;

		cout << endl << "�Q�[����:" << game_cnt << endl;

		person01.OyakoTyusen();//�eor�q�����߂�

		if (person01.GetOyako() != 1) {//person01���q�̎�
			cout << cpu.GetName() << "���e�ł�" << endl;

			cout << endl << "���݂̏������F" << person01.GetMoney() << "�~" << endl;
			cout << "�q���z�͂�����ł��� �͈�:" << KAKE_MIN << "�~�`" << person01.GetMoney() / 5 << "�~" << endl << ">>";
			cin >> kakegaku;
			cin.clear();//�G���[�t���O�N���A
			cin.ignore(256, '\n');//�o�b�t�@�N���A

			while (1) {//�K���l����͂���܂Ń��[�v
				if (kakegaku * 5 > person01.GetMoney() || kakegaku < KAKE_MIN) {//�q���z��5�{�����������傫���܂���100�~��菬�����ꍇ
					cout << ">>";
					cin >> kakegaku;
					cin.clear();//�G���[�t���O�N���A
					cin.ignore(256, '\n');//�o�b�t�@�N���A
				}
				else {
					break;
				}
			}
		}
		else {
			cout << person01.GetName() << "���e�ł�" << endl;
			kakegaku = (100 + (rand() % 50) * 100);//100����5000�܂�100���݂̗���
			cout << endl << "�q���z:" << kakegaku << "�~" << endl;
		}

		//2��ڈȍ~�̎����p�ɏ�����
		cntd = 0;//�T�C�R����U������
		turn_cnt = 0;
		p_yaku = c_yaku = 0;
		person01.SetYaku(0);
		cpu.SetYaku(0);
		person01.SetContin(0);
		cpu.SetContin(0);

		while (cntd <= 3) {//�T�C�R���U�����񐔂��ő�񐔈ȉ��̏ꍇ
			cntd++;
			//�T�C�R���U��
			if (person01.GetOyako() == 1) {//person01���e�������ꍇ
				if (cntd == 1) {
					cout << endl << "��" << person01.GetName() << "�̃^�[��" << endl;
					person01.SetContin(1);
				}

				if (person01.GetContin() == 1) {//�����Ȃ��ꍇ
					if (cntd - 1 == 3) {//�U�蒼���񐔂��Ȃ��ꍇ
						person01.SetContin(2);//person01�𕉂�����ɂ���
						cpu.SetContin(3);//cpu����������ɂ���
					}
					else {//�U�蒼���񐔂�����ꍇ
						person01.RandDi(rand(), game_cnt);//���������U��
						cout << person01;
						person01.Idet_yaku();
					}
				}
			}
			else {//cpu���e�������ꍇ
				if (cntd == 1) {
					cout << endl << "��" << cpu.GetName() << "�̃^�[��" << endl;
					cpu.SetContin(1);
				}

				if (cpu.GetContin() == 1) {//�����Ȃ��ꍇ
					if (cntd - 1 == 3) {//�U�蒼���񐔂��Ȃ��ꍇ
						cpu.SetContin(2);//cpu�𕉂�����ɂ���
						person01.SetContin(3);//person01����������ɂ���
					}
					else {//�U�蒼���񐔂�����ꍇ
						cpu.RandDi(rand(), game_cnt);//���������U��
						cout << cpu;
						cpu.Idet_yaku();
					}

				}
			}

			if (3 - cntd >= 1) {//�ő�U�蒼���񐔂�菬�����ꍇ
				if (person01.GetOyako() == 1) {//person01���e�������ꍇ
					if (person01.GetContin() == 1) {//���Ȃ��������ꍇ
						cout << endl << "�U�蒼��" << endl << endl;
					}
					else {//�����肾������I��点��
						cntd = 4;
					}
				}
				else {//cpu���e�������ꍇ
					if (cpu.GetContin() == 1) {//���Ȃ��������ꍇ
						cout << endl << cpu.GetName() << "�͐U�蒼����" << endl << endl;
					}
				}
			}

			if (cntd > 3) {//�Ō�̃��[�v�i3��T�C�R���U������j�̏ꍇ
				if (person01.GetOyako() == 1) {//person01���e�������ꍇ
					switch (person01.GetContin())
					{
					case 0://�ʏ�ڂ������ꍇ
						person01.SetOyako(0);
						cntd = 0;
						if (turn_cnt == 0) {
							person01.ShowResult();
						}
						else if (turn_cnt == 1) {
							person01.SetOyako(1);
							cntd = 4;
						}
						turn_cnt++;
						break;
					case 1://������or�������̏ꍇ
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
						cntd = 4;//�������I��������
						break;
					}
				}
				else {//cpu���e�������ꍇ
					switch (cpu.GetContin())
					{
					case 0://�ʏ�ڂ������ꍇ
						person01.SetOyako(1);
						cntd = 0;
						if (turn_cnt == 0) {
							person01.ShowResult();
						}
						else if (turn_cnt == 1) {
							person01.SetOyako(0);
							cntd = 4;
						}
						turn_cnt++;
						break;
					case 1://������or�������̏ꍇ
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
						cntd = 4;//�������I��������
						break;
					}
				}
			}
		}

		//���ꂼ���yaku���擾���ĕϐ��֑��
		p_yaku = person01.GetYaku();
		c_yaku = cpu.GetYaku();

		if (p_yaku <= 9 && p_yaku >= 4 && c_yaku <= 9 && c_yaku >= 4) {//�ʏ�ڂ������ꍇ
			if (p_yaku > c_yaku) {//�o�ڂ�cpu���person01�̂ق����傫���ꍇ
				person01.SetContin(6);
				cpu.SetContin(5);
			}
			else if (p_yaku < c_yaku) {//�o�ڂ�person01���cpu�̂ق����傫���ꍇ
				person01.SetContin(5);
				cpu.SetContin(6);
			}
			else {//���������̏ꍇ
				person01.SetContin(4);
				cpu.SetContin(4);
			}
		}

		//cpu���e�������ꍇ�ɐ������q����������������
		if (person01.GetOyako() != 1) {
			switch (cpu.GetContin())
			{
			case 2:
				person01.SetContin(3);
				break;
			case 3:
				person01.SetContin(2);
				break;
			}
		}
		person01.ShowResult();

		//�ʏ�̕���or�����ɂ����Đ��Z
		if (person01.GetOyako() == 1) {
			switch (person01.GetContin())
			{
			case 5://����
				kakegaku *= -1;
				break;
			case 4://��������
				kakegaku = 0;
				person01.MoveMoney(kakegaku);
				cpu.MoveMoney(kakegaku);
				break;
			}

			if (person01.GetContin() != 4) {//����or�����̎�
				cpu.MoveMoney(person01.MoveMoney(kakegaku));
			}
		}
		else {
			switch (cpu.GetContin())
			{
			case 5://����
				kakegaku *= -1;
				break;
			case 4://��������
				kakegaku = 0;
				person01.MoveMoney(kakegaku);
				cpu.MoveMoney(kakegaku);
				break;
			}

			if (cpu.GetContin() != 4) {//����or�����̎�
				person01.MoveMoney(cpu.MoveMoney(kakegaku));
			}
		}

		switch (person01.GetContin())//���s���L�^
		{
		case 2://����
		case 5:
			person01.IncLose();
			cpu.IncWin();
			break;
		case 3://����
		case 6:
			person01.IncWin();
			cpu.IncLose();
			break;
		}

		//���\��
		person01.ShowInfo();
		cout << endl;
		cpu.ShowInfo();

		//Gnuplot
		gp.AddPlotData(game_cnt, person01.GetMoney());
		gp2.AddPlotData(game_cnt, person01.GetWin());

		if (person01.GetMoney() < 5000 || cpu.GetMoney() < 5000) {//��������5000�~��؂�����
			cout << endl << "�����������Ȃ����߁C�Q�[�����I�����܂��D" << endl;
			game_contin = 0;
		}
		else {//�Q�[���𑱂��邩�ۂ�
			input_c = ' ';
			cout << endl << "�Q�[���𑱂��܂����H y/n" << endl;
			while (input_c != 'y' && input_c != 'n') {//y�܂���n�ȊO�̓��͂𖳎�
				cout << ">>";
				cin >> input_c;
				cin.clear();//�G���[�t���O�N���A
				cin.ignore(256, '\n');//�o�b�t�@�N���A
			}

			if (input_c == 'y') {//y�������瑱�s
				game_contin = 1;
				cout << endl << endl;
			}
			else {
				game_contin = 0;
			}
		}
	}

	//Gnuplot
	gp.PlotEnd();
	gp2.PlotEnd();

	return 0;
}

