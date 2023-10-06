CC = g++
CFLAGS = -Wall -g -std=c++17

main: main.o blockManager.o bPlusTree.o node.o loadData.o
	$(CC) $(CFLAGS) -o main main.o blockManager.o bPlusTree.o node.o loadData.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -IBlock\ Manager / -IBPlusTree -INode/ -IRecord/ -o main.o

bPlusTree.o:
	$(CC) $(CFLAGS) -std=c++17 -c BPlusTree/bPlusTree.cpp -INode/ -IRecord/ -o bPlusTree.o

blockManager.o:
	$(CC) $(CFLAGS) -std=c++17 -c Block\ Manager/blockManager.cpp -IBPlusTree/ -INode/ -IRecord/ -o blockManager.o

node.o:
	$(CC) $(CFLAGS) -c Node/node.cpp -IRecord/ -o node.o

loadData.o:
	$(CC) $(CFLAGS) -c loadData.cpp -IRecord -o loadData.o

clean: 
	$(RM) main main.o blockManager.o bPlusTree.o node.o loadData.o