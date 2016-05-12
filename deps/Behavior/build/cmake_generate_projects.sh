
echo please visit http://www.behaviac.com/docs/zh/articles/build/ for more information
echo ---------------------------------------------------------------------------------

mkdir cmake_binary
cd cmake_binary

# --------------------------------------------------------------
mkdir linux
cd linux

echo ---------------------------------------------------------------------------------
mkdir debug
cd debug
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_USE_64BITS=ON --build ../../../..
cd ..

echo ---------------------------------------------------------------------------------
mkdir release
cd release
# cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBEHAVIAC_VERSION_MODE=ForeUseRelease -DBUILD_USE_64BITS=ON --build ../../../..
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_USE_64BITS=ON --build ../../../..
cd ..

# linux
cd ..
# --------------------------------------------------------------
# mkdir sublime
# cd sublime
# cmake -G "Sublime Text 2 - Unix Makefiles" --build ../../..
# cd ..

# back cmake_binary
cd ..
