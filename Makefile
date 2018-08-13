

all:
    gcc -o jsonshow jsonshow.c cJSON.c

clean:
    rm -rf *.o jsonshow
