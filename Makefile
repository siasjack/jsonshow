

all:
	gcc -o jsonshow jsonshow.c cJSON.c -lm

clean:
	rm -rf *.o jsonshow
