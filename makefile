# CC = g++
CC = x86_64-w64-mingw32-g++
CFLAGS = -Wall -g -std=c++17

main: main.o blockManager.o bPlusTree.o node.o loadData.o utils.o
	$(CC) $(CFLAGS) -o main main.o blockManager.o bPlusTree.o node.o loadData.o utils.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -I "Block Manager" -I "BPlusTree" -I Node/ -I Record/ -o main.o

bPlusTree.o:
	$(CC) $(CFLAGS) -c BPlusTree/bPlusTree.cpp -I Node/ -I Record/ -o bPlusTree.o

blockManager.o:
	$(CC) $(CFLAGS) -c "Block Manager/blockManager.cpp" -I "BPlusTree" -I "Node" -I "Record" -o blockManager.o

node.o:
	$(CC) $(CFLAGS) -c Node/node.cpp -I Record/ -o node.o

utils.o:
	$(CC) $(CFLAGS) -c BPlusTree/utils.cpp -I Node/ -I Record/ -o utils.o

loadData.o:
	$(CC) $(CFLAGS) -c loadData.cpp -I Record -o loadData.o

clean: 
	$(RM) main main.o blockManager.o bPlusTree.o node.o loadData.o utils.o