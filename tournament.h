#ifndef POKER_TOURNAMENT_H_
#define POKER_TOURNAMENT_H_

#include <vector>

namespace poker {

class Tournament {
 public:
  struct Results {
    Results(int num_players)
        : finishes(num_players, {num_players, 0}), equity(num_players, 0.0) {}
    std::vector<std::vector<double>> finishes;
    std::vector<double> equity;
  };

  Tournament(const std::vector<double>& prizes, const std::vector<int>& stacks)
      : num_players_(stacks_.size()), prizes_(prizes_), stacks_(stacks) {}

  // RunOne runs a tournament one time.
  // Return:
  //   The finishing positions for each player, 0-offset.
  std::vector<int> RunOne();

  //  RunN: run a tournament N times returing collective results.
  // Args:
  //   n_trials: number of runs of tournament.
  // Return:
  //   Results containing counts of finishing positions for each player
  //     and total equity won in N tournaments.
  Results RunN(int n_trials);

  const int NumPlayers() const { num_players_; }
  const std::vector<double> Prizes() const { return prizes_; }
  const std::vector<int> StartingStacks() const { return stacks_; }

 private:
  // Worker to run in thread for RunN
  Results RunNImpl(int n_trials);

  const int num_players_;
  const std::vector<double> prizes_;
  const std::vector<int> stacks_;
};

}  // namespace poker

#endif /* POKER_TOURNAMENT_H_ */
