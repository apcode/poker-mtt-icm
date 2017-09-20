#ifndef POKER_STRUCTURE_H_
#define POKER_STRUCTURE_H_

#include <string>
#include <vector>

namespace poker {

std::vector<int> Stacks(int highest, int chips, int num_players);

std::vector<double> TournamentPayouts(double prizepool,
                                      const std::string& tournament_type,
                                      int num_players);

const char kTournamentTypes[] = {"sng-10"};

}  // namespace poker

#endif /* POKER_STRUCTURE_H_ */
