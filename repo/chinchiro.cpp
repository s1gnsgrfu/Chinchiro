#include<iostream>
#include<random>
#include<time.h>
#include<stdlib.h>
#include<string>
using namespace std;

#define MONEY 50000//最初の所持金
#define KAKE_MIN 100//賭け金の最小

class Gnuplot {
	FILE* gnuplot;
	float deltTime;
	float playTime;
public:
	Gnuplot();
	~Gnuplot();
	void SetTitle(const char* title);//タイトルの設定
	void SetXLabel(const char* label);//X軸ラベルの設定
	void SetYLabel(const char* label);//Y軸ラベルの設定
	void SetXRange(int lower, int higher);//X軸の範囲を設定
	void SetYRange(int lower, int higher);//Y軸の範囲を設定
	void PlotBegin(const char* str);//プロットの開始
	void PlotEnd();//プロットの終了
	void AddPlotData(float x, float y);//プロットデータの追加
};

class Human {
	string name;//名前
	int suuti[3];//振ったサイコロの目
	int yaku;//役の判別
	int win;//勝利数
	int lose;//敗北数
	int money;//所持金
	int oyako;//親or子判別 1:親 0:子
	int contin;//0:役あり 1:役なし 2:即負け 3:即勝ち 4:引き分け 5:通常負け 6:通常勝ち

public:
	Human();//メンバーの初期化
	void SetName(string s) { name = s; };//名前を設定
	string GetName() { return name; };//名前を取得
	void SetYaku(int i) { yaku = i; };//yakuを設定
	int GetYaku() { return yaku; };//yakuを取得
	void SetContin(int i) { contin = i; }//continを設定
	int GetContin() { return contin; };//continを取得
	void SetMoney(int m) { money = m; };//moneyを設定
	int GetMoney() { return money; };//moneyを取得
	void SetOyako(int i) { oyako = i; };//oyakoを設定
	int GetOyako() { return oyako; };//oyakoを取得
	void IncWin() { win++; };//勝利数をインクリメント
	void IncLose() { lose++; };//敗北数をインクリメント
	int GetWin() { return win; };//勝利数の取得
	void ShowInfo();//自分の情報を表示
	int MoveMoney(int);//賭け金を反映
	void OyakoTyusen();//親or子を抽選
	void RandDi(int, int);//3回サイコロを振る
	void Idet_yaku();//役を判別
	void ShowResult();//勝負結果を表示
	friend ostream& operator<<(ostream& stream, Human ob);//さいころを表示
};

class Cpu :public Human {
	string cpuname[3] = { "BOB","DIO","JOJO" };//cpuの名前の候補
public:
	Cpu() :Human() {};//メンバーの初期化
	void SetCpuName(int i) { SetName(string(cpuname[i])); };//cpuの名前を設定
};

class Game {
	int game_count;//ゲーム回数

public:
	Game();//メンバーの初期化
	void ShowRule();//ルールの表示

};

Game::Game() {
	game_count = 0;
}

Human::Human() {
	//初期化
	for (int i = 0; i < 3; i++) {
		suuti[i] = 0;
	}
	yaku = win = lose = money = contin = 0;
	oyako = contin = 0;
}

Gnuplot::Gnuplot() {
	deltTime = playTime = 0;
	if ((gnuplot = _popen("gnuplot -persist", "w")) == NULL) {
		cout << "ファイルが見つかりません" << endl;
		exit(1);
	}
}

Gnuplot::~Gnuplot() {
	fprintf(gnuplot, "exit\n");//gnuplotの終了
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
	cout << "チンチロリンで賭博しましょう！" << endl << endl;
	cout << "■遊び方" << endl;
	cout << "1.お金を賭けましょう" << endl;
	cout << "2.ターンになったらサイコロを3つ振りましょう" << endl;
	cout << "3.サイコロの出た目で対決します" << endl << endl;
	cout << "■ルール" << endl;
	cout << "       -----------------------------------------------" << endl;
	cout << "　  　 |名称　　　　出目　　　　配当　　　　　 備考　|" << endl;
	cout << "-------|---------------------------------------------|" << endl;
	cout << "|　強　|ピンゾロ　　1 1 1　　　 5倍もらう　　　即勝ち|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|   |　|　　　　　　2 2 2　　　　　　　 　　 　　　　|" << endl;
	cout << "|   |　|　　　　　　3 3 3　　　　　　　 　　　 　　　|" << endl;
	cout << "|   |　|ゾロ目　　　4 4 4　　　 3倍もらう　　　即勝ち|" << endl;
	cout << "|   |　|　　　　　　5 5 5　　　　　　　 　　　　 　　|" << endl;
	cout << "|   |　|　　　　　　6 6 6　　　　　　　 　　　　　 　|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|　 |　|シゴロ　　　4 5 6　　 　2倍もらう　　　即勝ち|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|   |　|　　　　　　2 2 6　　　　　　　 　　　　　　 |" << endl;
	cout << "|   |　|　　　　　　2 2 5　　　　　　　 　　　　　　 |" << endl;
	cout << "|   |　|通常目　　　2 2 4　　　 出した分もらう       |" << endl;
	cout << "|   |　|　　　　　　2 2 3　　　　　　　 　　　　　　 |" << endl;
	cout << "|   |　|　　　　　　3 3 2　　　　　　　 　　　　　　 |" << endl;
	cout << "|   |　|　　　　　　2 2 1　　　　　　　 　　　　　　 |" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|　 |　|役なし　　　1 5 6など　 出した分払う   即負け|" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|　 |　|ションベン　丼から　　　出した分払う　 即負け|" << endl;
	cout << "|　 |　|　　　　　　こぼれる　　　　　　　　　　　　 |" << endl;
	cout << "|   |  |---------------------------------------------|" << endl;
	cout << "|　 弱 |ヒフミ　　　1 2 3　　　 2倍払う　　　　即負け|" << endl;
	cout << "------------------------------------------------------" << endl;
}

void Human::ShowInfo() {
	cout << "■" << GetName() << endl;
	cout << "所持金:" << money << "円 勝利数:" << win << " 敗北数:" << lose << endl;
}

ostream& operator<<(ostream& stream, Human ob) {
	string dis[7] = {
	{"  |      @     |"}, //[0]--サイコロ1
	{"  | @          |"}, //[1]--サイコロ2_1
	{"  |          @ |"}, //[2]--サイコロ2_2
	{"  | @        @ |"}, //[3]--サイコロ4
	{"  --------------"}, //[4]--サイコロ上下枠
	{"  |            |"}, //[5]--サイコロ縦枠
	{"                "}  //[6]--空白
	};
	int i = 0, c = 0;
	string dis_tmp[7];//サイコロ表示用一時変数

	for (c = 0; c < 3; c++) {//さいころ3つ分
		if (ob.suuti[c] > 0 && ob.suuti[c] <= 7) {//想定内の数値の場合
			if (ob.suuti[c] == 7) {//ションベンの表示
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
				dis_tmp[0] += dis[4];//サイコロ上下枠を代入(上)
			}

			switch (ob.suuti[c])
			{
			case 1://目が1のさいころ表示
				for (i = 1; i < 6; i++) {
					if (i == 3) {
						dis_tmp[i] += dis[0];
					}
					else {
						dis_tmp[i] += dis[5];
					}
				}
				break;
			case 2://目が2のさいころ表示
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
			case 3://目が3のさいころ表示
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
			case 4://目が4のさいころ表示
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
			case 5://目が5のさいころ表示
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
			case 6://目が6のさいころ表示
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
				dis_tmp[6] += dis[4];//サイコロ上下枠を代入(下)
			}
		}
	}

	for (i = 0; i < 7; i++) {
		stream << dis_tmp[i] << endl;//サイコロの7行を代入
	}
	return stream;
}

void Human::Idet_yaku() {
	sort(begin(suuti), end(suuti));//昇順にsuutiをソート

	if (suuti[2] >= 7) {//ションベン
		cout << "  [---------------- ションベン ----------------]" << endl;
		yaku = 2;
		contin = 2;

	}
	else  if (suuti[0] == 1 && suuti[1] == 1 && suuti[2] == 1) {//ピンゾロ
		cout << "  [----------------- ピンゾロ -----------------]" << endl;
		yaku = 12;
		contin = 3;
	}
	else if (suuti[0] == suuti[1] == suuti[2]) {//ぞろ目
		cout << "  [----------------- " << suuti[0] << "のゾロ目 ----------------]" << endl;
		yaku = 11;
		contin = 3;
	}
	else if (suuti[0] == 4 && suuti[1] == 5 && suuti[2] == 6) {//シゴロ
		cout << "  [------------------ シゴロ ------------------]" << endl;
		yaku = 10;
		contin = 3;
	}
	else if (suuti[0] == suuti[1]) {//通常目--役の数値が大きい場合
		cout << "  [------------------ " << suuti[2] << "の目 -------------------]" << endl;
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
	else if (suuti[1] == suuti[2]) {//通常目--役の数値が小さい場合
		cout << "  [------------------ " << suuti[0] << "の目 -------------------]" << endl;
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
	else if (suuti[0] == 1 && suuti[1] == 2 && suuti[2] == 3) {//ヒフミ
		cout << "  [------------------ ヒフミ ------------------]" << endl;
		yaku = 1;
		contin = 2;
	}
	else {//役なし
		cout << "  [------------------ 役なし ------------------]" << endl;
		yaku = 3;
		contin = 1;
	}
}

void Human::OyakoTyusen() {//親か子を決める(原則1試合に1回)
	srand(time(NULL));
	oyako = (rand() % 2);//0か1の乱数
	//1:親 0:子
}

void Human::RandDi(int add, int add2) {
	srand(time(NULL) + add + add2);//乱数の固定化対策
	int randtmp;
	int sum = 0;

	for (int i = 0; i < 3; i++) {
		randtmp = (1 + rand() % 7);//1から7までの乱数

		if (randtmp == 7) {//ションベンの確率を下げる
			for (int i = 0; i < 10; i++) {//乱数を再び10個
				sum += (1 + rand() % 7);//1から7までの乱数
			}
			int roundsum = (sum / 10 + 0.5);//sumの平均をとって四捨五入
			if (roundsum >= 7) {//7以上になったらションベン
				suuti[i] = 7;
			}
			else {
				suuti[i] = roundsum;
			}
		}
		else {//1から6の場合
			suuti[i] = randtmp;
		}
	}
	// 7はサイコロの目にないが，ションベン用に抽選する
}

void Human::ShowResult() {
	string result;

	cout << endl;
	switch (GetContin())
	{
	case 0://役ありだった場合
		cout << "  =-=-=-=-=-=-=-=-= ターン交代 =-=-=-=-=-=-=-=-=";
		break;
	case 2://即負けだった場合
	case 3://即勝ちだった場合
	case 4://引き分けだった場合
	case 5://通常負けだった場合
	case 6://通常勝ちだった場合
		result = GetName();

		if (result.length() % 2 != 0) {//名前の長さが奇数だったら(半角対策)
			result += " ";
		}

		if (GetContin() == 2 || GetContin() == 5) {//負け
			result += "の負け";
		}
		else if (GetContin() == 3 || GetContin() == 6) {//勝ち
			result += "の勝ち";
		}
		else {
			result = "引き分け";
		}

		for (int i = 0; i < 3; i++) {//枠の表示
			for (int j = 0; j < result.length() / 2 + 4; j++) {
				if (i == 0) {
					if (j == 0 || j == result.length() / 2 + 3) {
						cout << "＿";
					}
					else {
						cout << "人";
					}
				}
				else if (i == 1) {
					cout << "＞　" << result << "　＜";
					break;
				}
				else {
					if (j == 0 || j == result.length() / 2 + 3) {
						cout << "￣";
					}
					else {
						cout << "Ｙ";
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
	case 12://ピンゾロ
		kakegaku *= 5;
		break;
	case 11://ゾロ目
		kakegaku *= 3;
		break;
	case 10://シゴロ
		kakegaku *= 2;
		break;
	case 3://役なし
	case 2://ションベン
		kakegaku *= -1;
		break;
	case 1://ヒフミ
		kakegaku *= -2;
		break;
	}
	//通常目は賭け金分(1倍)もらうため何もしない

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

	srand(time(NULL));//乱数が固定にならないように設定

	game.ShowRule();//ルールの表示

	cout << endl << "プレイヤー名を入力してください" << endl << ">>";
	getline(cin, nametmp);
	person01.SetName(nametmp);//名前の設定

	tmp = rand() % 3;//0から2までの乱数
	cpu.SetCpuName(tmp);//cpuのプレイヤー名の設定

	cout << endl << "ゲームを開始します．よければ任意の文字を入力してください．" << endl;
	while (input_c == ' ') {//文字入力を待機
		cout << ">>";
		cin >> input_c;
		cin.clear();//エラーフラグクリア
		cin.ignore(256, '\n');//バッファクリア
	}
	cout << endl;

	//情報表示
	person01.ShowInfo();
	cout << endl;
	cpu.ShowInfo();

	//Gnuplot
	gp.SetTitle("所持金の推移");
	gp.SetXLabel("ゲーム数 [回]");
	gp.SetYLabel("所持金 [円]");
	gp.PlotBegin("with lines lw 3 lt 1");
	gp.AddPlotData(game_cnt, person01.GetMoney());

	gp2.SetTitle("勝利数の推移");
	gp2.SetXLabel("ゲーム数 [回]");
	gp2.SetYLabel("勝利数 [回]");
	gp2.PlotBegin("with lines lw 3 lt 2");
	gp2.AddPlotData(game_cnt, person01.GetWin());

	//ゲーム開始
	while (game_contin == 1) {
		game_cnt++;

		cout << endl << "ゲーム回数:" << game_cnt << endl;

		person01.OyakoTyusen();//親or子を決める

		if (person01.GetOyako() != 1) {//person01が子の時
			cout << cpu.GetName() << "が親です" << endl;

			cout << endl << "現在の所持金：" << person01.GetMoney() << "円" << endl;
			cout << "賭け額はいくらですか 範囲:" << KAKE_MIN << "円～" << person01.GetMoney() / 5 << "円" << endl << ">>";
			cin >> kakegaku;
			cin.clear();//エラーフラグクリア
			cin.ignore(256, '\n');//バッファクリア

			while (1) {//適正値を入力するまでループ
				if (kakegaku * 5 > person01.GetMoney() || kakegaku < KAKE_MIN) {//賭け額の5倍が所持金より大きいまたは100円より小さい場合
					cout << ">>";
					cin >> kakegaku;
					cin.clear();//エラーフラグクリア
					cin.ignore(256, '\n');//バッファクリア
				}
				else {
					break;
				}
			}
		}
		else {
			cout << person01.GetName() << "が親です" << endl;
			kakegaku = (100 + (rand() % 50) * 100);//100から5000まで100刻みの乱数
			cout << endl << "賭け額:" << kakegaku << "円" << endl;
		}

		//2回目以降の試合用に初期化
		cntd = 0;//サイコロを振った回数
		turn_cnt = 0;
		p_yaku = c_yaku = 0;
		person01.SetYaku(0);
		cpu.SetYaku(0);
		person01.SetContin(0);
		cpu.SetContin(0);

		while (cntd <= 3) {//サイコロ振った回数が最大回数以下の場合
			cntd++;
			//サイコロ振る
			if (person01.GetOyako() == 1) {//person01が親だった場合
				if (cntd == 1) {
					cout << endl << "■" << person01.GetName() << "のターン" << endl;
					person01.SetContin(1);
				}

				if (person01.GetContin() == 1) {//役がない場合
					if (cntd - 1 == 3) {//振り直し回数がない場合
						person01.SetContin(2);//person01を負け判定にする
						cpu.SetContin(3);//cpuを勝ち判定にする
					}
					else {//振り直し回数がある場合
						person01.RandDi(rand(), game_cnt);//さいころを振る
						cout << person01;
						person01.Idet_yaku();
					}
				}
			}
			else {//cpuが親だった場合
				if (cntd == 1) {
					cout << endl << "■" << cpu.GetName() << "のターン" << endl;
					cpu.SetContin(1);
				}

				if (cpu.GetContin() == 1) {//役がない場合
					if (cntd - 1 == 3) {//振り直し回数がない場合
						cpu.SetContin(2);//cpuを負け判定にする
						person01.SetContin(3);//person01を勝ち判定にする
					}
					else {//振り直し回数がある場合
						cpu.RandDi(rand(), game_cnt);//さいころを振る
						cout << cpu;
						cpu.Idet_yaku();
					}

				}
			}

			if (3 - cntd >= 1) {//最大振り直し回数より小さい場合
				if (person01.GetOyako() == 1) {//person01が親だった場合
					if (person01.GetContin() == 1) {//役なしだった場合
						cout << endl << "振り直し" << endl << endl;
					}
					else {//役ありだったら終わらせる
						cntd = 4;
					}
				}
				else {//cpuが親だった場合
					if (cpu.GetContin() == 1) {//役なしだった場合
						cout << endl << cpu.GetName() << "は振り直した" << endl << endl;
					}
				}
			}

			if (cntd > 3) {//最後のループ（3回サイコロ振った後）の場合
				if (person01.GetOyako() == 1) {//person01が親だった場合
					switch (person01.GetContin())
					{
					case 0://通常目だった場合
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
					case 1://即勝ちor即負けの場合
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
						cntd = 4;//試合を終了させる
						break;
					}
				}
				else {//cpuが親だった場合
					switch (cpu.GetContin())
					{
					case 0://通常目だった場合
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
					case 1://即勝ちor即負けの場合
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
						cntd = 4;//試合を終了させる
						break;
					}
				}
			}
		}

		//それぞれのyakuを取得して変数へ代入
		p_yaku = person01.GetYaku();
		c_yaku = cpu.GetYaku();

		if (p_yaku <= 9 && p_yaku >= 4 && c_yaku <= 9 && c_yaku >= 4) {//通常目だった場合
			if (p_yaku > c_yaku) {//出目がcpuよりperson01のほうが大きい場合
				person01.SetContin(6);
				cpu.SetContin(5);
			}
			else if (p_yaku < c_yaku) {//出目がperson01よりcpuのほうが大きい場合
				person01.SetContin(5);
				cpu.SetContin(6);
			}
			else {//引き分けの場合
				person01.SetContin(4);
				cpu.SetContin(4);
			}
		}

		//cpuが親だった場合に正しく賭け金を処理させる
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

		//通常の負けor勝ちにおいて精算
		if (person01.GetOyako() == 1) {
			switch (person01.GetContin())
			{
			case 5://負け
				kakegaku *= -1;
				break;
			case 4://引き分け
				kakegaku = 0;
				person01.MoveMoney(kakegaku);
				cpu.MoveMoney(kakegaku);
				break;
			}

			if (person01.GetContin() != 4) {//負けor勝ちの時
				cpu.MoveMoney(person01.MoveMoney(kakegaku));
			}
		}
		else {
			switch (cpu.GetContin())
			{
			case 5://負け
				kakegaku *= -1;
				break;
			case 4://引き分け
				kakegaku = 0;
				person01.MoveMoney(kakegaku);
				cpu.MoveMoney(kakegaku);
				break;
			}

			if (cpu.GetContin() != 4) {//負けor勝ちの時
				person01.MoveMoney(cpu.MoveMoney(kakegaku));
			}
		}

		switch (person01.GetContin())//勝敗数記録
		{
		case 2://負け
		case 5:
			person01.IncLose();
			cpu.IncWin();
			break;
		case 3://勝ち
		case 6:
			person01.IncWin();
			cpu.IncLose();
			break;
		}

		//情報表示
		person01.ShowInfo();
		cout << endl;
		cpu.ShowInfo();

		//Gnuplot
		gp.AddPlotData(game_cnt, person01.GetMoney());
		gp2.AddPlotData(game_cnt, person01.GetWin());

		if (person01.GetMoney() < 5000 || cpu.GetMoney() < 5000) {//所持金が5000円を切ったら
			cout << endl << "所持金が少ないため，ゲームを終了します．" << endl;
			game_contin = 0;
		}
		else {//ゲームを続けるか否か
			input_c = ' ';
			cout << endl << "ゲームを続けますか？ y/n" << endl;
			while (input_c != 'y' && input_c != 'n') {//yまたはn以外の入力を無視
				cout << ">>";
				cin >> input_c;
				cin.clear();//エラーフラグクリア
				cin.ignore(256, '\n');//バッファクリア
			}

			if (input_c == 'y') {//yだったら続行
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

