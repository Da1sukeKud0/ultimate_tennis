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
using namespace std;

//仮想コートの座標設定
int rux = 0;
int ruy = 0;
int lux = 606;
int luy = 0;
int rdx = 0;
int rdy = 424;
int ldx = 606;
int ldy = 424;

int GetRandom(int min, int max) {
	return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void Model::initModel(void) {
//std::cout << "Init" << std::endl;
	Scene &scene = Manager::getInstance().scene;
	time_t t;
	t = time(NULL);
	localtime_r(&t, &scene.tm);
	scene.c[0] = 0;
	scene.c[1] = 0;
	/*for (Players::iterator i = scene.p.begin(); i != scene.p.end(); ++i) {
	 i->second.curDots = 0;
	 for (int j = 0; j < max_dots; ++j) {
	 i->second.dots[j].visible = 0;
	 }
	 }*/

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

//Player &player = scene.p[id];

	for (int i = 0; i < max_dots; ++i) {
		//player.dots[i].x += (input.right - input.left) * 5;
		//player.dots[i].y += (input.down - input.up) * 5;
	}

//追記
	if (id == -1) {
		if (scene.ip.y >= 212 && scene.ip.x >= 0 && scene.ip.x <= 606) {
			scene.ip.x += (input.right - input.left) * 10;
			scene.ip.y += (input.down - input.up) * 10;
		}
		if (input.w != (-1)) { //追記
			scene.g.change1 = input.w;
		}
		if (input.a != (-1)) { //追記
			scene.g.slice1 = input.a;
		}
		if (input.f != (-1)) { //追記
			scene.g.hissatu1 = input.f;
		}
	} else if (id == 0) { //server
		if (scene.ip.y >= 212) {
			scene.ip.y += (input.down - input.up) * 10;
		} else {
			scene.ip.y = 212;
		}
		if (scene.ip.y <= 424) {
			scene.ip.y += (input.down - input.up) * 10;
		} else {
			scene.ip.y = 424;
		}
		if (scene.ip.x >= 0) {
			scene.ip.x += (input.right - input.left) * 10;
		} else {
			scene.ip.x = 0;
		}
		if (scene.ip.x <= 606) {
			scene.ip.x += (input.right - input.left) * 10;
		} else {
			scene.ip.x = 606;
		}
		if (input.w != (-1) && scene.g.chanp == 0) { //追記
			scene.g.change1 = input.w;
		}
		if (input.score1 != (-1) && scene.g.chanp == 0) { //追記
			scene.g.normalslice1 = input.score1;
		}
		if (input.a != (-1) && scene.g.chanp == 0 && scene.g.slicelimit1 <= 2) { //追記
			scene.g.slice1 = input.a;
		}
		if (input.f != (-1) && scene.g.chanp == 0
				&& scene.g.hissatulimit1 <= 2) { //追記
			scene.g.hissatu1 = input.f;
		}
	}

	else if (id == 1) { //client
		if (scene.ip2.y <= 212) {
			scene.ip2.y += (input.up - input.down) * 10;
		} else {
			scene.ip2.y = 212;
		}
		if (scene.ip2.y >= 0) {
			scene.ip2.y += (input.up - input.down) * 10;
		} else {
			scene.ip2.y = 0;
		}
		if (scene.ip2.x >= 0) {
			scene.ip2.x += (input.left - input.right) * 10;
		} else {
			scene.ip2.x = 0;
		}
		if (scene.ip2.x <= 606) {
			scene.ip2.x += (input.left - input.right) * 10;
		} else {
			scene.ip2.x = 606;
		}
		if (input.w != (-1) && scene.g.chanp == 1) { //追記
			scene.g.change2 = input.w;
		}
		if (input.score1 != (-1) && scene.g.chanp == 1) { //追記
			scene.g.normalslice2 = input.score1;
		}
		if (input.a != (-1) && scene.g.chanp == 1 && scene.g.slicelimit2 <= 2) { //追記
			scene.g.slice2 = input.a;
		}
		if (input.f != (-1) && scene.g.chanp == 1
				&& scene.g.hissatulimit2 <= 2) { //追記
			scene.g.hissatu2 = input.f;
		}
	}

//サーブ時プレイヤーの初期位置固定
	if (scene.g.service == 0) {
		if ((scene.s.setx + scene.s.sety) % 2 == 0) {
			scene.ibs.vx = 0;
			scene.ibs.vy = 0;
			scene.ip.y = 364;
			scene.ibs.y = 364;
			if (scene.ip.x < 122) {
				scene.ip.x = 122;
			}
			if (scene.ip.x > 392) {
				scene.ip.x = 392;
			}
			scene.ibs.x = scene.ip.x - 10;
			scene.g.chanp = 0;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
			scene.g.flag = 0;
		}

		else {
			scene.ibs.vx = 0;
			scene.ibs.vy = 0;
			scene.ip2.y = 1;
			scene.ibs.y = 1;
			if (scene.ip2.x < 122) {
				scene.ip2.x = 122;
			}
			if (scene.ip2.x > 392) {
				scene.ip2.x = 392;
			}
			scene.ibs.x = scene.ip2.x - 10;
			scene.g.chanp = 1;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
			scene.g.flag = 0;
		}
	}

	//各種スイングによる
	if (input.w == 1 || input.a == 1 || input.f == 1) {
		scene.g.service = 1;
	}

	//得点判定
	if (scene.g.chanp == 0) {
		if (scene.ibs.y >= 225 && scene.ibs.x <= -30) {
			//out的な
			scorecalc(2);
		} else if (scene.ibs.y >= 225 && scene.ibs.x >= 630) {
			//out的な
			scorecalc(2);
		} else if (scene.ibs.y <= 225 && scene.ibs.x <= -30) {
			//out的な
			scorecalc(1);
		} else if (scene.ibs.y <= 225 && scene.ibs.x >= 630) {
			//out的な
			scorecalc(1);
		} else if (scene.ibs.y <= 225 && scene.ibs.y >= 195 && scene.ibs.x >= 0
				&& scene.ibs.x <= 50) {
			scorecalc(1);
		} else if (scene.ibs.y <= 225 && scene.ibs.y >= 195
				&& scene.ibs.x >= 556 && scene.ibs.x <= 606) {
			scorecalc(1);
		} else if (scene.ibs.x >= 0 && scene.ibs.x <= 606
				&& scene.ibs.y >= 454) {
			scorecalc(2);
		}
	} else if (scene.g.chanp == 1) {
		if (scene.ibs.y >= 195 && scene.ibs.x <= 0) {
			//out的な
			scorecalc(2);
		} else if (scene.ibs.y >= 195 && scene.ibs.x >= 630) {
			//out的な
			scorecalc(2);
		} else if (scene.ibs.y <= 195 && scene.ibs.x <= 0) {
			//out的な
			scorecalc(1);
		} else if (scene.ibs.y <= 195 && scene.ibs.x >= 630) {
			//out的な
			scorecalc(1);
		} else if (scene.ibs.y <= 225 && scene.ibs.y >= 195 && scene.ibs.x >= 0
				&& scene.ibs.x <= 50) {
			scorecalc(2);
		} else if (scene.ibs.y <= 225 && scene.ibs.y >= 195
				&& scene.ibs.x >= 556 && scene.ibs.x <= 606) {
			scorecalc(2);
		} else if (scene.ibs.x >= -30 && scene.ibs.x <= 630
				&& scene.ibs.y <= -30) {
			scorecalc(1);
		}
	}

	//打った後にservice == 1になるように
	//std::cout << scene.ibs.vx << "," << scene.ibs.vy << std::endl;
	//std::cout<<scene.g.chanp<<endl;

}

void Model::ballmovement() {
	Manager &mgr = Manager::getInstance();
	Scene &scene = mgr.scene;
	//1Pノーマルショット
	if (scene.g.change1 == 1) {

		if (scene.ip.x - 50 <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 20
				&& scene.ip.y - 40 <= scene.ibs.y
				&& scene.ibs.y <= scene.ip.y + 40) {
			scene.ibs.vy = -5;
			scene.ibs.vx = (-(scene.ip.y - scene.ibs.y)) / 10;
			scene.g.flag += 1;
			scene.g.chanp = 1;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
		} else if (scene.ip.x + 20 <= scene.ibs.x
				&& scene.ibs.x <= scene.ip.x + 90
				&& scene.ip.y - 40 <= scene.ibs.y
				&& scene.ibs.y <= scene.ip.y + 40) {
			scene.ibs.vy = -5;
			scene.ibs.vx = (-(scene.ip.y - scene.ibs.y)) / 10;
			scene.g.flag += 1;
			scene.g.chanp = 1;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
		}

		scene.g.change1 = 0;
	}

	//2Pノーマルショット
	if (scene.g.change2 == 1) {
		if (scene.ip2.x - 50 <= scene.ibs.x && scene.ibs.x <= scene.ip2.x + 20
				&& scene.ip2.y - 40 <= scene.ibs.y
				&& scene.ibs.y <= scene.ip2.y + 40) {
			scene.ibs.vy = 5;
			scene.ibs.vx = (-(scene.ip2.y - scene.ibs.y)) / 10;
			scene.g.flag += 1;
			scene.g.chanp = 0;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
		} else if (scene.ip2.x + 20 <= scene.ibs.x
				&& scene.ibs.x <= scene.ip2.x + 90
				&& scene.ip2.y - 40 <= scene.ibs.y
				&& scene.ibs.y <= scene.ip2.y + 40) {
			scene.ibs.vy = 5;
			scene.ibs.vx = (-(scene.ip2.y - scene.ibs.y)) / 10;
			scene.g.flag += 1;
			scene.g.chanp = 0;
			scene.g.slice = 0;
			scene.g.hissatu = 0;
		}
		scene.g.change2 = 0;
	}

	//1Pノーマルスライス
	if (scene.g.normalslice1 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip.x - 50 <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 20
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -4;
				scene.ibs.svx = 0.02;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 2;
				scene.g.hissatu = 0;
			} else if (scene.ip.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x + 90
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -4;
				scene.ibs.svx = -0.02;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 2;
				scene.g.hissatu = 0;
			}
			scene.g.normalslice1 = 0;
		}
	}

	//2Pノーマルスライス
	if (scene.g.normalslice2 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip2.x - 50 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 20
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 4;
				scene.ibs.svx = 0.02;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.slice = 2;
				scene.g.hissatu = 0;
			} else if (scene.ip2.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 90
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 4;
				scene.ibs.svx = -0.02;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.slice = 2;
				scene.g.hissatu = 0;
			}
			scene.g.normalslice2 = 0;
		}
	}

	//1Pスーパースライス
	if (scene.g.slice1 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip.x - 50 <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 20
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -7;
				scene.ibs.svx = 0.07;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 1;
				scene.g.hissatu = 0;
				scene.g.slicelimit1 += 1;
			} else if (scene.ip.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x + 90
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -7;
				scene.ibs.svx = -0.07;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 1;
				scene.g.hissatu = 0;
				scene.g.slicelimit1 += 1;
			}
			scene.g.slice1 = 0;
		}
	}

	//2Pスーパースライス
	if (scene.g.slice2 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip2.x - 50 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 20
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 7;
				scene.ibs.svx = 0.07;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.slice = 1;
				scene.g.hissatu = 0;
				scene.g.slicelimit2 += 1;
			} else if (scene.ip2.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 90
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 7;
				scene.ibs.svx = -0.07;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.slice = 1;
				scene.g.hissatu = 0;
				scene.g.slicelimit2 += 1;
			}
			scene.g.slice2 = 0;
		}
	}

	//1P必殺
	if (scene.g.hissatu1 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip.x - 50 <= scene.ibs.x && scene.ibs.x <= scene.ip.x + 20
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -18;
				scene.ibs.vx = 0;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 0;
				scene.g.hissatu = 2;
				scene.g.hissatulimit1 += 1;
			} else if (scene.ip.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip.x + 90
					&& scene.ip.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip.y + 40) {
				scene.ibs.vy = -18;
				scene.ibs.vx = 0;
				scene.g.flag += 1;
				scene.g.chanp = 1;
				scene.g.slice = 0;
				scene.g.hissatu = 2;
				scene.g.hissatulimit1 += 1;
			}
			scene.g.hissatu1 = 0;

		}
	}

	//2P必殺
	if (scene.g.hissatu2 == 1) {
		if (scene.g.flag != 0) {
			if (scene.ip2.x - 50 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 20
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 3;
				scene.ibs.vx = 0;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.hissatu = 1;
				scene.g.slice = 0;
				scene.g.hissatulimit2 += 1;
			} else if (scene.ip2.x + 20 <= scene.ibs.x
					&& scene.ibs.x <= scene.ip2.x + 90
					&& scene.ip2.y - 40 <= scene.ibs.y
					&& scene.ibs.y <= scene.ip2.y + 40) {
				scene.ibs.vy = 3;
				scene.ibs.vx = 0;
				scene.g.flag += 1;
				scene.g.chanp = 0;
				scene.g.hissatu = 1;
				scene.g.slice = 0;
				scene.g.hissatulimit2 += 1;
			}
			scene.g.hissatu2 = 0;
		}
	}

	if (scene.g.slice == 1 || scene.g.slice == 2) {
		scene.ibs.vx += scene.ibs.svx;
		scene.ibs.y += scene.ibs.vy;
		scene.ibs.x += scene.ibs.vx;
	} else if (scene.g.hissatu == 1) {
		scene.ibs.hvx = GetRandom(-20, 20);
		scene.ibs.hvy = GetRandom(1, 20);
		scene.ibs.vx = scene.ibs.hvx;
		scene.ibs.vy = scene.ibs.hvy;
		scene.ibs.y += scene.ibs.vy;
		scene.ibs.x += scene.ibs.vx;
	} else {
		scene.ibs.y += scene.ibs.vy;
		scene.ibs.x += scene.ibs.vx;
	}
}
//通常得点
void Model::scorecalc(int i) {
	Scene &scene = Manager::getInstance().scene;
	scene.g.service = 0;
//xの得点
	switch (i) {
	case 1:
		scene.g.flag = 0;
		cout << "scorecalc呼出テスト" << scene.s.sx << endl;
		if (scene.s.sx == 3) { //40点以降
			if (scene.s.sy == 3) {
				scene.s.sx = 4; //P1がAve
				break;
			} else if (scene.s.sy == 4) {
				scene.s.sx = 3; //P2のAveに追いついたので40:40に
				scene.s.sy = 3;
				break;
			} else { //P2が30点未満なので勝利
				gameset(1);
				break;
			}
		}
		if (scene.s.sx == 4) { //P1がAveの状態から勝利
			gameset(1);
			break;
		}
		if (scene.s.sx <= 2) { //通常得点
			scene.s.sx++;
			break;
		}
		break;

	case 2:
		cout << "scorecalc呼出テスト" << scene.s.sy << endl;
		scene.g.flag = 0;
		if (scene.s.sy == 3) { //40点以降
			if (scene.s.sx == 3) {
				scene.s.sy = 4; //P2がAve
				break;
			} else if (scene.s.sx == 4) {
				scene.s.sy = 3; //P1のAveに追いついたので40:40に
				scene.s.sx = 3;
				break;
			} else { //P1が30点未満なので勝利
				gameset(2);
				break;
			}
		}
		if (scene.s.sy == 4) { //P2がAveの状態から勝利
			gameset(2);
			break;
		}
		if (scene.s.sy <= 2) { //通常得点
			scene.s.sy++;
			break;
		}
		break;
	}
}

void Model::gameset(int i) { //gamesetって書いちゃったけど1setとった時の動作＋2set先取完全試合終了時の操作を含む
	Scene &scene = Manager::getInstance().scene;
	scene.g.service = 0;

	switch (i) {
	case 1:
		scene.s.sx = scene.s.sy = 0;
		++scene.s.setx;
		if (scene.s.setx == 2) {
			scene.g.win = 1; //P1勝利画面
		}
		break;

	case 2:
		scene.s.sx = scene.s.sy = 0;
		++scene.s.sety;
		if (scene.s.sety == 2) {
			scene.g.win = 2; //P2勝利画面
		}
		break;
	}

}
