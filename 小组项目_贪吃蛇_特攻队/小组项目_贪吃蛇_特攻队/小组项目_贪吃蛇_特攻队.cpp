#include <iostream>
#include <time.h>//随机种子来出现食物和道具
#include<Windows.h>//为了使用gotoxy（光标移动函数）
#include<conio.h>//监听键盘
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int direct = 'a';//蛇的初始方向
int number = 10;//每次加的分数
int score = 0;//总分
int num = 0;//道具的判定
string filename;//难度文件替代
string player;//玩家

#define ROW 30//游戏区行数
#define COL 80//游戏区列数
#define MOVE_CENTER 20//整体页面的移动
#define WALL "■"//墙的图标
#define SNAKE_HEAD "◎"//蛇头
#define SNAKE_BODY "●"//蛇身
#define	PROP "⭐"//道具

//账户结构体
struct ScoreAccount {
	int score;
	int account;
};

string filenameuser = "users.txt";

//蛇的结构体
struct
{
	//记录每一节蛇的坐标
	//蛇的最大结数
	int x[ROW * COL];
	int y[ROW * COL];
	int len;//蛇的长度
	int speed;//蛇的移动速度
}snake;

//食物的结构体
struct
{
	int x;
	int y;
}food;

//函数声明：
void menuView1();

// 登录成功后跳转的界面
void menuView2();

void createMap();//地图创建

void createFoodorprop();//食物和道具的创建

void initSnake();//初始化

void moveSnake();//蛇的移动

int gameover(int num);//游戏结束

void nanduxuanzeView();

void youxibangzhuView();

void paihangbangView(string filename);

void paihangbang_nanduxuanzeView();

void inPaihangbangFile(string filename, int score, int account);


void gameoverView(int score, int account, string filename);

void Hide();

void gameView();

// 注册功能，将用户名和密码存入文件
bool registerUser(const string& filename, const string& username, const string& password);

// 登录功能，从文件读取用户名和密码，进行匹配验证
bool loginUser(const string& filename, const string& username, const string& password);

// 忘记密码功能，允许用户重置密码
bool resetPassword(const string& filename, const string& username, const string& newPassword);

// 检查用户名是否已存在
bool cz(const string& filename, const string& username);

// 选择登陆注册忘记密码函数，提供用户界面选择功能
void mainMenu(const string& filename);

int main() {
	menuView1();
	return 0;
}

//额外加的函数
//光标跳转
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//光标隐藏
void Hide()
{
	CONSOLE_CURSOR_INFO c;
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);

}

void menuView1() {
	while (1) {
		system("cls");

		gotoxy(53, 4);
		cout << "贪吃蛇大战";
		gotoxy(53, 9);
		cout << "登录游戏 1";
		gotoxy(53, 11);
		cout << "游戏帮助 2";
		gotoxy(53, 13);
		cout << "退出游戏 3";

		gotoxy(53, 15);
		cout << "请选择：";
		int choice;
		cin >> choice;

		while (1)
		{
			system("cls");
			switch (choice)
			{
			case 1:
				// 调用登陆注册忘记密码函数
				mainMenu(filenameuser);
				break;
			case 2:
				youxibangzhuView();
				break;
			case 3:
				//退出游戏
				exit(0);
				break;
			default:
				while (1) {
					system("cls");
					gotoxy(48, 11);
					cout << "无效的输入，请重新输入\n";
					cin >> choice;
					if (choice == 1 || choice == 2) {
						break;
					}
				}
				break;
			}
		}
	}
}

// 登录成功后的功能菜单界面
void menuView2() {
	int choice;

	while (true)
	{
		system("cls");
		gotoxy(53, 4);
		cout << "1. 开始游戏" << endl;
		gotoxy(53, 6);
		cout << "2. 排行榜" << endl;
		gotoxy(53, 8);
		cout << "3. 返回首页" << endl;
		gotoxy(53, 10);
		cout << "请选择：";

		cin >> choice;

		switch (choice) {
		case 1:
			gotoxy(48, 11);
			cout << "开始游戏..." << endl;
			nanduxuanzeView();
			break;
		case 2:
			//cout << "显示排行榜..." << endl;
			paihangbang_nanduxuanzeView();
			break;
		case 3:
			gotoxy(48, 11);
			cout << "返回首页..." << endl;
			menuView1();
		default:
			gotoxy(48, 11);
			cout << "无效选择，请重试。" << endl;
		}
	}
}

// 登录功能，从文件读取用户名和密码，进行匹配验证
bool loginUser(const string& filename, const string& username, const string& password) {
	ifstream inFile(filename);
	string storedUsername, storedPassword;

	if (inFile.is_open()) {
		// 从文件中读取用户名和密码
		while (inFile >> storedUsername >> storedPassword) {
			// 对比文件中的用户名和密码是否匹配
			if (storedUsername == username && storedPassword == password) {
				gotoxy(53, 11);
				cout << "登录成功！" << endl;
				gotoxy(53, 13);
				cout << "请稍等....";
				Sleep(1000);
				inFile.close();
				player = username;
				//account = username;
				// 登录成功后跳转到新的界面
				menuView2();
				return true;
			}
		}
		inFile.close();
	}
	system("cls");
	gotoxy(50, 11);
	cout << "用户名或密码错误。" << endl;
	gotoxy(50, 12);
	cout << "稍等返回重新登录";
	Sleep(1000);
	return false;
}

// 注册功能，将用户名和密码存入文件
bool registerUser(const string& filename, const string& username, const string& password) {
	if (cz(filename, username)) {
		gotoxy(48, 11);
		cout << "用户名已存在，请使用其他用户名。" << endl;
		Sleep(1000);
		return false;
	}

	ofstream outFile(filename, ios::app);
	if (outFile.is_open()) {
		outFile << username << " " << password << endl;
		outFile.close();
		gotoxy(53, 11);
		cout << "注册成功！" << endl;
		Sleep(1000);
		return true;
	}
	else {
		gotoxy(43, 11);
		cout << "无法打开文件。" << endl;
		Sleep(1000);
		return false;
	}
}

// 忘记密码功能，允许用户重置密码
bool resetPassword(const string& filename, const string& username, const string& newPassword) {
	ifstream inFile(filename);
	ofstream tempFile("temp.txt");
	string storedUsername, storedPassword;
	bool found = false;

	if (inFile.is_open() && tempFile.is_open()) {
		while (inFile >> storedUsername >> storedPassword) {
			if (storedUsername == username) {
				tempFile << storedUsername << " " << newPassword << endl; // 更新密码
				found = true;
			}
			else {
				tempFile << storedUsername << " " << storedPassword << endl; // 复制其他用户信息
			}
		}
		inFile.close();
		tempFile.close();

		remove(filename.c_str());
		rename("temp.txt", filename.c_str());

		if (found) {
			gotoxy(53, 11);
			cout << "密码重置成功！" << endl;
			Sleep(1000);
			return true;
		}
		else {
			gotoxy(53, 11);
			cout << "用户名不存在。" << endl;
			Sleep(1000);
			return false;
		}
	}
	else {
		gotoxy(53, 11);
		cout << "无法打开文件。" << endl;
		Sleep(1000);
		return false;
	}
}

// 检查用户名是否已存在
bool cz(const string& filename, const string& username) {
	ifstream inFile(filename);
	string storedUsername, storedPassword;

	if (inFile.is_open()) {
		while (inFile >> storedUsername >> storedPassword) {
			if (storedUsername == username) {
				inFile.close();
				return true;
			}
		}
		inFile.close();
	}

	return false;
}

// 选择登陆注册忘记密码函数，负责用户界面和选择功能
void mainMenu(const string& filename) {
	int choice;
	string username, password, newPassword;

	while (true)
	{
		system("cls");
		gotoxy(53, 5);
		cout << "  1.登录" << endl;
		gotoxy(53, 7);
		cout << "  2.注册" << endl;
		gotoxy(53, 9);
		cout << "3.忘记密码" << endl;
		gotoxy(53, 11);
		cout << "请选择：";

		cin >> choice;
		system("cls");
		switch (choice) {
		case 1:  // 登录
			gotoxy(53, 6);
			cout << "请输入用户名: ";
			cin >> username;
			gotoxy(53, 8);
			cout << "请输入密码: ";
			cin >> password;
			loginUser(filename, username, password);
			break;
		case 2:  // 注册
			gotoxy(53, 6);
			cout << "请输入用户名: ";
			cin >> username;
			gotoxy(53, 8);
			cout << "请输入密码: ";
			cin >> password;
			registerUser(filename, username, password);
			break;
		case 3:  // 忘记密码
			gotoxy(53, 6);
			cout << "请输入用户名: ";
			cin >> username;
			gotoxy(53, 8);
			cout << "请输入新密码: ";
			cin >> newPassword;
			resetPassword(filename, username, newPassword);
			break;
		default:
			gotoxy(48, 11);
			cout << "无效选择，请重试。" << endl;
		}
	}
}


void nanduxuanzeView() {
	while (1)
	{
		system("cls");
		gotoxy(53, 4);
		cout << "难度选择" << endl;
		gotoxy(53, 6);
		cout << "1 简单" << endl;
		gotoxy(53, 8);
		cout << "2 普通" << endl;
		gotoxy(53, 10);
		cout << "3 困难" << endl;
		gotoxy(53, 12);
		cout << "4 噩梦" << endl;
		gotoxy(53, 14);
		cout << "请选择：";


		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			//简单
			snake.speed = 300;
			filename = "eaxy.txt";
			gameView();
			break;
		case 2:
			//普通
			snake.speed = 200;
			filename = "ordinary.txt";
			gameView();
			break;
		case 3:
			//困难
			snake.speed = 100;
			filename = "difficulty.txt";
			gameView();
			break;
		case 4:
			//噩梦
			snake.speed = 50;
			filename = "hell.txt";
			gameView();
			break;
		default:
			while (1) {
				system("cls");
				cout << "无效的输入，请重新输入\n";
				cin >> choice;
				if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
					break;
				}
			}
			break;
		}
	}
}


void youxibangzhuView()
{
	while (1) {
		system("cls");
		gotoxy(53, 4);
		cout << "游戏规则";
		gotoxy(40, 6);
		cout << "1.使用WASD控制蛇的移动。";
		cout << "W↑ S↓ A← D→";
		gotoxy(40, 8);
		cout << "2.蛇吃到食物会变长，吃到一个食物加10分。";
		gotoxy(40, 10);
		cout << "3.按q返回开始界面。";
		gotoxy(53, 15);
		cout << "输入q返回主页面";
		char ch;
		ch = getchar();
		if (ch == 'q')
		{
			menuView1();
		}
	}
}

void inPaihangbangFile(string filename, int score, string account) {
	ofstream fout;
	fout.open(filename, ios::app);
	if (!fout.is_open()) {
		cout << "创建文件写入失败" << endl;
		return;
	}
	// 写入分数和账号
	fout << score << " " << account << endl;
	fout.close();
}

void paihangbangView(string filename)
{
	system("cls");
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open())
	{
		gotoxy(40, 10);
		cout << "打开排行榜文件失败" << endl;
		gotoxy(40, 11);
		system("pause");
		return;
	}

	vector<ScoreAccount> scores;
	int score;
	int account;
	while (fin >> score >> account) {
		scores.push_back({ score, account });
	}
	fin.close();

	// 排序，分数高的在前
	sort(scores.begin(), scores.end(), [](const ScoreAccount& a, const ScoreAccount& b) {
		return a.score > b.score;
		});

	// 显示前十名
	gotoxy(48, 4);
	cout << "排行榜前十名：" << endl;
	for (int i = 0; i < min(10, static_cast<int>(scores.size())); ++i)
	{
		gotoxy(48, 6 + i);
		cout << "账号: " << scores[i].account << " 分数: " << scores[i].score << endl;

	}
	gotoxy(48, 20);
	system("pause");

}

void paihangbang_nanduxuanzeView() {
	system("cls");
	gotoxy(44, 7);
	cout << "请选择你要查看那个难度的排行榜" << endl;
	gotoxy(44, 9);
	cout << "1.简单.2.普通.3.困难.4.噩梦" << endl;
	gotoxy(52, 14);
	cout << "请选择：";
	int i;
	cin >> i;
	string filenameEaxy = "eaxy.txt";
	string filenameOrdinary = "ordinary.txt";
	string filenameDifficulty = "difficulty.txt";
	string filenameHell = "hell.txt";
	switch (i)
	{
	case 1:
		paihangbangView(filenameEaxy);
		break;
	case 2:
		paihangbangView(filenameOrdinary);
		break;
	case 3:
		paihangbangView(filenameDifficulty);
		break;
	case 4:
		paihangbangView(filenameHell);
		break;
	default:
		cout << "输入错误" << endl;
	}
}

void gameover_paihangbangView(string filename) {
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin.is_open()) {
		cout << "打开排行榜文件失败" << endl;
		return;
	}

	vector<ScoreAccount> scores;
	int score;
	int account;
	while (fin >> score >> account) {
		scores.push_back({ score, account });
	}
	fin.close();

	// 排序，分数高的在前
	sort(scores.begin(), scores.end(), [](const ScoreAccount& a, const ScoreAccount& b) {
		return a.score > b.score;
		});

	// 显示前三名
	for (int i = 0; i < min(3, static_cast<int>(scores.size())); ++i)
	{
		gotoxy(44, 12 + i);
		cout << "第" << i + 1 << "名: " << "账号 : " << scores[i].account << " 分数 : " << scores[i].score << endl;
	}
}

void gameoverView(int score, string account, string filename)
{
	system("cls");

	inPaihangbangFile(filename, score, account);

	gotoxy(53, 4);

	printf("游 戏 结 束");

	gotoxy(50, 6);
	printf("您的得分是：%3d分", score);


	gotoxy(53, 8);
	printf("1.再来一局");
	gotoxy(53, 10);
	printf("2.返回主菜单");

	//文件流操作，展示排行榜前三
	gotoxy(44, 12);
	gameover_paihangbangView(filename);

	gotoxy(54, 20);
	printf("请选择(1-2)");
	int n;
	cin >> n;
	while (1)
	{
		switch (n)
		{
		case 1:
			system("cls");
			nanduxuanzeView();
			break;
		case 2:
			system("cls");
			menuView2();
			break;
		default:
			system("cls");
			gotoxy(53, 10);

			cout << "请重新选择";
			Sleep(1000);
			gameoverView(score, account, filename);
		}
	}
}

void initSnake() {
	snake.len = 3;//蛇初始化长度
	//蛇初始化位置
	snake.x[0] = COL / 2 + MOVE_CENTER;
	snake.y[0] = ROW / 2;
	gotoxy(snake.x[0], snake.y[0]);
	cout << SNAKE_HEAD;
	//用循环打印出蛇身，蛇身街道蛇头后面
	for (int i = 1; i < snake.len; i++) {
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		cout << SNAKE_BODY;
	}
	//3.画食物
	//3.1确定坐标
	srand((unsigned int)time(NULL)); //随机函数种子
	food.x = rand() % (COL - 5) + 2 + MOVE_CENTER;
	food.y = rand() % (ROW - 2) + 1;
	//3.2根据坐标画出食物
	gotoxy(food.x, food.y);
	cout << WALL;
	//改变光标位置使其不影响美观
	gotoxy(102, 0);
	//cout << "得分";
}


int gameover(int num) {//判断当前的游戏状态
	//判断是否撞墙
	if (num != 6) {//判断是否使用了无敌道具
		//判断是否撞墙
		if (snake.x[0] == 0 + MOVE_CENTER || snake.x[0] == COL + MOVE_CENTER || snake.y[0] == 0 || snake.y[0] == ROW - 1) {
			return 1;
		}
		//判断是否吃到自己
		for (int i = 1; i < snake.len; i++) {
			if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
				return 1;
			}
		}
	}
	return 0;
}

void createMap() {
	system("mode con cols=120 lines=40");
	for (int i = 0 + MOVE_CENTER; i < COL + MOVE_CENTER; i += 2) {
		gotoxy(i, 0);//最上面的一条边，改变x，y不变
		cout << WALL;
		gotoxy(i, ROW - 1);//最下面的一条边，改变x，y不变
		cout << WALL;
	}
	for (int i = 0; i < ROW; i++) {
		gotoxy(0 + MOVE_CENTER, i);//最左面的一条边，改变y,x不变
		cout << WALL;
		gotoxy(COL + MOVE_CENTER, i);//最右面的一条边，改变y,x不变
		cout << WALL;
	}
	initSnake();
}

void moveSnake() {
	if (_kbhit()) {//监听键盘输入
		fflush(stdin);
		direct = _getch();
	}
	//擦除最后一节蛇尾
	gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
	cout << "  ";

	//开始替换坐标，移动蛇
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		cout << SNAKE_BODY;
	}
	//蛇移动的方向
	switch (direct) {
	case'w':
	case'W':
		snake.y[0]--;
		break;
	case's':
	case'S':
		snake.y[0]++;
		break;
	case'a':
	case'A':
		snake.x[0] -= 2;
		break;
	case'd':
	case'D':
		snake.x[0] += 2;
		break;
	}
	//移动之后新蛇头的位置
	gotoxy(snake.x[0], snake.y[0]);
	cout << SNAKE_HEAD;
	//改变光标位置使其不影响美观
	gotoxy(102, 1);
	cout << "得分";
	cout << score;
}

void createFoodorprop() {
	//蛇把食物吃了
	if (snake.x[0] == food.x && snake.y[0] == food.y)
	{
		if (num < 6) {//蛇吃到了食物
			snake.len++;
			score += number;
			gotoxy(102, 2);
			cout << "蛇吃到了食物     ";
		}
		else {
			switch (num) {
			case 6:
				gotoxy(102, 2);
				cout << "使用了无敌道具    ";
				break;
			case 7:
				snake.len++;
				gotoxy(102, 2);
				cout << "使用了身体加长道具";
				break;
			case 8:
				snake.len--;
				gotoxy(102, 2);
				cout << "使用了身体减短道具";
				break;
			case 9:
				number *= 2;
				score += number;
				gotoxy(102, 2);
				cout << "使用了双倍得分道具";
				break;
			}
		}
		while (1)
		{
			int flag1 = 1;  //标记食物出现是否合适
			//随机产生食物位置
			srand((unsigned int)time(NULL));
			food.x = rand() % (COL - 5) + 2 + MOVE_CENTER;
			food.y = rand() % (ROW - 2) + 1;
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)//食物的位置不能与蛇位置相同
				{
					flag1 = 0;
					break;
				}
			}
			if (food.x % 2 == 1)//食物出现在蛇身上或食物X坐标为奇数时，重新出现食物
				flag1 = 0;
			if (flag1)//食物和道具刷新位置
				break;
		}
		//刷新食物或道具类型
		srand((unsigned int)time(NULL));
		num = rand() % (10);
		if (num < 6) {
			gotoxy(food.x, food.y);
			cout << WALL;
		}
		else {
			gotoxy(food.x, food.y);
			cout << PROP;
			if (snake.x[0] == food.x && snake.y[0] == food.y && num == 6) {
				gotoxy(102, 2);
				cout << "使用了无敌道具    ";
			}
		}
		gotoxy(102, 3);
		cout << "当前蛇的长度为" << snake.len;
	}
}

/*
	负责人：谢
	功能：游戏界面整合
		  1.初始化游戏数据(调用函数init())
		  2.进入开始页面menuView()
			（1）进入登录页面dengluView()
			（2）进入难度选择页面nanduxuanzeView()
			while(1){
				打印地图, 打印蛇();
				if(用户是否有输入){
					input = 接收输入;
					switch(input){
               			case WASD:改变蛇的方向
					}
				}
				else{
					用户无输入蛇自行移动
				}
				//每次循环都要执行的代码
       			执行结果 = 移动一格的整体逻辑();
				eg:if(蛇头的横坐标snake.x[0]等于食物的横坐标，并且蛇头的纵坐标snake.y[0]等于食物的纵坐标) 执行结果=3；
				switch(执行结果){
					case 1撞墙: 游戏结束();gameoverView()
					case 2吃到自己:游戏结束();gameoverView()
					case 3吃到食物: 蛇变长，积分增加;
					case 4吃到道具1: 触发道具效果
					case 5吃到道具2: 触发道具效果
					case 6吃到道具3: 触发道具效果
					case 7吃到道具4: 触发道具效果
					case 8正常移动: 无事发生;
				}
			}
			或（1）进入游戏帮助界面youxibangzhuView()
			或（1）进入排行榜难度选择界面paihangbang_nanduxuanzeView()
			  （2）进入相应的排行榜界面paihangbangView()
	参数：void
	返回值；void
*/

void gameView() {
	createMap();
	while (1) {
		moveSnake();
		createFoodorprop();
		Sleep(snake.speed);
		if (gameover(num)) {
			gotoxy(COL / 2, ROW / 2);
			//cout << "Game Over";
			gameoverView(score, player, filename);
			Hide();
			getchar();
			exit(0);
		}
	}
}

