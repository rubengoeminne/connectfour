
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include "minmax.h"
#include "constants.cpp"
using namespace std;


Minimax_search::Minimax_search(Board* board, int search_depth, int depth_increase_freq):board(board){
  this->trials = 0;
  this->search_depth = search_depth;
  this->depth_increase_freq = depth_increase_freq;
}


Minimax_search::~Minimax_search(){}

int Minimax_search::search(){

  cout << "Starting search" << endl;
  srand(time(0));

  int* moveset = new int[DIM];
  int moveset_len = this->board->possible_moves(moveset);
  int best_move, best_score, score;
  int last_move_0, last_move_1;
  best_score = -DIM*DIM;
  int best_score_degen = 0;
  int depth = this->search_depth + board->get_nmoves()/this->depth_increase_freq;
  //cout << "Depth: " << depth << endl;

  // Do minmax on search of the child nodes
  // AI is player -1 -> maximizing player == -1 !!
  auto start = chrono::high_resolution_clock::now();
  for(int i=0;i<moveset_len;i++){
    Board* newboard = new Board(*this->board);
    newboard->move(moveset[i], -1);
    // Child nodes are evaluated -> minimizing player
    score = this->minimax(newboard, depth, -DIM*DIM, DIM*DIM, false);
    //cout << score << endl;
    if(score == best_score){
      best_score_degen++;
    }
    if(score > best_score){
      best_score_degen = 1;
      best_score = score;
      best_move = moveset[i];
    }
    delete newboard;
  }
  if(best_score_degen == DIM){
    best_move = rand() % DIM;
  }
  auto stop = chrono::high_resolution_clock::now();
  cout << this->trials << " moves generated in " << chrono::duration_cast<chrono::milliseconds>(stop - start).count()  << " milliseconds"<< endl;
  return best_move;
}


int Minimax_search::minimax(Board* board, int depth, int alpha, int beta, bool maxplayer){
  int maxeval, mineval, eval;
  Board* newboard;
  this->trials++;

  // If maxdepth -> 0 score
  if(depth == 0){
    return 0;
  }

  // Static evaluation of the finished game
  // draw -> score == 0
  int board_result = board->check_game_over();
  if(board_result == 2){
    return 0;
  }
  // Win -> score == amount of free spaces left on the board
  // Player -1 wins -> positive score
  else if(board_result == -1 || board_result == 1){
    return -(DIM*DIM - board->get_nmoves() + 1) * board_result;
  }
  else{
    // Maximizing player (player -1)
    if(maxplayer){
      maxeval = -DIM*DIM;
      for(int i=0;i<board->get_dim();i++){
        if(board->allowed_move(i)){
          newboard = new Board(*board);
          newboard->move(i, -1);
          eval = this->minimax(newboard, depth-1, alpha, beta, false);
          delete newboard;
          if(eval > maxeval){
            maxeval = eval;
          }
          // alpha-beta pruning
          if(eval > alpha){
            alpha = eval;
          }
          if(beta <= alpha){
            break;
          }
        }
      }
      return maxeval;
    }
    // Minimizing player (player 1)
    else{
      mineval = DIM*DIM;
      for(int i=0;i<board->get_dim();i++){
        if(board->allowed_move(i)){
          newboard = new Board(*board);
          newboard->move(i, 1);
          eval = this->minimax(newboard, depth-1, alpha, beta, true);
          delete newboard;
          if(eval < mineval){
            mineval = eval;
          }
          // alpha-beta pruning
          if(eval < beta){
            beta = eval;
          }
          if(beta <= alpha){
            break;
          }
        }
      }
      return mineval;
    }
  }
}








/*
int odd_even(int nr){
  if(nr%2 == 0){
    return 1;
  }
  return -1;
}

int Minmax::negamax(Board* board){

  this->trials += 1;
  //cout << "trials: " << this->trials << endl;

  //board->display();
  //cout << "Nmoves: " << board->get_nmoves() << endl;
  int curr_player = odd_even(board->get_nmoves());
  //cout << "current: " << curr_player << endl;

  // Draw
  if(board->get_nmoves() == DIM*DIM){
    //cout << "Draw" << endl;
    return 0;
  }

  // Can the agent win?
  for(int i=0;i<board->get_dim();i++){
    if(board->winning_move(i, curr_player)){
      //cout << "Winning move for " << curr_player << ", score: " << (DIM*DIM + 1 - board->get_nmoves()) << endl;
      //cout << "moves: " << board->get_nmoves() << endl;
      //this->board->display();
      return (DIM*DIM + 1 - board->get_nmoves());
    }
  }

  int best_score = -DIM*DIM;
  int score;
  for(int i=0;i<board->get_dim();i++){
    if(board->allowed_move(i)){
      Board* newboard = new Board(*board);
      newboard->move(i, curr_player);
      score = -this->negamax(newboard);
      if(score > best_score){
        best_score = score;
      }
      delete newboard;
    }
  }
  return best_score;
}
*/
