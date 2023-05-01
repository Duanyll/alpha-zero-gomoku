#pragma once

#include <tuple>
#include <vector>

class Gomoku {
public:
  using move_type = int;
  using board_type = std::vector<char>;

  Gomoku(unsigned int n, unsigned int n_in_row, int first_color, bool allow_unusual_beginning = false);

  enum class GameStatus {
    NOT_END = 0,
    FIRST_WIN = 1,
    SECOND_WIN = -1,
    DRAW = 2,
  };

  bool has_legal_moves();
  std::vector<int> get_legal_moves();
  void execute_move(move_type move);
  GameStatus get_game_status();
  void display() const;
  void to_numpy(int dim1, int dim2, int* data) const;

  inline unsigned int get_action_size() const { return this->n * this->n; }
  inline board_type get_board() const { return this->board; }
  inline move_type get_last_move() const { return this->last_move; }
  inline int get_current_color() const { return this->cur_color; }
  inline unsigned int get_n() const { return this->n; }

private:
  board_type board;      // game borad
  unsigned int n;        // board size
  unsigned int n_in_row; // 5 in row or else
  bool allow_unusual_beginning; // allow the first three moves to be anywhere

  int cur_color;          // current player's color
  move_type last_move;    // last move

  int remain_moves;       // remaining moves

  GameStatus status_cache = GameStatus::NOT_END;
  GameStatus get_status_incremental(move_type move);
};
