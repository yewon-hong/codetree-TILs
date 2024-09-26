#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <filesystem>

using namespace std;

struct Coord {
	int y;
	int x;
};

struct Node {
	int y;
	int x;
	int dir; // 0: 0도, 1: 90도, 2: 180도, 3: 270도
	int maxi;
	vector<Coord> path;

	bool operator< (Node right) const {
		if (maxi > right.maxi) return false;
		if (maxi < right.maxi) return true;
		if (dir < right.dir) return false;
		if (dir > right.dir) return true;
		if (x < right.x) return false;
		if (x > right.x) return true;
		if (y < right.y) return false;
		return true;
	}
};

int K, M;
int Map[6][6];
int Map2[6][6];
vector<int> vec;

int dy[] = { -1, 1, 0, 0 };
int dx[] = { 0, 0, -1, 1 };

void turnBox(int y, int x, int dir) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			switch (dir) {
			case 0: // 제자리
				Map2[i + y - 1][j + x - 1] = Map[i + y - 1][j + x - 1];
				break;
			case 1: // 90도 회전
				Map2[j + y - 1][2 - i + x - 1] = Map[i + y - 1][j + x - 1];
				break;
			case 2: // 180도 회전
				Map2[2 - i + y - 1][2 - j + x - 1] = Map[i + y - 1][j + x - 1];
				break;
			case 3: // 270도 회전
				Map2[2 - j + y - 1][i + x - 1] = Map[i + y - 1][j + x - 1];
				break;
			default:
				break;
			}
		}
	}
}

vector<Coord> BFS(int MAP[6][6]) {
	vector<Coord> path;
	int visited[6][6] = { 0 };

	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 6; j++) {
			if (visited[i][j]) continue;
			queue<Node> q;
			int cnt = 1;

			q.push({ i, j, cnt });
			Node now;

			while (!q.empty()) {
				now = q.front();
				q.pop();

				if (!visited[now.y][now.x]) visited[now.y][now.x] = 1;
				else continue;

				path.push_back({ now.y, now.x });

				for (int k = 0; k < 4; k++) {
					int ny = now.y + dy[k];
					int nx = now.x + dx[k];

					if (ny < 1 || ny >= 6 || nx < 1 || nx >= 6) continue;
					if (visited[ny][nx]) continue;
					if (MAP[ny][nx] != MAP[now.y][now.x]) continue;

					q.push({ ny, nx, ++cnt });
				}

			}

			if (now.dir < 3) while (now.dir--) path.pop_back();
		}
	}

	return path;
}

void setMap(Node node) {
	turnBox(node.y, node.x, node.dir);
	copy(&Map2[0][0], &Map2[0][0] + (6 * 6), &Map[0][0]);
}

bool compare(Coord left, Coord right) { // 열이 작은 순으로 -> 행이 작은 순으로
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;
	if (left.y > right.y) return true;
	return false;
}

Node exploration() {
	Node result = { 6, 6, 3, 0 };
	copy(&Map[0][0], &Map[0][0] + (6 * 6), &Map2[0][0]);

	for (int d = 1; d < 4; d++) {
		for (int i = 2; i < 5; i++) {
			for (int j = 2; j < 5; j++) {
				turnBox(i, j, d);
				vector<Coord> vec = BFS(Map2);
				Node next = { i, j, d, (int)vec.size(), vec };
				if (result < next) result = next;
				turnBox(i, j, 0); // 초기화
			}
		}
	}

	setMap(result);

	return result;
}

int main() {
	// 입력
	cin >> K >> M;
	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 6; j++) {
			cin >> Map[i][j];
			Map2[i][j] = Map[i][j];
		}
	}

	int num;
	while (M--) {
		cin >> num;
		vec.push_back(num);
	}

	// 시작
	while (K--) {
		int answer = 0;


		Node now = exploration();
		if (now.path.size() == 0) { //유물이 없을 경우
			break;
		}
		answer += now.path.size();

		while (1) {
			// vector sort
			sort(now.path.begin(), now.path.end(), compare);

			// 순서대로 벽면에 써있는 숫자 채워넣기
			for (int i = 0; i < now.path.size(); i++) {
				Map[now.path[i].y][now.path[i].x] = vec[++M];
			}

			// vector clear
			now.path.clear();

			// BFS 돌기 & vector 갱신
			now.path = BFS(Map);

			// 만약 유물이 없을 경우 break
			if (now.path.size() == 0) break;
			else answer += now.path.size();
		}
		cout << answer << ' ';
	}
}