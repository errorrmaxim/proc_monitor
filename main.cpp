#include "get_info.h"


int main(int argc, char *argv[])
{
	//main vector for our process class
    vector<proc_info>main_vec = proc_vector();

	//sort functor
    sort_fctr s;


    initscr();
    noecho();
    nodelay(stdscr, true);

    int c, fg;
    while (1) {
        clear();

        mvprintw (0, 0, "%s", system_info().c_str());
        mvprintw(5, 0, "%s", get_header().c_str());
        mvprintw(8, 0, "%s", set_rows(main_vec).c_str());
        c = getch();
        switch (c) {
        case 'P': s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 'V':s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 'p': s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 'M': s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 't': s.set_flag(c);sort(main_vec.begin(), main_vec.end(), s); break;
        case 'R': s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 'C': s.set_flag(c); sort(main_vec.begin(), main_vec.end(), s);break;
        case 'q': exit(0); break;
        default:
            break;
        }
	//update every 10 sec
	if(fg == 10)
         {
            main_vec = proc_vector();
            fg = 0;
         }

        fg++;
        refresh ();
        sleep (1);
    }

     endwin();

    return 0;
}

