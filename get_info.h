#ifndef GET_INFO_H
#define GET_INFO_H
#include <iostream>
#include <vector>
#include <proc/readproc.h>
#include <proc/sysinfo.h>
#include <cstring>
#include <pwd.h>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <fcntl.h>
#include <algorithm>
#include <ncurses.h>


using namespace std;



class proc_info
{
private:

       string time_active;		//time of activity process
       proc_t sing_proc;		//struct prot_t of procps library

public:
    proc_info();
    proc_info(proc_t _prc);

    void SET_PROC(proc_t &);
    char GET_STATE();
    long GET_PR();
    long GET_NI();
    string GET_PCOMMAND() const;
    string GET_UNAME();
    double GET_VIRT() const;
    double GET_RES() const;
    double GET_MEM() const;
    double GET_CPU() const;         //+
    string GET_PACTIV() const;      //+
    pid_t GET_PID() const;          //+


};

//convertion B, Kb, Mb
template<typename T>
inline T convert_unit( T num, int n)
{

  for( int i = 0; i < n; i++)
  {
    num /= 1024;
  }
  return num;
}


template<typename T>
T my_round(T pcpu, int N)
{
    static const double powerOfTen[] = { 1.0, 10.0, 100.0, 1000.0};
    pcpu = std::floor(pcpu * powerOfTen[N]) / powerOfTen[N];
    return pcpu;
}



//sort functions for more comfortable 
typedef proc_info p_inf;
 bool sort_pid(const p_inf& lhs, const p_inf& rhs);
 bool by_virt(const p_inf& lhs, const p_inf& rhs);
 bool by_act(const p_inf& lhs, const p_inf& rhs);
 bool by_cpu(const p_inf& lhs, const p_inf& rhs);
 bool by_mem(const p_inf& lhs, const p_inf& rhs);
 bool by_res(const p_inf& lhs, const p_inf& rhs);
 bool by_cmd(const p_inf& lhs, const p_inf& rhs);

//functor of sorting
class sort_fctr
{
    char fg;

public:
    sort_fctr() {}
    bool operator ()(const proc_info& lhs, const proc_info& rhs)
    {

        switch (fg) {
        case 'P':return sort_pid(lhs, rhs); break;
        case 't':return by_act(lhs, rhs); break;
        case 'p':return by_cpu(lhs, rhs); break;
        case 'M':return by_mem(lhs, rhs); break;
        case 'R':return by_res(lhs, rhs); break;
        case 'V':return by_virt(lhs, rhs); break;
        case 'C':return by_cmd(lhs, rhs); break;
        case 'q': exit(0); break;
        default: return sort_pid(lhs, rhs); break;
        }
    return sort_pid(lhs, rhs);
        
    }
 void set_flag(char ch)
    {
       fg = ch;
    }
};

void update_data(vector<proc_info>&, sort_fctr&);
vector<proc_info>proc_vector();
string set_rows(vector<proc_info>&mn_vec);
string get_header();
string system_info();
string  get_now();
#endif // GET_INFO_H

