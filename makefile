CC=gcc
CFLAGS=-Wall

appBlockTargets=appBox1 appBox2 appBox3 appBox4 appBox5 appBox6 appBox7 appBox8 appBox9
linuxProjectTargets=a.out

all: $(appBlockTargets) $(linuxProjectTargets)

$(appBlockTargets): appBlock.c
	$(CC) $(CFLAGS) $< -o $@

$(linuxProjectTargets): linuxProject.c linuxProjectFunc.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(appBlockTargets) $(linuxProjectTargets)
