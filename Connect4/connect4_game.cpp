#pragma warning(disable : 4996)

#include<windows.h>
#include<stdio.h>
#include "connect4_heuristic.h"
#include "connect4_rulebase2.h"
#include "connect4_rulebase.h"
#include "connect4_game.h"	

int board[10][10], len[10], log[50][2], turn = 1;
int dy[4] = { 0, -1, -1, -1 }, dx[4] = { -1, -1, 0, 1 };

void print_board() {
	for (int i = 6;i >= 1;i--) {
		printf("|");
		for (int j = 1;j <= 7;j++) {
			if (board[j][i] == 0) printf("  |");
			else if (board[j][i] == 1) printf("○|");
			else printf("●|");
		}
		printf("\n");
	}
	printf(" ");
	for (int i = 1;i <= 7;i++) printf(" %d ", i);
	printf("\n\n");
}

void put_piece(int x, bool num) {
	board[x][++len[x]] = num?1:2;
}

void deput_piece(int x) {
	board[x][len[x]--] = 0;
}

int putable_both_side(int x) {
	int cnt1 = 0;
	for (int i = 0;i < 4;i++) {
		bool flag = false;
		int cnt = 1;
		for (int j = -1;j >= -2;j--) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = false;
				break;
			}
			cnt++;
		}
		for (int j = 1;j <= 2;j++) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = flag;
				else flag = false;
				break;
			}
			cnt++;
		}
		if (flag && cnt >= 3) cnt1++;
	}
	return cnt1;
}

bool double_three(int x) {
	int cnt1 = 0;
	for (int i = 0;i < 4;i++) {
		bool flag = false;
		int cnt = 1;
		for (int j = -1;j >= -2;j--) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = false;
				break;
			}
			cnt++;
		}
		for (int j = 1;j <= 2;j++) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = flag;
				break;
			}
			cnt++;
		}
		if (flag && cnt >= 3) cnt1++;
	}
	if (cnt1 >= 2) return true;
	return false;
}

int get_unlimit_length_connected(int x, int i) {
	int cnt = 1;
	for (int j = -1;j >= -3;j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] != 0 && board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	for (int j = 1;j <= 3;j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] != 0 && board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	return cnt;
}

int get_overflow_length_connected(int x, int i) {
	int cnt = 1;
	bool flag = false;
	for (int j = -1;j >= -3;j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] == 0 && !flag) {
			flag = true;
			cnt++;
		}
		else if (board[target_x][target_y] == board[x][len[x]]) cnt++;
		else break;
	}
	flag = false;
	for (int j = 1;j <= 3;j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] == 0 && !flag) {
			flag = true;
			cnt++;
		}
		else if (board[target_x][target_y] == board[x][len[x]]) cnt++;
		else break;
	}
	return cnt;
}

int get_current_length_connected(int x, int i) {
	int cnt = 1;
	for (int j = -1;j >= -3;j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7 || board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	for (int j = 1;j <= 3;j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>7 || target_y < 1 || target_y>7 || board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	return cnt;
}

int get_max_length_connected(int x) {
	int maxv = 0;
	for (int i = 0;i < 4;i++) {
		int result = get_current_length_connected(x, i);
		if (maxv < result) maxv = result;
	}
	return maxv;
}

void game_over(int winner) {


	printf("게임 종료\n\n");
	print_board();

	if (winner == 1) printf("○ 승리!");
	else printf("● 승리!");

	Sleep(3000);



	freopen("result.txt", "w", stdout);
	for (int i = 0;i < turn;i++) printf("%d %d\n", log[i][0], log[i][1]);
}

int main() {
	int is_first, placeholder;
	printf("공격 순서 (선공: 1, 후공: 2) : ");
	scanf("%d", &is_first);

	system("cls");
	if (is_first == 1) {
		printf("\n\n");
		print_board();
		printf("○ 나의 돌을 놓을 알고리즘 (Heuristic: 1, RuleBase: 2) : ");
		scanf("%d", &placeholder);

		/* 선공이면 무조건 5에 놓고 시작함 */	

		put_piece(5, true);
		system("cls");

		printf("마지막 놓은 위치 : 5\n\n");

		/* logging */

		log[turn][0] = 1;
		log[turn++][1] = 5;
	}
	else printf("\n\n");

	while (true) {
		int position;

		/* print basic information */

		print_board();
		printf("● 상대 돌을 놓을 위치(1~7) : ");
		scanf("%d", &position);
		if (position < 1 || position>7 || len[position] >= 7) {
			system("cls");
			printf("잘못된 위치의 입력\n\n");
			continue;
		}

		put_piece(position, false);
		system("cls");

		/* logging */

		log[turn][0] = 2;
		log[turn++][1] = position;

		/* Discriminate that the game is end */

		if (get_max_length_connected(position) >= 4) {
			game_over(2);
			return 0;
		}

		/* print basic information */

		printf("마지막 놓은 위치 : %d\n", position);
		printf("\n");
		print_board();

		int algorithm;
		printf("○ 나의 돌을 놓을 알고리즘 (Heuristic: 1, RuleBase: 2) : ");
		scanf("%d", &algorithm);


		// position = get_position_by_rulebase();
		if (algorithm == 1) position = get_position_by_heuristic();
		else position = get_position_by_rulebase();

		put_piece(position, true);
		system("cls");

		/* logging */

		log[turn][0] = 1;
		log[turn++][1] = position;

		/* Discriminate that the game is end */

		if (get_max_length_connected(position) >= 4) {
			game_over(1);
			return 0;
		}

		/* print basic information */

		printf("마지막 놓은 위치 : %d\n\n", position);

	}
	return 0;
}