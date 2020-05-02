
all:
	make --directory src

clean:
	rm -f src/*.o
	rm -f src/ray

.PHONY : all clean

