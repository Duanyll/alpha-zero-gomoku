#include <cmath>
#include <iostream>

#include "gomoku.h"

Gomoku::Gomoku(unsigned int n, unsigned int n_in_row, int first_color)
    : n(n),
      n_in_row(n_in_row),
      cur_color(first_color),
      last_move(-1),
      remain_moves(n * n),
      board(n * n, 0) {}

#define BOARD(i, j) this->board[(i)*n + (j)]

std::vector<int> Gomoku::get_legal_moves() {
  auto n = this->n;
  std::vector<int> legal_moves(this->get_action_size(), 0);

  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      if (BOARD(i, j) == 0) {
        legal_moves[i * n + j] = 1;
      }
    }
  }

  return legal_moves;
}

bool Gomoku::has_legal_moves() { return this->remain_moves > 0; }

void Gomoku::execute_move(move_type move) {
  if (this->board[move] != 0) {
    throw std::runtime_error("execute_move borad[i][j] != 0.");
  }

  this->board[move] = this->cur_color;
  this->last_move = move;
  // change player
  this->cur_color = -this->cur_color;
  this->remain_moves--;

  this->status_cache = this->get_status_incremental(move);
}

Gomoku::GameStatus Gomoku::get_game_status() {
  return this->status_cache;
}

const static int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
const static int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};

Gomoku::GameStatus Gomoku::get_status_incremental(move_type move) {
  if (board[move] == 0) {
    return GameStatus::NOT_END;
  }
  auto n = this->n;
  auto n_in_row = this->n_in_row;
  auto x = move / n;
  auto y = move % n;

  int count[8] = {0};    
  for (int i = 0; i < 8; i++) {
    for (int j = 1; j < n_in_row; j++) {
      auto new_x = x + dx[i] * j;
      auto new_y = y + dy[i] * j;
      if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= n) {
        break;
      }
      if (BOARD(new_x, new_y) == BOARD(x, y)) {
        count[i]++;
      } else {
        break;
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    if (count[i] + count[i + 4] + 1 >= n_in_row) {
      return (GameStatus)BOARD(x, y);
    }
  }
  if (this->remain_moves == 0) {
    return GameStatus::DRAW;
  } else {
    return GameStatus::NOT_END;
  }
}

void Gomoku::display() const {
  auto n = this->board.size();

  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      std::cout << BOARD(i, j) << ", ";
    }
    std::cout << std::endl;
  }
}

void Gomoku::to_numpy(int dim1, int dim2, int* data) const {
  auto n = this->n;

  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      data[i * dim2 + j] = BOARD(i, j);
    }
  }
}