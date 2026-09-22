#!/usr/bin/env fish

set COUNTS
set FLAGS
set BENCH

for arg in $argv
    switch $arg
        case --bench
            set BENCH --bench
        case --simple --medium --complex --adaptive
            set FLAGS $FLAGS $arg
        case '*'
            set COUNTS $COUNTS $arg
    end
end

if test (count $COUNTS) -eq 0
    set COUNTS 16
end

if test (count $FLAGS) -eq 0
    set FLAGS --adaptive
end

make -C (dirname (status filename))

for count in $COUNTS
    set ARG (shuf -i 0-$count -n $count)
    ./push_swap $FLAGS $BENCH $ARG
    echo ""
end