#include <iostream>

using namespace std;

//-----------------数据设计----------------------------------------
//贪吃蛇：
	/*
		eg:
		地图
		map[ROW][COL]表示30*30的地图
		其中map[i][j]表示坐标（i，j)
		其中元素值的含义：
		0空地 1墙 2食物 3无敌道具 4身体变长道具 5身体变短道具 6双倍得分道具 7蛇头 8蛇身
		eg:map[3][6] = 2;  表示（3，6）的位置是食物
	*/
int map[30][30];
int score = 0;

#define ROW 30 //游戏区行数
#define COL 30 //游戏区列数
#define WALLNUMBER//墙的数量
#define KONGDI 0//空地
#define QING 1//墙
#define SHIWU 2//食物
#define WUDI 3//无敌
#define BIANCHANG 4//身体变长
#define BIANDUAN 5//身体变短
#define SHUANGBEI 6//双倍道具
#define SHETOU 7//蛇头
#define SHESHEN 8//蛇身

//蛇的结构体
struct
{
	//记录每一节蛇的坐标
	//蛇的最大结数
	int x[ROW * COL];
	int y[ROW * COL];
	int len;		//蛇的长度
	int speed;		//蛇的移动速度
	int dir;		//蛇的移动方向
}snake;

//食物的结构体
struct
{
	int x;
	int y;
}food;

//无敌道具的结构体
struct
{
	int x;
	int y;
}wudi;

//身体变长道具的结构体,不加分数
struct
{
	int x;
	int y;
}belong;

//身体变短道具的结构体
struct
{
	int x;
	int y;
}beshort;

//双倍得分道具的结构体
struct
{
	int x;
	int y;
}doublescore;

//墙的结构体
struct
{
	int x;
	int y;
}wall[WALLNUMBER];
//-----------------数据设计----------------------------------------

//函数声明：
//-----------------service-----------------------------------------

/*
	负责人：邓
	功能：初始化游戏数据
		1.地图（空地，墙）
		2.食物（坐标）
		3.蛇（位置，方向）
		4.道具（坐标，第几轮刷新）
	参数：void
	返回值；void
*/
void init();
/*
	负责人：肖
	功能：判断游戏是否结束
		  1.蛇头的横坐标snake.x[0]等于墙的横坐标，并且蛇头的纵坐标snake.y[0]等于墙的纵坐标
		  2.蛇头的横坐标snake.x[0]等于身体的横坐标snake.x[i]，并且蛇头的纵坐标snake.y[0]等于身体的横坐标snake.y[i]
	参数：void
	返回值；0代表蛇撞到墙或吃到自己，表示游戏结束
*/
int gameover();
//-----------------service-----------------------------------------

//-----------------view--------------------------------------------
/*
	负责人：苏
	功能：展示选项, 玩家可以在这里选择1.开始游戏,2.游戏帮助或3.退出游戏
			按1：进入登陆页面
			按2：进入游戏帮助页面
			按3：退出游戏
	参数：void
	返回值；void
*/
void menuView();

/*
	负责人：武大郎
	功能：展示选项, 玩家可以在这里选择1.登录,2.注册或3.忘记密码
			按1：输入账号密码
				账号密码输入正确：
					显示登录成功
				账号密码输入错误：
					显示账号或密码错误，重新跳转到登陆页面
			按2：进入注册页面
			按3：进入忘记密码页面
	参数：void
	返回值；void
*/
void dengluView();

/*
	负责人：武大郎
	功能：1.输入新账号,2.输入新密码或3.确认新密码
		（1）两次输入的新密码相同：
			跳转到登陆页面
		（2）两次输入的新密码不同：
			让玩家再次输入新密码，和确认新密码
	参数：void
	返回值；void
*/
void zhuceView();

/*
	负责人：武大郎
	功能：1.输入账号,2.输入新密码或3.确认新密码
		(1）判断账号是否存在：
			若不存在：输出“账号不存在，请再次输入账号”让玩家再次输入账号
			若存在：
			1、两次输入的新密码相同：
				跳转到登陆页面
			2、两次输入的新密码不同：
				让玩家再次输入新密码，和确认新密码
	参数：void
	返回值；void
*/
void wangjimimaView();

/*
	负责人：苏
	功能：展示选项, 玩家可以在这里选择1.简单,2.普通，3.困难或4.噩梦
		  跳转到对应的难度的游戏界面
	参数：void
	返回值；void
*/
void nanduxuanzeView();

/*
	负责人：魏
	功能：介绍游戏规则
		1.使用WASD控制蛇的移动
		2.吃到食物会变长
		3.撞到墙会死
		4.按q键会返回开始页面
	参数：void
	返回值；void
*/
void youxibangzhuView();

/*
	负责人：郭
	功能：1.显示1到10名的得分用时，得分高用时低的排名靠前。
		  2.按q键会返回开始页面
	参数：void
	返回值；void
*/
void paihangbangView();
/*
	负责人：郭
	功能：展示选项, 玩家可以在这里选择1.简单,2.普通，3.困难或4.噩梦
		  然后跳转到相应的排行榜界面
	参数：void
	返回值；void
*/
void paihangbang_nanduxuanzeView();

/*
	负责人：田
	功能：1.显示当前难度和得分
		  2.展示选项, 玩家可以在这里选择1.继续游戏2.返回开始页面
	参数：void
	返回值；void
*/
void shezhiView();

/*
	负责人：季
	功能：1.显示得分
		  2.展示选项, 玩家可以在这里选择1.再来一局或2.返回开始页面
		  3.展示该难度历史前三排名
	参数：void
	返回值；void
*/
void gameoverView();

/*
	负责人:殷
	功能: 根据map数组 打印地图 打印蛇
	参数: void
	返回值: void
*/
void gameView_ShowMap();

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
void gameView();
//-----------------view--------------------------------------------

int main() {

	return 0;
}

void init() {

}

int gameover() {

}

void snakemove() {

}

void menuView() {

}

void dengluView() {

}

void zhuceView() {

}

void wangjimimaView() {

}

void nanduxuanzeView() {

}

void youxibangzhuView() {

}

void paihangbangView() {

}

void paihangbang_nanduxuanzeView() {

}

void shezhiView() {

}

void gameoverView() {

}

void gameView_ShowMap() {

}

void gameView() {

}
