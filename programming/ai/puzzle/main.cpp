#include "puzzle_solver.h"
#include "board_builder.h"

template <size_t SIZE>
void printSolution(const std::vector<Board<SIZE>>&);

int main() {
  const auto kBoardSize = 3;

  const auto fixedSrc = Board<kBoardSize>({1, 8, 2, -1, 4, 3, 7, 6, 5}, {1, 0});
  const auto randomSrc = BoardBuilder<kBoardSize>::createRandomBoard({1, 2, 3, 4, 5, 6, 7, 8});
  const auto dest = Board<kBoardSize>({1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2});

  const auto hammingHeuristic = [](const Board<kBoardSize>& from, const Board<kBoardSize>& to) -> Puzzle8Solver<kBoardSize>::Cost {
    auto diff_counter = 0;
    for (size_t i = 0; i < from.size(); ++i)
      for (size_t j = 0; j < from.size(); ++j)
        if (from(i, j) >= 0 && from(i, j) != to(i, j)) ++diff_counter;

    return diff_counter;
  };

  Puzzle8Solver<kBoardSize> solver(hammingHeuristic);
  solver.solve(randomSrc, dest);

  if (solver.hasSolution()) printSolution(solver.solution());
  else std::cout << "No solution.";

  std::cout << std::endl;

  return 0;
}

template <size_t SIZE>
void printSolution(const std::vector<Board<SIZE>>& path) {
  for (const auto& board: path)
    std::cout << board << "\n\n";
}
