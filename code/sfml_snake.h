#pragma once


////////////////////////////
/// 输出提示信息
/// 
///
/// _x 输出x坐标
/// _y 输出y坐标
////////////////////////////
void Prompt_info(int _x, int _y);

////////////////////////////
/// 游戏失败信息
/// 
/// 
/// _x 输出x坐标
/// _y 输出y坐标
////////////////////////////
void gameFail_info(int _x, int _y);

////////////////////////////
/// 游戏胜利信息
/// 
/// 
/// _x 输出x坐标
/// _y 输出y坐标
////////////////////////////
void gameSuccess_info(int _x, int _y);

////////////////////////////
/// 游戏内菜单信息
/// 
/// 
/// 
/// 
////////////////////////////
void ingameMenu_info();

////////////////////////////
/// 暂停信息
/// 
/// 
/// 
/// 
////////////////////////////
void gamePause_info();

////////////////////////////
/// 游戏内 快速保存
/// 
/// 
/// 
/// 
////////////////////////////
void qSave();

////////////////////////////
/// 游戏内 快速读档
/// 
/// 
/// 
/// 
////////////////////////////
void qLoad();

////////////////////////////
/// 游戏结束输入
/// 
/// 
/// _b1 gameSuccess布尔
/// _b2 gameFail布尔
////////////////////////////
void gameOver_input(bool& _b1, bool& _b2);

////////////////////////////
/// 启动菜单显示
/// 
/// 
/// 
/// 
////////////////////////////
void Menu();

////////////////////////////
/// 启动菜单输入
/// 
/// 
/// 
/// 
////////////////////////////
void Menu_input();

////////////////////////////
/// 保存读档界面
/// 
/// 
/// 
/// 
////////////////////////////
void SaveAndLoad_info();

////////////////////////////
/// 保存读档操作
/// 
/// 
/// 
/// 
////////////////////////////
void SaveAndLoad(int j);

////////////////////////////
/// 设置界面
/// 
/// 
/// 
/// 
////////////////////////////
void Option_info();

////////////////////////////
/// 游戏结束绘制
/// 
/// 
/// 
/// 
////////////////////////////
void gameOver_draw();

