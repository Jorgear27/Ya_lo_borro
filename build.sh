echo "Build submodule"
cd My_Shell
./build.sh
cd ..

echo "Build project"
./set_env.sh
python3 -m venv venv
source venv/bin/activate
pip install conan
mkdir build
conan install . --build=missing --settings=build_type=Debug
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=./build/Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
