mainName="main"
rm $mainName
files="$mainName.cpp "`find ./lib -type f -name "*.cpp"`
echo $files
flags="-rdynamic -lstdc++ -lpthread -lm -Winline -O1 -std=c++17"
clang $files $flags -o $mainName
./main
