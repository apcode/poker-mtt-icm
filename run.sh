#!/bin/bash -v
# Typical usage

./bazel-bin/mtt_icm \
    --num_players 10 \
    --highest_stack 3000 \
    --chips_in_play 10000 \
    --prizepool 100 \
    --tournament_type "sng-10" \
    --ntrials 10000 \
    --finish_file "sng-10-finish.csv" \
    --equity_file "sng-10-equity.csv"

