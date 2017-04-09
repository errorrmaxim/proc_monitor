#include "get_info.h"



proc_info::proc_info() { }
proc_info::proc_info(proc_t _prc): sing_proc(_prc) { }


void proc_info::SET_PROC(proc_t &_p)
{
    sing_proc = _p;
}

pid_t proc_info::GET_PID() const
{
    return sing_proc.tid;
}

char proc_info::GET_STATE()
{
    return sing_proc.state;
}

long proc_info::GET_PR()
{
    return sing_proc.priority;
}

long proc_info::GET_NI()
{
    return sing_proc.nice;
}

string proc_info::GET_UNAME()
{
        passwd *pw = getpwuid(static_cast<uid_t>(sing_proc.euid));
        if(strlen(pw->pw_name) > 7)
        {
            return string(pw->pw_name).substr(0, 6) + '+';
        }else
        {
            return pw->pw_name;
        }

}

double proc_info::GET_VIRT() const
{
    double tmp = convert_unit(static_cast<double>(sing_proc.vsize), 2);
    return my_round(tmp, 1);
}

double proc_info::GET_RES() const
{
    return my_round(convert_unit(static_cast<double>(sing_proc.vm_rss),1), 1);
}

string proc_info::GET_PACTIV() const
{
    stringstream ss;
    double tot= (sing_proc.utime + sing_proc.stime)/ static_cast<double>(Hertz);
    int H = tot / 60;
    double min_sec = fmod(tot, 60);

     ss << H << ':' << setw(5) << setfill('0') << fixed << setprecision(2) << min_sec;

     return ss.str();
}
double proc_info::GET_CPU() const
{
    time_t total = sing_proc.utime + sing_proc.stime;
    time_t seconds_since_boot = uptime(NULL,NULL);
    time_t seconds = seconds_since_boot - sing_proc.start_time / Hertz;
    double pcpu = static_cast<double>((total * 1000ULL / Hertz) / (seconds*10.0));
    return my_round(pcpu, 1);

}

double proc_info::GET_MEM() const
{
    meminfo();

    unsigned tot_ram = kb_main_total;
    return my_round(static_cast<double>((sing_proc.vm_rss * 100.0) / tot_ram), 1);
}


string proc_info::GET_PCOMMAND() const
{

string tmp;


    char buff[PATH_MAX];
    string path = "/proc/" + to_string(sing_proc.tid    ) + "/cmdline";

    int fd = open(path.c_str(), O_RDONLY);
    read(fd, buff, 128);
     int str_sz = strlen(buff);
     if(str_sz > 20)
     {
      tmp = string(buff).substr(str_sz-20);
      return tmp;
     }
     else {
          return buff;
          }

}


//read all process of system
vector<proc_info>proc_vector()
{
   vector<proc_info>ret_set;
     PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
     proc_t proces_info;
     memset(&proces_info, 0, sizeof(proces_info));
     while (readproc(proc, &proces_info) != NULL ) {
            ret_set.push_back(proces_info);
     }

     return ret_set;
}

//set line of process info
string set_rows(vector<proc_info>&mn_vec)
{
    stringstream ss;
    for(auto it = mn_vec.begin(); it != mn_vec.end();++it)
    {

        ss << setw(5) << setfill(' ') << it->GET_PID();
        ss << setw(9) << setfill(' ') <<  it->GET_UNAME();
        ss << setw(5) << setfill(' ') << it->GET_PR() << setw(4) << setfill(' ') << it->GET_NI()\
           << setw(8) << setfill(' ') << it->GET_VIRT() << setw(7) << setfill(' ') << it->GET_RES()\
           << setw(6) << setfill(' ') << it->GET_CPU() << setw(5) << setfill(' ') << it->GET_MEM()\
           << setw(10) << setfill(' ') << it->GET_PACTIV() << setw(3) << setfill(' ') << it->GET_STATE() << ' '\
           << setw(16) << setfill(' ') << it->GET_PCOMMAND() << endl;
    }
    return ss.str();

}

//set header
string get_header()
{
    stringstream ss;

    ss << string(80, '-') << endl << setw(5) << setfill(' ') << "PID" << setw(9) << setfill(' ') << "USER  "\
       << setw(5) << setfill(' ') << "PR" << setw(4) << setfill(' ') << "NI"\
       << setw(8) << setfill(' ') << "VIRT" << setw(7) << setfill(' ') << "RES"\
       << setw(6) << setfill(' ') << "%CPU" << setw(5) << setfill(' ') << "%MEM"\
       << setw(10) << setfill(' ') << "TIME+" << setw(3) << setfill(' ') << "S"\
       << setw(16) << setfill(' ') << "COMMAND" << endl << string(80, '-') << endl;

    return ss.str();
}

//time
string  get_now()
{
    stringstream ss;
    time_t seconds = time(NULL);
    tm* now = localtime(&seconds);
    ss << setw(2) << setfill('0') << now->tm_hour << ':'\
       << setw(2) << setfill('0') << now->tm_min << ':'\
       << setw(2) << setfill('0') << now->tm_sec;
    return ss.str();

}

//set sys info
string system_info()
{
    meminfo();
    double tatal_mem = my_round((kb_main_total/1024/1024.0), 1);
    double free_mem = my_round((kb_main_free/1024/1024.0), 1);
    unsigned tasks = proc_vector().size();
    stringstream ss;
    time_t seconds_since_boot = uptime(NULL,NULL);
    int d = seconds_since_boot / (24 * 3600);
    int h = (seconds_since_boot % (3600 *24)) / 3600;
    int m = (seconds_since_boot / 60) % 60;
    ss << "my_top - " << get_now()  << " up " << d << " day, "<< setw(2) << setfill('0') << h << ":"\
       << setw(2) << setfill('0') << m << endl << "Tasks: " << tasks << " total." << endl << "CPU: " << smp_num_cpus\
       << endl << "GiB MeM  : " << tatal_mem << endl << "Free Mem : " << free_mem << endl;
    return ss.str();
}


bool sort_pid(const p_inf& lhs, const p_inf& rhs) {return lhs.GET_PID() < rhs.GET_PID();}
bool by_virt(const p_inf& lhs, const p_inf& rhs) {return lhs.GET_VIRT() > rhs.GET_VIRT();}
bool by_act(const p_inf& lhs, const p_inf& rhs) { int answ = std::strcmp(lhs.GET_PACTIV().c_str(), rhs.GET_PACTIV().c_str());
    if(answ > 0) { return true; }
                   return false;}
bool by_cpu(const p_inf& lhs, const p_inf& rhs) { return lhs.GET_CPU() > rhs.GET_CPU();}
bool by_mem(const p_inf& lhs, const p_inf& rhs) { return lhs.GET_MEM() > rhs.GET_MEM();}
bool by_res(const p_inf& lhs, const p_inf& rhs) { return lhs.GET_RES() > rhs.GET_RES();}
bool by_cmd(const p_inf& lhs, const p_inf& rhs) { int answ = std::strcmp(lhs.GET_PCOMMAND().c_str(), rhs.GET_PCOMMAND().c_str());
    if(answ > 0) {return true; }
                  return false;}

