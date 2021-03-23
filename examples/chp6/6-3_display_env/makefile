TARGET = display_env.out get_env.out modify_env.out
 
all: $(TARGET)

display_env.out: display_env.c
	gcc -o $@ $^

get_env.out: get_env.c
	gcc -o $@ $^

modify_env.out: modify_env.c 
	gcc -o $@ $^

clean:
	rm $(TARGET)