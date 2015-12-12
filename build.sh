post_compile() {
    if [ -e "./hw8" ]; then
        echo " Done!"
        echo "Run with ./hw8"
    else
        echo " Failed!"
        echo "Check log.txt for details"
    fi
}

if hash clang++ 2>/dev/null; then
    printf "Compiling with clang++ ..."
    clang++ hw8.cpp -o hw8 > log.txt 2>&1
    post_compile
elif hash g++ 2>/dev/null; then
    printf "Compiling with g++ ..."
    g++ hw8.cpp -o hw8 > log.txt 2>&1
    post_compile
else
    echo "This script requires either clang++ or g++ to run. Please install one of these two or use your own command to compile."
fi
