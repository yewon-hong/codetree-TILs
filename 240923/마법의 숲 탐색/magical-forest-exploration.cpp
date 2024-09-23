#include <iostream>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

struct Gollam {
	int y;
	int x;
	int dir;
	int id;
};

int R, C, K;
pair<int, int> Map[75][75];

int sy, sx;
int result = 0;

int dy[] = { -1, 0, 1, 0 };
int dx[] = { 0, 1, 0, -1 };

int check_dy[3][5] = { {1, 2, 1}, {-1, 0, 1, 1, 2}, {-1, 0, 1, 1, 2} };
int check_dx[3][5] = { {-1, 0, 1}, {-1, -2, -1, -2, -1}, {1, 2, 1, 2, 1} };

bool moveGollam(int row, int dir, int id) {

	queue<Gollam> q;
	q.push({ 0, row, dir, id });

	while (!q.empty()) {
		Gollam now = q.front();
		q.pop();

		int cnt = 0;

		// 남쪽 방향 탐색
		for (int i = 0; i < 3; i++) {
			int ny = now.y + check_dy[0][i];
			int nx = now.x + check_dx[0][i];

			if (Map[ny][nx].first) break;
			if (nx < 1 || ny > R + 2 || nx > C) break;

			cnt++;
		}

		if (cnt == 3) {
			q.push({ now.y + 1, now.x, now.dir, now.id });
			continue;
		}

		cnt = 0;

		// 서쪽 방향 탐색
		for (int i = 0; i < 5; i++) {
			int ny = now.y + check_dy[1][i];
			int nx = now.x + check_dx[1][i];

			if (Map[ny][nx].first) break;
			if (nx < 1 || ny > R + 2 || nx > C) break;

			cnt++;
		}

		if (cnt == 5) {
			q.push({ now.y + 1, now.x - 1, (now.dir + 3) % 4, now.id });
			continue;
		}

		cnt = 0;

		//동쪽 방향 탐색
		for (int i = 0; i < 5; i++) {
			int ny = now.y + check_dy[2][i];
			int nx = now.x + check_dx[2][i];

			if (Map[ny][nx].first) break;
			if (nx < 1 || ny > R + 2 || nx > C) break;

			cnt++;
		}

		if (cnt == 5) {
			q.push({ now.y + 1, now.x + 1, (now.dir + 1) % 4, now.id });
			continue;
		}

		if (now.y > 3) {
			sy = now.y;
			sx = now.x;

			Map[now.y][now.x] = make_pair(now.id, 0);

			for (int i = 0; i < 4; i++) {
				int ny = now.y + dy[i];
				int nx = now.x + dx[i];

				if (i == now.dir) Map[ny][nx] = make_pair(now.id, 1);
				else Map[ny][nx] = make_pair(now.id, 0);
			}
			return true;
		}
	}

	return false;
}

void moveSpirit() {
	queue<Gollam> q;
	int visited[75][75] = {};
	int maxi = 0;

	q.push({ sy, sx });

	while (!q.empty()) {
		Gollam now = q.front();
		q.pop();

		if (!visited[now.y][now.x])visited[now.y][now.x] = 1;
		else continue;

		maxi = max(maxi, now.y);
		if (maxi == R + 2) break;

		for (int i = 0; i < 4; i++) {
			int ny = now.y + dy[i];
			int nx = now.x + dx[i];

			if (ny < 1 || nx < 1 || ny > R + 2 || nx > C) continue;
			if (visited[ny][nx]) continue;
			if (!Map[ny][nx].first) continue;
			if (Map[now.y][now.x].first != Map[ny][nx].first && !Map[now.y][now.x].second) continue;

			q.push({ ny, nx });
		}
	}

	result += maxi - 2;
}

void reset() {
	for (int i = 1; i <= R + 2; i++) memset(Map[i], 0, sizeof(pair<int, int>) * (C + 2));
}

int main() {
	cin >> R >> C >> K;

	int ci, di;
	int cnt = 1;
	bool flag = false;
	while (K--) {
		cin >> ci >> di;
		flag = false;

		flag = moveGollam(ci, di, cnt);
		if (flag) moveSpirit();
		else reset();
		cnt++;
	}

	cout << result;
}