#include "tournament.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <random>
#include <future>

namespace poker {

std::vector<int> Tournament::RunOne() {
  std::mt19937 gen_;
  gen_.seed(static_cast<uint32_t>(time(NULL)));
  std::uniform_real_distribution<> draw(0, 1);
  std::uniform_int_distribution<> select_player(0, stacks_.size());
  using param_type = typename std::uniform_int_distribution<>::param_type;

  std::vector<int> stacks(stacks_);
  std::vector<int> players(num_players_);
  std::iota(players.begin(), players.end(), 0);
  std::vector<int> finishes(num_players_, 0);

  while (players.size() > 1) {
    // select player 1
    auto p1 = players[select_player(gen_, param_type(0, players.size()))];
    // select opponent
    auto p2 = p1;
    while (p2 == p1) {
      p2 = players[select_player(gen_, param_type(0, players.size()))];
    }
    const auto win_prob = 0.5;
    int loser(-1);
    // play off
    if (draw(gen_) < win_prob) {
      // p1 wins
      ++stacks[p1];
      --stacks[p2];
      loser = p2;
    } else {
      // p2 wins
      --stacks[p1];
      ++stacks[p2];
      loser = p1;
    }
    if (stacks[loser] < 1) {
      // loser is out
      finishes[loser] = players.size() - 1;
      // TODO: better data structure that can remove and select random element
      // in O(1). Though we only do this once for each player, when they're
      // knocked out. So we want more efficient player selection.
      players.erase(std::remove(players.begin(), players.end(), loser),
                    players.end());
    }
  }
  // one player left
  finishes[players[0]] = 0;
  return finishes;
}

Tournament::Results Tournament::RunN(int n_trials) {
  assert(n_trials > 0);
  auto n_threads = std::thread::hardware_concurrency();
  if (n_trials < n_threads) {
    n_threads = 1;
  }
  auto trials_per_thread = n_trials / n_threads;
  std::vector<std::future<Tournament::Results>> workers;
  for (int i(n_threads), nt(n_trials); i > 0; --i, nt -= trials_per_thread) {
    if (i == 1) {
      // final thread, do remaining
      trials_per_thread = nt;
    }
    workers.emplace_back(std::async(
        std::launch::async, [=]() { return RunNImpl(trials_per_thread, i); }));
  }

  // Merge results
  Results results(num_players_);
  for (auto& w : workers) {
    auto result = w.get();
    for (int p(0); p < num_players_; ++p) {
      for (int f(0); f < num_players_; ++f) {
        results.finishes[p][f] += result.finishes[p][f];
      }
      results.equity[p] += result.equity[p];
    }
  }
  // Normalise results by n_trials
  for (int p(0); p < num_players_; ++p) {
    for (int f(0); f < num_players_; ++f) {
      results.finishes[p][f] /= n_trials;
    }
    results.equity[p] /= n_trials;
  }

  return results;
}

Tournament::Results Tournament::RunNImpl(int n_trials, int thread) {
  Results results(num_players_);
  for (int t(0); t < n_trials; ++t) {
    std::cout << thread << "-" << t << std::endl;
    const auto finishes = RunOne();
    // merge results
    for (int p(0); p < num_players_; ++p) {
      auto finish = finishes[p];
      results.finishes[p][finish] += 1.0;
      if (finish < prizes_.size()) {
        results.equity[p] += prizes_[finish];
      }
    }
  }
  return results;
}

}  // namespace poker
