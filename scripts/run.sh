cd ..
mkdir build
cd build
make clean_all
cmake ..
make
echo "Running the main.cpp"
../bin/interprator_cli
