all:	prc_monitor

prc_monitor:	main.o get_info.o
		g++ -std=c++11 main.o get_info.o -o prc_monitor -lprocps -lncurses



main.o:		main.cpp
		g++ -std=c++11 -c main.cpp -lprocps -lncurses



get_info.o:	get_info.cpp
		g++ -std=c++11 -c get_info.cpp -lprocps -lncurses
