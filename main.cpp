#include <htime.h>

int main(int argc, char* argv[])
{
    time_t now;
    time_t thisMon;
    time_t total, lastSession=0;

    struct tm *td;
    string fileString;
    vector<time_t> times;

    now=time(nullptr);
    td=localtime(&now);
    thisMon=getThisMonday(td);
    times=getTimesFromFile(thisMon);
    total=calculate(times,thisMon,now);
    //lastSession=times.at(times.size()-1)-times.at(times.size()-2);

    IS_DONE=total>=W_HOURS;
    IS_STATUS=hasParameter(argc,argv,STATUS_PARAMETER);
    if(hasParameter(argc,argv,TOGGLE_PARAMETER)){
        fileString=toString(td,total);
        toFile(fileString);
    }

    notify(total, lastSession);

    return 0;
}
