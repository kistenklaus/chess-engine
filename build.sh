mkdir build -p
cd build
cmake ..
mv compile_commands.json ../
cd ..
make -C build --no-print-directory
