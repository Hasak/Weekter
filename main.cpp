#include <htime.h>

// Arguments:
// [0] Command (Weekter, or so)
// [1] Arbiraty name (shown in notify)
// [2] Text file for storing
// [3] Weekly hours
// [4] TOGGLE_PARAMETER|STATUS_PARAMETER toggle or show status

int main(int argc, char* argv[])
{
    time_t now;
    time_t thisMon;
    time_t total, lastSession=0;

    struct tm *td;
    vector<time_t> times;

    if(argc!=ARG_NUM){
        notify("ERROR","Invalid number of arguments");
        return 0;
    }

    if(STATUS_PARAMETER.compare(argv[4]) && TOGGLE_PARAMETER.compare(argv[4])){
        notify("ERROR","Third argument can be <b>"+TOGGLE_PARAMETER+"</b> or <b>"+STATUS_PARAMETER+"</b>");
        return 0;
    }

    NAME=argv[1];
    FILENAME=argv[2];
    W_HOURS=stod(argv[3])*60*60;

    if(!exist(getDirPath())){
        string cmd;
        cmd="mkdir "+getBasePath()+FILE_DELIMETER+DIRECTORY;
        system(cmd.c_str());
    }

    now=time(nullptr);
    td=localtime(&now);
    thisMon=getThisMonday(td);
    times=getTimesFromFile(thisMon);
    total=calculate(times,thisMon,now);
    times.push_back(now);
    lastSession=times.size()>1?times.at(times.size()-1-IS_FINISHED)-times.at(times.size()-2-IS_FINISHED):0;

    IS_DONE=total>=W_HOURS;
    IS_STATUS=STATUS_PARAMETER.compare(argv[4])==0;
    if(TOGGLE_PARAMETER.compare(argv[4])==0)
        toFile(toString(td,total));

    notify(total, lastSession);

    return 0;
}
