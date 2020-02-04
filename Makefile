default :
	clear
	gcc -w gtk_interface.c libbt.a  -o run `pkg-config --cflags gtk+-3.0`  `pkg-config --libs gtk+-3.0`
	./run
	rm run
