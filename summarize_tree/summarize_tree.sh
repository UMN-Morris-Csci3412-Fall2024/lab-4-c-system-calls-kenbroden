#!/bin/bash

# Check for the correct number of arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path>"
    exit 1
fi

# Store the path argument
path=$1

# Store the path argument if it is a valid directory
if [ ! -d "$path" ]; then
    echo "$path is not a valid directory"
    exit 1
fi

# Count the number of files in the directory
num_dirs=$(find "$path" -type d | wc -l)
num_files=$(find "$path" -type f | wc -l)

# Print the results
echo "There were $num_dirs directories."
echo "There were $num_files regular files."