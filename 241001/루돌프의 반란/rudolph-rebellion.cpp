#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

/*
5 30 10 2 3
4 4
6 2 3
7 5 5
2 5 1
5 1 4
4 5 2
1 3 3
9 4 1
8 2 2
3 3 2
10 2 1
*/

struct Rudolph {
	int y;
	int x;
};

struct Santa {
	int y;
	int x;
	int id;
	//int ver;
	int score;
	bool fainting;
	bool die;
	int cnt;

	bool operator< (Santa right) const {
		return id < right.id;
	}
};


int N, M, P, C, D;
int Rr, Rc, Rver, Rdir;
vector<Santa> santas;
int Map[51][51];

int dy_R[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dx_R[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int dy_S[] = { -1, 0, 1, 0 };
int dx_S[] = { 0, 1, 0, -1 };

void moveRudolph() {
	int mini = 6000;
	int mini_y = 0;
	int mini_x = 0;

	for (int i = 0; i < santas.size(); i++) {
		if (santas[i].die) continue;
		int temp = (int)pow(santas[i].y - Rr, 2) + (int)pow(santas[i].x - Rc, 2);
		if (temp < mini) {
			mini = temp;
			mini_y = santas[i].y;
			mini_x = santas[i].x;
		}
		else if (temp == mini) {
			if (santas[i].y > mini_y || (santas[i].y == mini_y && santas[i].x > mini_x)) {
				mini_y = santas[i].y;
				mini_x = santas[i].x;
			}
		}
	}

	int mini_santa = 6000;
	Rudolph next = { 0, 0 };
	int dir = 0;
	for (int i = 0; i < 8; i++) {
		int ny = Rr + dy_R[i];
		int nx = Rc + dx_R[i];

		if (ny <1 || ny >N || nx <1 || nx >N) continue;

		int temp = pow(mini_y - ny, 2) + pow(mini_x - nx, 2);

		if (temp < mini_santa) {
			mini_santa = temp;
			next = { ny, nx };
			dir = i;
		}
	}

	Rr = next.y;
	Rc = next.x;
	Rver++;
	Rdir = dir;
}

// 충돌 여부 체크
// 루돌프: DIR = 방향 값  checkCollision(C, Rdir)
// 산타: DIR = 방향 값 * 2 checkCollision(D, dir*2)
void checkCollision(int score, int DIR) {
	if (Map[Rr][Rc]) { // 루돌프와 산타가 충돌할 경우
		
		santas[Map[Rr][Rc] - 1].score += score; // 충돌한 산타에 점수 추가
		santas[Map[Rr][Rc] - 1].fainting = true; // 충돌한 산타 기절
		
		int now_y = Rr + dy_R[DIR] * score; // 충돌한 산타 밀려남
		int now_x = Rc + dx_R[DIR] * score; // 충돌한 산타 밀려남

		if (now_y <1 || now_y >N || now_x <1 || now_x >N) { // 판 밖으로 밀려날 경우
			santas[Map[Rr][Rc] - 1].die = true;
			Map[Rr][Rc] = 0;
			P--;
		}
		else { // 판 안에서 살아남을 경우 -> 상호작용 확인
			if (Map[now_y][now_x]) { // 다른 산타가 있을 경우
				int cnt = 0;
				while (1) { // 밀려날 칸이 없거나, 밀어낼 산타가 없을 때까지
					if (now_y <1 || now_y >N || now_x <1 || now_x >N) break;
					if (!Map[now_y][now_x]) break;
					cnt++;
					now_y += dy_R[DIR];
					now_x += dx_R[DIR];
				}

				if (now_y <1 || now_y >N || now_x <1 || now_x >N) { // 상호작용으로 다른 산타가 밀려나서 탈락할 경우

					now_y += dy_R[(DIR + 4) % 8];
					now_x += dx_R[(DIR + 4) % 8];
					santas[Map[now_y][now_x]-1].die = true;
					P--;
					cnt--;
				}

				while (cnt--) {
					Map[now_y][now_x] = Map[now_y + dy_R[(DIR + 4) % 8]][now_x + dx_R[(DIR + 4) % 8]];
					santas[Map[now_y + dy_R[(DIR + 4) % 8]][now_x + dx_R[(DIR + 4) % 8]] - 1].y = now_y;
					santas[Map[now_y + dy_R[(DIR + 4) % 8]][now_x + dx_R[(DIR + 4) % 8]] - 1].x = now_x;

					now_y += dy_R[(DIR + 4) % 8];
					now_x += dx_R[(DIR + 4) % 8];
				}
				
				Map[now_y][now_x] = Map[Rr][Rc];
				santas[Map[Rr][Rc] - 1].y = now_y;
				santas[Map[Rr][Rc] - 1].x = now_x;
				Map[Rr][Rc] = 0;

			}
			else { // 없을 경우 이동 후 이전 위치 삭제
				Map[now_y][now_x] = Map[Rr][Rc];
				santas[Map[Rr][Rc] - 1].y = now_y;
				santas[Map[Rr][Rc] - 1].x = now_x;
				Map[Rr][Rc] = 0;
			}
				
		}

		
	}
}

void moveSantas() {
	// 1번 산타부터 순서대로 이동
	for (int i = 0; i < santas.size(); i++) {
		Santa now = santas[i];
		if (now.die) continue; // 죽었으면 continue
		if (now.fainting) { // 기절했으면 다음턴에는 돌도록 false로 바꾸고 continue
			//now.cnt++;
			continue;
		}

		int mini_santa = pow(Rr - now.y, 2) + pow(Rc - now.x, 2);
		Rudolph next = { now.y, now.x };
		int dir = -1;
		for (int j = 0; j < 4; j++) { // 4방향 중 루돌프와 가까운 방향 확인
			int ny = now.y + dy_S[j];
			int nx = now.x + dx_S[j];

			if (ny <1 || ny >N || nx <1 || nx >N) continue;
			if (Map[ny][nx]) continue;

			int temp = pow(Rr - ny, 2) + pow(Rc - nx, 2);

			if (temp < mini_santa) {
				mini_santa = temp;
				next = { ny, nx };
				dir = j;
			}
		}

		if (dir != -1) { // 움직일 곳이 있을 경우
			Map[next.y][next.x] = i + 1;
			Map[now.y][now.x] = 0;
			santas[i].y = next.y;
			santas[i].x = next.x;

			checkCollision(D, (dir * 2 + 4) % 8); // 루돌프와 충돌 여부 확인
		}
		else { // 움직일 곳이 없거나 루돌프와 가까워질 곳이 없을 경우
			continue;
		}
	}
}

bool getOnePoint() {
	bool flag = false;
	for (int i = 0; i < santas.size(); i++) {
		if (santas[i].die) continue;
		if (santas[i].fainting) {
			if (santas[i].cnt == 0) santas[i].cnt++;
			else {
				santas[i].fainting = false;
				santas[i].cnt = 0;
			}
		}
		flag = true;
		santas[i].score++;
	}

	return flag;
}


int main() {
	cin >> N >> M >> P >> C >> D;
	cin >> Rr >> Rc;
	Rver = 0;

	int Pn, Sr, Sc;
	for (int i = 0; i < P; i++) {
		cin >> Pn >> Sr >> Sc;
		santas.push_back({ Sr, Sc, Pn, 0, false, false, 0 });
		Map[Sr][Sc] = Pn;
	}

	sort(santas.begin(), santas.end());

	// 턴 진행
	while (M--) {
		//if (P == 0) break;
		moveRudolph();
		checkCollision(C, Rdir);
		moveSantas();
		if (!getOnePoint()) break;
	}

	// 산타별 점수 출력
	for (int i = 0; i < santas.size(); i++) cout << santas[i].score << ' ';

	return 0;
}