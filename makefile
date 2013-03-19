all:
	gcc -o zipmm main.c compress.c
clean:
	rm -f zipmm
