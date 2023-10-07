# SC3020-ProjectOne

## Contributors

Ng Xuan Yi
Siah Wee Hung
Lee Pei Yee
Syed Saad

## Installation guide

If possible, compile all the necessary files using ```make``` and run the main executable using ```./main```.

If there are any issues, the compiled files can be cleaned using ```make clean```, and then recompiled.

If issues arise preventing the use of makefile, the source files can be compiled manually as well with the following commands

1. ```g++ -std=c++17 -c loadData.cpp -IRecord -o loadData.o```
2. ```g++ -std=c++17 -c BPlusTree/utils.cpp -INode/ -IRecord/ -o utils.o```
3. ```g++ -std=c++17 -c Node/node.cpp -IRecord/ -o node.o```
4. ```g++ -std=c++17 -c BPlusTree/bPlusTree.cpp -INode/ -IRecord/ -o bPlusTree.o```
5. ```g++ -std=c++17 -c Block\ Manager/blockManager.cpp -IBPlusTree/ -INode/ -IRecord/ -o blockManager.o```
6. ```g++ -std=c++17 -c main.cpp -IBlock\ Manager / -IBPlusTree -INode/ -IRecord/ -o main.o```
7. ```g++ -std=c++17 -o main main.o blockManager.o bPlusTree.o node.o loadData.o utils.o```


If no errors arise then the main executable can be run using ```./main```