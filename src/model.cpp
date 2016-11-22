/*
 * model.cpp
 * 入力データInputDataの内容を場面Sceneに反映させる
 * 不公平にならないよう注意が必要
 */

//inputとsceneの連携
#include <iostream>
#include "model.h"
#include "manager.h"
#include "input.h"

void Model::initModel(void) {
//	std::cout << "Init" << std::endl;
	Scene &scene = Manager::getInstance().scene;
	time_t t;
	t = time(NULL);
	localtime_r(&t, &scene.tm);
	scene.c[0] = 0;
	scene.c[1] = 0;
	for (Players::iterator i = scene.p.begin(); i != scene.p.end(); ++i) {
		i->second.curDots = 0;
		for (int j = 0; j < max_dots; ++j) {
			i->second.dots[j].visible = 0;
		}
	}
}

void Model::preAction(void) { // 衝突判定など、判定のみを行う。公平のため、ここで動かしてはいけない
	time_t t;
	Scene &scene = Manager::getInstance().scene;
	t = time(NULL);
	localtime_r(&t, &scene.tm);
}

void Model::postAction(void) { // 全プレイヤーの動作を終えた後に、全体の状況を変えたい場合はここで処理する

}

void Model::stepPlayer(int fd) { // 各プレイヤーの動作を行う。公平のため、ここでは判断を行ってはいけない
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;
	InputData &input = mgr.members[fd].input;
	int id = 0;
	for (Members::iterator i = mgr.members.begin(); i != mgr.members.end();
			++i, ++id) {
		if (i->first == fd)
			break;
	}
	Player &player = scene.p[id];
	//imgcircle &ic=scene.pic; 短縮

	for (int i = 0; i < max_dots; ++i) {
		player.dots[i].x += (input.right - input.left) * 5;
		player.dots[i].y += (input.down - input.up) * 5;
	}

	//追記
	for (int i = 0; i < max_dots; ++i) {
		scene.pic.picx += (input.right - input.left) * 2;
		scene.pic.picy += (input.down - input.up) * 2;
	}

	if (input.x != (-1)) {
		player.dots[player.curDots].x = input.x;
		player.dots[player.curDots].y = input.y;
		player.dots[player.curDots].visible = 1;
		player.curDots = (player.curDots + 1) % max_dots;

		//追記
		scene.pic.picx = input.x;
		scene.pic.picy = input.y;
	}

	if (input.w != (-1)) {
	scene.pic.change = input.w;
	}

	player.ax = input.ax;
	player.ay = input.ay;
	player.az = input.az;
	if (input.key != 0) {
		scene.c[0] = input.key;
	}
}