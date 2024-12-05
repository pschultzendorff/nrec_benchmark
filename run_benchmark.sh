# !/bin/bash
chdir /workspaces/nrec_benchmark

# Specify the C# file.
CPP_FILE="benchmark.cpp"
EXE_FILE="benchmark"

# Check if the C# file exists
if [ ! -f "$CPP_FILE" ]; then
    echo "C# file '$CPP_FILE' not found!"
    exit 1
fi

# Compile the C++ file.
echo "Compiling $CPP_FILE..."
g++ $CPP_FILE -o $EXE_FILE


# Run the program if compilation succeeds.
if [ $? -eq 0 ]; then
    echo "Running the program..."

    # Create a timestamp for the results file
    TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
    RESULTS_FILE="results_$TIMESTAMP.txt"
    ./$EXE_FILE > $RESULTS_FILE

    echo "Results saved to $RESULTS_FILE"
else
    echo "Compilation failed. Check for errors in your code."
    exit 1
fi
