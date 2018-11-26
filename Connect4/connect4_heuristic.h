#define MAXDEPTH 12
#define INFI 100000000

int maxf(int x, int y);

void winning_count(int board[][10], int player, int value[]);

int win_check(int x, int y, int board[][10]);

int negamax_solver(int depth, int board[][10], int turn, int alpha, int beta, int *best_x, int put_x, int put_y);

int get_position_by_heuristic();
