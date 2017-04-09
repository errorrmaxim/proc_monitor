all:	prc_monitor

prc_monitor:	main.o get_info.o
		g++ -std=c++11 main.o get_info.o -o prc_monitor -L. -lprocps -lncurses



main.o:		main.cpp
		g++ -std=c++11 -c main.cpp -L. -lprocps -lncurses



get_info.o:	get_info.cpp
		g++ -std=c++11 -c get_info.cpp -L. -lprocps -lncurses
