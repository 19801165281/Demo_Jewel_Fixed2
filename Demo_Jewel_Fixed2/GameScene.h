#pragma once
#include <easy2d/easy2d.h>
#include "GameTask.h"
#include "HintButton.h"
#include "ScoreBoard.h"
#include "WashButton.h"
#include "GameStatus.h"
#include "Jewel.h"
#include <mutex>
using namespace easy2d;

enum class Mode {
	Classic,
	Fast
};

class GameScene :
	public Scene
{
public:
	//默认构造函数
	GameScene();
	//构造函数，指定游戏关卡和游戏模式
	GameScene(int l, Mode mode = Mode::Classic);
	//按帧数刷新游戏场景，捕获操作和游戏逻辑，播放相应的动画
	void onUpdate() override;
	//禁用/恢复所有Button
	static void SetBtnEnabled(bool status);
	//游戏模式
	Mode game_mode;
	//步数
	int steps;
	//#############################################################
	//竞速模式需要的内容
	// 开始计时函数
	void startTiming();
	// 结束计时函数
	void endTiming();
	//在游戏暂停时调用，暂停计时
	void pauseTiming();
	//在游戏恢复时调用，恢复计时
	void resumeTiming();
	// 计时函数
	void timing();
	//更新显示所剩时间的函数
	void setTimeLastSeconds(int seconds);
	//增加触发暂停的按钮的函数
	void addPauseBtn();
	//记录计时是否开始/结束的属性
	//开始为true
	bool _timingStarted = false;
	//记录计时是否暂停的属性
	//暂停为true
	bool _timingPaused = false;
	//记录当前所剩时间的属性，初始状态为100s
	int _timeLast = 100;
	//显示时间的文本框
	Text* _timeLastText = nullptr;
	Text* _stepText = nullptr;
	

	~GameScene();
	//捕获选择操作时需要的函数
	static int get_selected_jewels_numbers();
	static void add_selected_jewels_numbers();
	static void init_selected_jewels_numbers();
	static void minu_selected_jewels_numbers();
	//存储宝石的容器数组
	static Jewel* map[8][8];
	//中断onupdate的小trick
	static bool canResume;
	//游戏逻辑任务实体
	GameTask* gt;
	//游戏状态任务实体
	GameStatus* gst;
	//提示按钮
	HintButton* hint_btn;
	//分数板
	ScoreBoard* score_board;
	//二级菜单
	//SubMenu* submenu;
	//测试洗牌的函数
	void WashJews();
	
protected:
	//捕获选择宝石数目的静态变量
	static int selected_jewels_numbers;
	//私有函数，用于在构造函数中初始化场景内容
	void Initialize();
};

