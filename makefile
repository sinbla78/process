CC=gcc
CFLAGS=-Wall

appBoxTargets=appBox1 appBox2 appBox3 appBox4 appBox5 appBox6 appBox7 appBox8 appBox9
linuxProjectTargets=a.out

all: $(appBoxTargets) $(linuxProjectTargets)

$(appBoxTargets): appBox.c
	$(CC) $(CFLAGS) $< -o $@

$(linuxProjectTargets): linuxProject.c linuxProjectFunc.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(appBoxTargets) $(linuxProjectTargets)
