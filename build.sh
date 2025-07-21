!#/bin/bash/
if cmake  -DCMAKE_PREFIX_PATH=~/Qt/6.8.0/gcc_64/ -DCMAKE_CXX_FLAGS="-std=c++17"  -S . -B build; then
    if cmake  --build build --config Release -j8; then  
        ./build/TetroidNES
    else 
        echo "compiler error"
    fi
else 
   echo "cmake error" 
fi