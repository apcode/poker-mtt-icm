# Multitable Tournament ICM

Independent Chip Models (ICM) are often used to calculate player
equity for single table tournaments. This model calculates a player's
equity by assuming that it is proportional to their chip stack
relative to the other players, i.e. a players chance of winning is
(their chips / total chips in play).

One problem with this method is that you have to iterate over all
players finishing in all positions. This is quite an expensive
computation, hence it is often limited to single tables, or at most a
small number of tables.

This article provides an approximate way to calculate ICM when there
are many tables and perhaps hundred of players left. This is the case
in large live and online multitable tournaments where many places pay.
The method described uses the basics of ICM using each chip in a
player's stack as being independent and simulating games against other
players where each chip is played for independently. We introduce a
novel approximation method for estimating player stack distributions
for large tournaments.

## Standard ICM Method

The standard way of calculating ICM equity involves iterating through
each player and each position to calculate the probability a player
finishes in each position.

If chips are truelly independent we can calculate the probability of a
player winning as the proportion of the chips in play he holds. We can
then calculate the probability of each player finishing in each
position using this.

1. Calculate the probability of each player winning.
2. Calculate the probability of each player finishing second:
   a. For player A finishing second,
   b. Take the probability of another player (player B) winning.
   c. Calculate the probability of player A winning against the
      remaining players, without player B, and multiply by the 
      probability of player B winning.
   d. Repeat steps b and c for all other players to get the total
      probability of player A finishing second.
3. We can now repeat step 2 for other positions from third onwards,
   always selecting players to finish above player A then player A
   winning against the remaining players.

## MTT Approximate ICM Method

The problem with the standard ICM calculation is that you have to
iterate through all players and positions. So for example to get the
prpbability of a player finishing in the nth position, you must
calculate all combinations of the other players finishing in the 0th
to n-1th positions. This is an expensive computation.

Instead we can estimate the probabilities of every player's finishing
positions by statistically simulating tournaments and recording the
results. 

The procedure to simulate a tournament when we assume that all chips
are independent is:

1. Every player starts with a known number of chips. This can be
   different for every player.
2. Select two players at random.
3. Toss a coin with heads meaning one player wins, tails, the other
   wins.
4. The winning play takes a single chip from the loser.
5. Play until players have no chips hence are knocked out. Repeat
   until one player is left.
   
This procedure is almost by definition what we mean by the independent
chip model. Every simulated tournament is a reasonable estimate of
finishing positions for every player. We just need to simulate a lot
of tournaments and average the results to get a reasonable estimate of
the probabilities of each player finishing in each position.

### Player Starting Stacks

One requirement for simulating ICM tournaments is that we need to know
every player's stack. When there are only ten players we can easily
specify every player's stack. However, when we have potentially
hundreds of players this is not possible.

Instead, what I do is assume that if you order the players by stack
size from largest to smallest, you get roughly an exponential
distribution on average.

By only defining the total chips in play (hence the average) and the
highest stack, we can approximate the distribution of stack sizes for
all players in the tournament.

This calculation is:

    stack[i] = highest_stack * exp(-(i * num_players) / total_chips)

and for highest of X, total of Y and N players t looks like this:

[diagram]

So now with the chip stacks estimated for all players, we can now
simulate the finishing positions for all players.

Of course it is then a simple matter to calculate the equity each
player has:

    equity[p] = sum([prize[f] * prob_finish[f] for f in all_positions])

