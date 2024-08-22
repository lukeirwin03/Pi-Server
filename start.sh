#!/bin/bash

echo "Starting Server..."

# Fetch private IP using hostname
port=7703 # CHANGE THIS TO BE WHATEVER PORT YOU'D LIKE
private_ip=$(hostname -I | awk '{print $1}')
echo "Your server will be running on: $private_ip:$port"

# Compile the C program
gcc -o server server.c

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Start the server in the background
    ./server $private_ip $port &
    server_pid=$!
    echo "Server running with PID: $server_pid"

    # Define a cleanup function
    cleanup() {
        echo "Shutting down the server with PID: $server_pid..."
        kill -SIGINT "$server_pid" || kill -9 "$server_pid"
        wait "$server_pid" 2>/dev/null
        echo "Server has been shut down."
        exit 0
    }

    # Trap SIGINT (Ctrl+C) and call the cleanup function
    trap cleanup SIGINT

    # Wait for the server process to finish
    wait "$server_pid"
else
    echo "Compilation failed."
    echo "Shutting down."
fi
