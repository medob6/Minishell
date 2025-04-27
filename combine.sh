#!/bin/bash

# Define the characters
chars=("<" ">" "|" '"' "'" "&" "$" "W" " " '(' ')')

# Length of combinations
length=4  # Change this value for longer or shorter combinations

# Function to generate combinations
generate_combinations() {
    local current=$1
    local depth=$2

    if [ "$depth" -eq 0 ]; then
        echo "$current" | ./bash_prompt > /dev/null
        if [ $? -ne 0 ]; then 
            echo "$current"
            exit 1
        fi
        return
    fi

    for char in "${chars[@]}"; do
        generate_combinations "$current$char" $((depth - 1))
    done
}

# Generate combinations
generate_combinations "" "$length"
