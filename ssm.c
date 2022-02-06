#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <X11/Xlib.h>

typedef struct {
	char *command;
	int signal;
} Block;

#include "config.h"
#define NUMBER sizeof(blocks) / sizeof(blocks[0])

static char buffer[NUMBER][LENGTH];
static Display *display;

void insert(const Block config, char *block) {
	FILE *output;

	if (output = popen(config.command, "r")) {
		fgets(block, LENGTH, output);
		pclose(output);
	}	
}

void update(void) {
	char status[NUMBER * LENGTH] = {0};

	for (int i = 0; i < NUMBER; i++) {
		strcat(status, buffer[i]);

		if (i < NUMBER - 1)
			strcat(status, DELIMITER);
	}
	XStoreName(display, DefaultRootWindow(display), status);
	XFlush(display); // XSync(display, False)
}

void handler(int signal) {
	for (int i = 0; i < NUMBER; i++) {
		if (blocks[i].signal == signal - SIGRTMIN)
			insert(blocks[i], buffer[i]);
	}
	update();
}

int main(void) {
	if (!(display = XOpenDisplay(NULL))) {
		return 1;
	}
	for (int i = 0; i < NUMBER; i++) {
		if (blocks[i].signal > 0)
			signal(SIGRTMIN + blocks[i].signal, handler);
	}
	while (1) {
		for (int i = 0; i < NUMBER; i++)
			insert(blocks[i], buffer[i]);

		update();
		sleep(INTERVAL);
	}
	XCloseDisplay(display);

	return 0;
}
