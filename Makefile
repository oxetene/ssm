PREFIX = /usr/local/bin

install:
	gcc ssm.c -l X11 -o ssm
	mkdir -p $(PREFIX)
	install -m 0755 ssm $(PREFIX)/ssm

uninstall:
	rm -f $(PREFIX)/ssm
