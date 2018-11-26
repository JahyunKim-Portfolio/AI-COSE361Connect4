#include "connect4_game.h"


int get_position_by_rulebase() {
	/* put a piece in the middle when my first turn (But it's not the first turn of game)*/
	if (len[4] == 0) return 4;

	/* if I can win the game end after putting a piece in column i, put it in */
	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		put_piece(i, true);
		if (get_max_length_connected(i) >= 4) {
			deput_piece(i);
			return i;
		}
		deput_piece(i);
	}

	/* if I immediately lose the game when the opposite player put a next piece in column i, put it in */
	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		put_piece(i, false);
		if (get_max_length_connected(i) >= 4) {
			deput_piece(i);
			return i;
		}
		deput_piece(i);
	}

	int score[10] = { 0, }, score1[10] = { 0, }, score2[10] = { 0, };

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		put_piece(i, false);
		if (int result = putable_both_side(i)) score[i] += 10000 * result;
		deput_piece(i);
		put_piece(i, true);
		if (int result = putable_both_side(i)) score[i] += 10001 * result;
		deput_piece(i);
	}

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		put_piece(i, false);
		if (double_three(i)) score[i] += 15000;
		deput_piece(i);
		put_piece(i, true);
		if (double_three(i)) score[i] += 15001;
		deput_piece(i);
	}

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		put_piece(i, true);
		for (int j = 0;j < 4;j++) {
			int unlimit = get_unlimit_length_connected(i, j);
			int current = get_current_length_connected(i, j);
			int overflow = get_overflow_length_connected(i, j);
			if (unlimit >= 4) score[i]++;
			if (current == 2 && overflow == 4) score[i] += 2;
			if (current == 3) {
				if (overflow == 4) score[i] += 5;
				else if (overflow == 5) score[i] += 20;
			}
		}
		deput_piece(i);
		put_piece(i, false);
		for (int j = 0;j < 4;j++) {
			int unlimit = get_unlimit_length_connected(i, j);
			int current = get_current_length_connected(i, j);
			int overflow = get_overflow_length_connected(i, j);
			if (unlimit >= 4) score1[i]++;
			if (current == 2 && overflow == 4) score1[i] += 1;
			if (current == 3) {
				if (overflow == 4) score1[i] += 4;
				else if (overflow == 5) score1[i] += 19;
			}
		}
		deput_piece(i);

		if (len[i] >= 5) continue;
		put_piece(i, true);
		put_piece(i, false);
		for (int j = 0;j < 4;j++) {
			int unlimit = get_unlimit_length_connected(i, j);
			int current = get_current_length_connected(i, j);
			int overflow = get_overflow_length_connected(i, j);
			if (unlimit >= 4) score2[i]++;
			if (current == 2 && overflow == 4) score2[i] += 1;
			if (current == 3) {
				if (overflow == 4) score2[i] += 4;
				else if (overflow == 5) score2[i] += 19;
			}
			if (current == 4) score[i] = -1000000;
		}
		deput_piece(i);
		deput_piece(i);
	}

	int maxv = -10000000;
	int ans;

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		if (score[i] + score1[i] > maxv) {
			maxv = score[i] + score1[i];
			ans = i;
		}
	}

	if (score2[ans] == 0) return ans;
	maxv = -10000000;

	for (int i = 1;i <= 7;i++) {
		if (len[i] == 6) continue;
		if (score2[i] == 0) {
			if (score[i] + score1[i] > maxv) {
				maxv = score[i] + score1[i];
				ans = i;
			}
		}
		else if (score[i] * 2 + score1[i] - score2[i] > maxv) {
			maxv = score[i] * 2 + score1[i] - score2[i];
			ans = i;
		}
	}

	return ans;
}