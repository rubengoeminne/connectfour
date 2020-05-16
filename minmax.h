
#include "board.h"

class Minimax_search{
  public:
    Minimax_search(Board* board, int search_depth, int depth_increase_freq);
    ~Minimax_search();
    int minimax(Board* board, int depth, int alpha, int beta, bool maxplayer);
    int search();
  private:
    Board* board;
    int trials;
    int search_depth;
    int depth_increase_freq;
};
