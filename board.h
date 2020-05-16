
#ifndef BOARD_HEADER
#define BOARD_HEADER

class Board{
  public:
    Board(int dim);
    Board(const Board& board);
    ~Board();
  public:
    void display();
    int possible_moves(int* moves);
    bool allowed_move(int column);
    bool winning_move(int column, int player);
    int move(int column, int player);
    void revert_last_move();
    int check_game_over();
    int get_dim();
    int get_nmoves();
    int* get_last_move();
    void set_last_move(int move, int index);
  private:
    bool check_win(int player);
    int dim;
    int** board;
    int nmoves;
    int last_move[2];
};

#endif
