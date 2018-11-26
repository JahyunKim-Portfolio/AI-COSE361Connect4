extern int board[10][10], len[10], turn, dx[4], dy[4];

void print_board();

void put_piece(int, bool);

void deput_piece(int);

int putable_both_side(int);

bool double_three(int);

int get_max_length_connected(int);

int get_unlimit_length_connected(int, int);

int get_overflow_length_connected(int, int);

int get_current_length_connected(int, int);