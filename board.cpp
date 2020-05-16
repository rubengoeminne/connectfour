
#include <iostream>
#include "board.h"
#include "constants.cpp"
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */


Board::Board(int dim) : last_move {-1, -1}{
  int** board = new int*[dim];
  for(int i=0;i<dim;i++){
    board[i] = new int[dim];
    for(int j=0;j<dim;j++){
      board[i][j] = 0;
    }
  }
  this->nmoves = 0;
  this->dim = dim;
  this->board = board;
}

Board::Board(const Board& board){
  this->dim = board.dim;
  this->nmoves = board.nmoves;
  copy(begin(board.last_move), end(board.last_move), begin(this->last_move));
  this->board = new int*[this->dim];
  for(int i=0;i<this->dim;i++){
    this->board[i] = new int[this->dim];
    for(int j=0;j<this->dim;j++){
      this->board[i][j] = board.board[i][j];
    }
  }
}


Board::~Board(){
  int dim = this->dim;
  for(int i=0;i<dim;i++){
    delete[] this->board[i];
  }
  delete[] this->board;
}

int Board::possible_moves(int* moves){
  int move_counter = 0;
  for(int i=0;i<this->dim;i++){
    if(this->board[this->dim-1][i] == 0){
      moves[move_counter] = i;
      move_counter++;
    }
  }
  return move_counter;
}

bool Board::allowed_move(int column){
  if(column >= this->dim || this->board[this->dim-1][column] != 0){
    return false;
  }
  return true;
}

bool Board::winning_move(int column, int player){
  if(this->move(column, player)){
    //cout << "game state for col " << column << ", player " << player << ": " << this->check_game_over() << endl;
    if(this->check_game_over() == player){
      return true;
    }
    this->revert_last_move();
  }
  return false;
}


// return 1 if succesfull moves
// return 0 if illegal move
int Board::move(int column, int player){
  if(this->allowed_move(column)){
    int row = 0;
    while(this->board[row][column] != 0){
      row++;
    }
    this->board[row][column] = player;
    this->last_move[0] = row;
    this->last_move[1] = column;
    this->nmoves++;
    return 1;
  }
  return 0;
}

int Board::get_dim(){
  return this->dim;
}

int Board::get_nmoves(){
  return this->nmoves;
}

int* Board::get_last_move(){
  return this->last_move;
}

void Board::set_last_move(int move, int index){
  this->last_move[index] = move;
}

void Board::revert_last_move(){
  this->board[this->last_move[0]][this->last_move[1]] = 0;
  this->last_move[0] = -1;
  this->last_move[1] = -1;
  this->nmoves--;
}

bool Board::check_win(int player){
  int** b = this->board;

  for(int i=0;i<this->dim;i++){
    for(int j=0;j<=this->dim-WIN_LENGTH;j++){
      // Check rows
      if(b[i][j] == player && b[i][j+1] == player && b[i][j+2] == player && b[i][j+3] == player){
        return true;
      }
      // Check columns
      if(b[j][i] == player && b[j+1][i] == player && b[j+2][i] == player && b[j+3][i] == player){
        return true;
      }
    }
  }

  // d: offset from diagonal
  for(int d=-this->dim+WIN_LENGTH; d<=this->dim-WIN_LENGTH;d++){
    for(int i=max(0,-d);i<=min(this->dim,this->dim-d)-WIN_LENGTH;i++){
      // Check first diagonal
      if(b[i][this->dim-(i+d)-1] == player && b[i+1][this->dim-(i+d+1)-1] == player && b[i+2][this->dim-(i+d+2)-1] == player && b[i+3][this->dim-(i+d+3)-1] == player){
        return true;
      }
      // Check second diagonal
      if(b[i][i+d] == player && b[i+1][i+1+d] == player && b[i+2][i+2+d] == player && b[i+3][i+3+d] == player){
        return true;
      }
    }
  }

  return false;
}


// -1 : player -1 won
// 1 : player 1 won
// 0 : game is not over
// 2 : draw
int Board::check_game_over(){

  // Check for win
  if(this->check_win(1)){
    return 1;
  }
  else if(this->check_win(-1)){
    return -1;
  }
  else{
    // Check for draw
    int unplaced_top_pieces = 0;
    for(int i=0;i<this->dim;i++){
      if(this->board[this->dim-1][i] == 0){
        unplaced_top_pieces++;
      }
    }
    if(unplaced_top_pieces == 0){
      return 2;
    }
  }
  return 0;
}

void Board::display(){
  cout << "  ";
  for(int i=0;i<this->dim;i++){
    cout << "# " ;
  }
  cout << endl;

  for(int i=this->dim-1;i>=0;i--){
    cout << "# ";
    for(int j=0;j<this->dim;j++){
      if(this->board[i][j] == 0){
        cout << "  ";
      }
      else{
        if(this->board[i][j] == 1){
          cout << RED << "o " << RESET;
        }
        else{
          cout << BLUE << "x " << RESET;
        }
      }
    }
    cout << "#" << endl;
  }

  cout << "  ";
  for(int i=0;i<this->dim;i++){
    cout << "# " ;
  }
  cout << endl;
}
