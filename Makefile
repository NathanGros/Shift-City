make :
	gcc main.c structures.c drawing.c -o main.exe -Wall -lraylib -lm
	./main.exe
