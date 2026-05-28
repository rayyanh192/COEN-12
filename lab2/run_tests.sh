#!/bin/bash

# Define the list of test files
test_files=(
  "/scratch/coen12/GreenEggsAndHam.txt"
  "/scratch/coen12/Macbeth.txt"
  "/scratch/coen12/Genesis.txt"
  "/scratch/coen12/HoundOfTheBaskervilles.txt"
  "/scratch/coen12/TheWarOfTheWorlds.txt"
  "/scratch/coen12/TreasureIsland.txt"
  "/scratch/coen12/TheSecretGarden.txt"
  "/scratch/coen12/TwentyThousandLeagues.txt"
  "/scratch/coen12/TheCountOfMonteCristo.txt"
  "/scratch/coen12/Bible.txt"
)

# Define the programs to test
programs=(
  "./unique_unsorted"
  "./unique_sorted"
  "./parity_unsorted"
  "./parity_sorted"
)

# Output file for results
output_file="results.txt"

# Initialize the output file
echo "Test Results" > "$output_file"
echo "" >> "$output_file"

# Loop through each test file
for test_file in "${test_files[@]}"; do
  if [[ -f "$test_file" ]]; then
    echo "Testing with $test_file" | tee -a "$output_file"
    for program in "${programs[@]}"; do
      if [[ -x "$program" ]]; then
        echo -n "Running $program with $(basename "$test_file"): " | tee -a "$output_file"
        # Measure execution time
        { time $program "$test_file"; } 2>&1 | grep real | tee -a "$output_file"
      else
        echo "$program is not executable" | tee -a "$output_file"
      fi
    done
    echo "" >> "$output_file"
  else
    echo "$test_file does not exist" | tee -a "$output_file"
  fi
done

echo "Testing complete. Results saved to $output_file."