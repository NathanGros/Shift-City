make :
	gcc main.c structures.c drawing.c input_management.c city_manipulation.c objectives.c -o main.exe -Wall -lraylib -lm
	./main.exe
