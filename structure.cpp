#include "structure.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace poker {

std::vector<int> Stacks(int highest, int chips, int num_players) {
  double c = static_cast<double>(num_players) / chips;
  int n = 0;
  std::vector<int> players(num_players);
  std::generate(players.begin(), players.end(), [ highest, c, &n ]()->int {
    auto stack = static_cast<int>(highest * std::exp(-(n++ * c)) + 0.5);
    return std::max(stack, 1);
  });
  return players;
}

std::vector<double> TournamentPayouts(double prizepool,
                                      const std::string& tournament_type,
                                      int num_players) {
  std::vector<double> prizes;
  if (tournament_type == "sng-10") {
    // At least two opponents, three total
    assert(num_players > 1);
    for (auto p : {0.5, 0.3, 0.2}) {
      prizes.emplace_back(p * prizepool);
    }
  }
  return prizes;
}

}  // namespace poker
