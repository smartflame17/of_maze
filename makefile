CC = gcc
target = 20201572
object = 20201572.c

$(target): $(object)
	$(CC) -g $(object) -o $(target)
clean:
	rm $(target)
