all:
	gcc -o zipmm main.c compress.c hash_table.c
clean:
	rm -f zipmm
