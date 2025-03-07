#!/bin/bash

# Change to the build directory
rm -rf build
mkdir build
cd build

# Run the build commands
cmake .. && make && ./bin/Cathedral

# Store the exit status of the last command
EXIT_STATUS=$?

# Change back to the cathedral directory regardless of success or failure
cd ..

# Exit with the same status as the build/run command
exit $EXIT_STATUS