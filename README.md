# SC3020-ProjectOne

## Contributors

- Ng Xuan Yi
- Siah Wee Hung
- Lee Pei Yee
- Syed Saad

## Installation guide
1. Ensure that your computer has the g++ compiler installed before running our code. You may check if the compiler exists by following the steps below:
- On Mac:
```
g++ --version
```
- On Windows:
```
g++ -v
```

2. git clone this repository to copy the source code.

3. Run the code in a single line in your terminal after git cloning the source code.
```
g++ -std=c++17 "Block Manager/blockManager.cpp" "BPlusTree/bPlusTree.cpp" "BPlusTree/utils.cpp" "Node/node.cpp" "loadData.cpp" "main.cpp" -o main;
```

If no errors arise then the main program can be run using ```./main```