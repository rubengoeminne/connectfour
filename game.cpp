
#include <iostream>
#include <random>

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

#include "board.cpp"
#include "minmax.cpp"
#include "constants.cpp"
using namespace std;


int random_number(int max){
  std::uniform_int_distribution<int> uni(0,max);
  int rnd = uni(rng);
  return rnd;
}

int main() {

  Board* board = new Board(DIM);
  Minimax_search* minimax;
  int legal_move;
  int player;
  int rnd;
  int search_depth = 6;
  int depth_increase_freq = 8;

  board->display();

  player = 1;
  while(board->check_game_over() == 0){
    do{
      if (player == 1){
        cout << "Select move (int)" << endl;
        cin >> rnd;
      }
      else{
        // Random move: this is a very bad AI
        //rnd = random_number(DIM-1);
        // Minmax algorithm
        minimax = new Minimax_search(board, search_depth, depth_increase_freq);
        rnd = minimax->search();
      }
      legal_move = board->move(rnd, player);
    } while(!legal_move);
    player *= -1;

    board->display();
  }

  switch (board->check_game_over()) {
    case 1:
      cout << "Player 1 wins!" << endl;
      break;
    case -1:
      cout << "Player 2 wins!" << endl;
      break;
    case 2:
      cout << "It's a draw!" << endl;
      break;
  }



  /*
  int* moveset = new int[DIM];
  int moveset_len = board->possible_moves(moveset);

  cout << "Moveset_len: " << moveset_len << endl;
  cout << "Moveset" << endl;
  for(int i=0;i<moveset_len;i++){
    cout<<moveset[i]<<" ";
  }
  cout << endl;

  delete[] moveset;
  */

  return 0;
}
