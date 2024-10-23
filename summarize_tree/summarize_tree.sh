#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path>"
    echo "       where <path> is the file or root of the tree you want to summarize."
    exit 1
fi

path=$1

# Check if the path exists and is a directory
if [ ! -d "$path" ]; then
    echo "Error: $path is not a directory or does not exist."
    exit 1
fi

# Count directories and subtract 1 to exclude the root directory itself
num_dirs=$(find "$path" -type d | wc -l | xargs)
num_dirs=$((num_dirs - 1))

# Count regular files
num_regular=$(find "$path" -type f | wc -l | xargs)

# Print the results
echo "Processed all the files from $path."
echo "There were $num_dirs directories."
echo "There were $num_regular regular files."