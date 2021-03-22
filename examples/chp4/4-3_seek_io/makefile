OBJS = get_num.o seek_io.o
CC = gcc

seek_io.out: $(OBJS)  
	$(CC) $(OBJS) -g -o $@

get_num.o: get_num.c
	$(CC) -c $^

seek_io.o: seek_io.c
	$(CC) -c $^

clean:
	rm -f $(OBJS)