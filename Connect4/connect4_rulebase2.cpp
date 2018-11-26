#pragma warning(disable : 4996)

#include<stdio.h>
#include "connect4_game.h"

int immediate_win(bool flag) {
	for (int i = 1;i <= 7;i++) {
		if (len[i] == 7) continue;
		if (flag) put_piece(i, true);
		else put_piece(i, false);
		if (get_max_length_connected(i) >= 4) {
			deput_piece(i);
			return i;
		}
		deput_piece(i);
	}
	return 0;
}

int get_winable_lines(bool flag, int x) {
	int tx = len[x], ty = x;
	int ans = 0;

	for (int i = 0; i < 4; i++) {
		int cnt = 1;
		for (int j = 1; j < 4; j++) {
			int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (flag && board[target_x][target_y] == 2) break;
			else if (!flag && board[target_x][target_y] == 1) break;
			else if (cnt++ >= 4) {
				ans++;
				break;
			}
		}
		if (cnt >= 4) continue;
		for (int j = -1; j > -4; j--) {
			int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (flag && board[target_x][target_y] == 2) break;
			else if (!flag && board[target_x][target_y] != 1) break;
			else if (cnt++ >= 4) {
				ans++;
				break;
			}
		}
	}
	return ans;
}

int reasonable_sum_connect(bool flag, int x) {
	int tx = x, ty = len[x];
	int res = 0;
	for (int i = 0;i < 4;i++) {
		int cnt = 1;
		for (int j = 1; j < 4; j++) {
			int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (flag && board[target_x][target_y] == 2) break;
			else if (!flag && board[target_x][target_y] == 1) break;
			cnt++;
		}
		for (int j = -1; j > -4; j--) {
			int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
			if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
			if (flag && board[target_x][target_y] == 2) break;
			else if (!flag && board[target_x][target_y] != 1) break;
			cnt++;
		}

		if (cnt >= 4) {
			int cnt1 = 1;
			for (int j = 1; j < 4; j++) {
				int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
				if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
				if (flag && board[target_x][target_y] != 1) break;
				else if (!flag && board[target_x][target_y] != 2) break;
				cnt1++;
			}
			for (int j = -1; j > -4; j--) {
				int target_x = tx + dx[i] * j, target_y = ty + dy[i] * j;
				if (target_x < 1 || target_x > 7 || target_y < 1 || target_y > 7) break;
				if (flag && board[target_x][target_y] != 1) break;
				else if (!flag && board[target_x][target_y] != 2) break;
				cnt1++;
			}
			res += cnt1 > 4 ? 4 : cnt1;
		}
	}
	return res;
}

int get_position_by_rulebase2() {
	int score[10] = { 0, };

	if (len[4] == 0) return 4;
	else if (int result = immediate_win(true)) return result;
	else if (result = immediate_win(false)) return result;
		

	int maxv = -1000000, ans = 0, sumv = 0;

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 7) continue;
		put_piece(i, false);
		if (int result = putable_both_side(i)) score[i] += 10000 * result;
		deput_piece(i);
	}

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 7) continue;
		put_piece(i, false);
		if (double_three(i)) score[i] += 15000;
		deput_piece(i);
	}

	for (int i = 1;i <= 7;i++) {
		if (len[i] >= 7) continue;

		put_piece(i, true);
		score[i] += get_winable_lines(true, i) + reasonable_sum_connect(true, i);
		if (len[i] < 7) {
			put_piece(i, false);
			if (get_max_length_connected(i) >= 4) {
				deput_piece(i);
				deput_piece(i);
				score[i] = -10000;
				continue;
			}
			score[i] -= (get_winable_lines(false, i) + reasonable_sum_connect(false, i)) / 2;                 //  get_winable_lines(false, i) - reasonable_sum_connect(false, i);   µµ ¼º´ÉÀÌ ±¦Âú°Ô ³ª¿È
			sumv += (get_winable_lines(false, i) + reasonable_sum_connect(false, i)) / 2;
			deput_piece(i);
		}
		deput_piece(i);

		put_piece(i, false);
		score[i] += get_winable_lines(false, i) + reasonable_sum_connect(false, i);
		deput_piece(i);
	}
	for (int i = 1;i <= 7;i++) {
		if (len[i] == 7) continue;
		if (len[i] == 6) score[i] -= sumv / 7;
		if (score[i] > maxv) {
			maxv = score[i];
			ans = i;
		}
	}
	return ans;
}