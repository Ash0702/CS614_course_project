#!/bin/bash

cd /flexclone_mtpt

while true; do
    clear
    echo "======================================================="
    echo "=== Terminal 2: Live Isolation Dashboard ==="
    echo " Controls: Press [u] to update child | Press [q] to quit"
    echo "======================================================="
    
    echo ">> [PARENT FILE - Live State]:"
    cat par
    
    echo "-------------------------------------------------------"
    echo ">> [CHILD FILE - Isolated See-Thru State]:"
    cat child
    echo "-------------------------------------------------------"
    
    # Wait 1 second for a single character input
    read -t 1 -n 1 key
    
    if [[ $key == "u" ]]; then
        echo -e "\n--> ⚡ Running update_chilfs..."
        ./binaries/update_chilfs child
        sleep 1.5 # Pause so you can see the command ran before the screen clears
    elif [[ $key == "q" ]]; then
        echo -e "\n--> Exiting monitor."
        break
    fi
done
