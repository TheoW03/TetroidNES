#!/bin/bash

build() {
    # Define the list of ROM build commands
    cmake -S . -B build -DUNITTESTS=ON

    mkdir -p ./src/unitTests/TestRoms/
    RomList=(
        "nya~"
        "./src/unitTests/TestRomsSource/LDATest.s -o ./src/unitTests/TestRoms/LDATest.nes"
        "./src/unitTests/TestRomsSource/JMPTest.s -o ./src/unitTests/TestRoms/JMPTest.nes"
        "./src/unitTests/TestRomsSource/TestASL.s -o ./src/unitTests/TestRoms/TestAsl.nes"
    )

    echo ""

    # Iterate over each command in the RomList
    for p in "${RomList[@]}"; do
        # Split the command string into an array
        args=($p)
        
        # Run the cl65 command with the appropriate arguments
        if cl65 --target nes "${args[@]}"; then
            echo "Built ${args[2]} successfully"
        else
            echo "Built ${args[2]} unsuccessfully"
        fi

        echo ""
    done
}

# Call the build function
build
