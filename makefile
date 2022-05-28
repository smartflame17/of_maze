CC = gcc
target = a.out
object = 20201572.c

$(target): $(object)
	$(CC) -o $(target) $(object)
clean:
	rm $(target)
