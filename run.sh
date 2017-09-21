#!/bin/bash -v
# Typical usage

./bazel-bin/mtt_icm \
    --num_players 100 \
    --highest_stack 3000 \
    --chips_in_play 100000 \
    --prizepool 100 \
    --tournament_type "sng-10" \
    --ntrials 100000 \
    --finish_file "sng10-100p-finish.csv" \
    --equity_file "sng10-100p-equity.csv"

