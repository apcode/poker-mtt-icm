#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "gflags/gflags.h"
#include "structure.h"
#include "tournament.h"

DEFINE_int64(num_players, -1, "Number of players in tournament");
DEFINE_int64(highest_stack, -1, "Highest stack in tournament");
DEFINE_int64(chips_in_play, -1, "Numbef of chips in play");
DEFINE_double(prizepool, 1.0, "Total prizepool used for equity calculations");
DEFINE_string(tournament_type, "sng-10",
              "Type of tournament payout: sng-10, ps-180");
DEFINE_int64(ntrials, -1, "Number of trials, must be significant");
DEFINE_string(finish_file, "finishes.csv",
              "Store finishes in this csv filename");
DEFINE_string(equity_file, "equity.csv", "Store equities in this csv filename");

using poker::Stacks;
using poker::Tournament;
using poker::TournamentPayouts;

void CheckFlags() {
  if (FLAGS_num_players < 2) {
    std::cerr << "You must have 2 or more players, --num_players=N"
              << std::endl;
    std::exit(1);
  }
  if (FLAGS_highest_stack < FLAGS_num_players) {
    std::cerr << "You must provide the highest stack value, --highest_stack=N"
              << std::endl;
    std::exit(1);
  }
  if (FLAGS_chips_in_play < FLAGS_highest_stack) {
    std::cerr << "You must provide chips_in_play > highest, --chips_in_play=N"
              << std::endl;
    std::exit(1);
  }
  if (FLAGS_highest_stack < FLAGS_chips_in_play / FLAGS_num_players) {
    std::cerr << "Highest must be >= average, --highest=N" << std::endl;
    std::exit(1);
  }
}

void WriteFinishes(std::ostream& os, const Tournament::Results& results) {
  for (const auto& f : results.finishes) {
    auto elem = f.begin();
    os << *elem;
    for (++elem; elem != f.end(); ++elem) {
      os << "," << *elem;
    }
    os << std::endl;
  }
}

void WriteEquity(std::ostream& os, const Tournament::Results& results) {
  for (const auto e : results.equity) {
    os << e << '\n';
  }
  os.flush();
}

int main(int argc, char* argv[]) {

  gflags::ParseCommandLineFlags(&argc, &argv, true);

  auto stacks =
      Stacks(FLAGS_highest_stack, FLAGS_chips_in_play, FLAGS_num_players);
  auto prizes = TournamentPayouts(FLAGS_prizepool, FLAGS_tournament_type,
                                  FLAGS_num_players);

  Tournament tournament(prizes, stacks);
  int ntrials(FLAGS_ntrials);
  if (ntrials < 0) {
    // Set default number of trials
    ntrials = FLAGS_num_players * 1000;
  }

  std::cerr << "Running " << ntrials << " tournaments:\n"
            << "\ttype:\t" << FLAGS_tournament_type << "\n\tnum_players:\t"
            << FLAGS_num_players << "\n\tchips in play\t" << FLAGS_chips_in_play
            << "\n\tprizepol:\t" << FLAGS_prizepool << "\n\tfinishes in:\t"
            << FLAGS_finish_file << "\n\tequity in:\t" << FLAGS_equity_file
            << std::endl;

  auto results = tournament.RunN(FLAGS_ntrials);
  std::ofstream finish(FLAGS_finish_file, std::ofstream::out);
  WriteFinishes(finish, results);
  finish.close();
  std::ofstream equity(FLAGS_equity_file, std::ofstream::out);
  WriteEquity(equity, results);
  equity.close();

  return 0;
}
