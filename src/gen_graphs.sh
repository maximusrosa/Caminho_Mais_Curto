#!/bin/bash

# Output directories
OUTPUT_DIR="input"
DIMACS_DIR="$OUTPUT_DIR/random"
mkdir -p "$DIMACS_DIR"

# Path to the executable
gen_graph="./ref/gen"

# Check if the executable exists
if [[ ! -f $gen_graph ]]; then
    echo "Error: The executable '$gen_graph' was not found. Make sure it is in the current directory."
    exit 1
fi

# Generate graphs with m=2^12 and n=(2th_root(2))^i, i in [13, 27]
M=$((2**12))
echo "Generating graphs for m=$M with varying n"
for i in {13..27}; do
    N=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando N
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/graph_n${N}_m${M}.dimacs"
    echo "Executing: $gen_graph $N $P > $FILENAME"
    $gen_graph $N $P > $FILENAME
done

# Generate graphs with n=2^15 and m=sqrt(2)^i, i in [31, 45]
N=$((2**15))
echo "Generating graphs for n=$N with varying m"
for i in {31..45}; do
    M=$(echo "scale=10; e( ($i/2) * l(2) )" | bc -l | awk '{print int($1)}')  # Truncando M
    P=$(echo "$M / ($N * ($N - 1))" | bc -l)
    FILENAME="$DIMACS_DIR/graph_n${N}_m${M}.dimacs"
    echo "Executing: $gen_graph $N $P > $FILENAME"
    $gen_graph $N $P > $FILENAME
done


echo "All graphs have been generated in the directory: $DIMACS_DIR"