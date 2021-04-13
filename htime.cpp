#include <htime.h>

string NAME="Dummy";
string FILENAME="dummy.txt";
unsigned int W_HOURS=20*60*60;

bool IS_DONE = false;
bool IS_STATUS = false;
bool WAS_OFFSET = false;
bool IS_FINISHED = true;

time_t TIME_LEFT = W_HOURS;


bool exist(string path){
    struct stat st;
    return stat(path.c_str(),&st)==0;
}

string leadingZero(string s){
    return s.length()==1?"0"+s:s;
}

time_t ceil(double n){
    long a=n;
    return a==n?a:a+1;
}

string getDate(struct tm *date){
    string ret="";
    ret+=to_string(date->tm_mday);
    ret+=DATE_DELIMETER;
    ret+=to_string(date->tm_mon+1);
    ret+=DATE_DELIMETER;
    ret+=to_string(date->tm_year+1900);
    ret+=YEAR_DELIMETER;
    return ret;
}

string getTime(struct tm *time, bool seconds){
    string ret="";
    ret+=leadingZero(to_string(time->tm_hour));
    ret+=TIME_DELIMETER;
    ret+=leadingZero(to_string(time->tm_min));
    if(seconds){
        ret+=TIME_DELIMETER;
        ret+=leadingZero(to_string(time->tm_sec));
    }
    return ret;
}

string getTime(time_t t){
    if(t<0)t=0;
    string ret="";
    ret+=leadingZero(to_string(t/3600));
    ret+=TIME_DELIMETER;
    ret+=leadingZero(to_string(t/60%60));
    return ret;
}

string getTimeAndDate(struct tm *tad){
    string ret="";
    ret+=getTime(tad,true);
    ret+=TIME_DATE_DELIMETER;
    ret+=getDate(tad);
    return ret;
}

time_t getThisMonday(struct tm *wk){
    struct tm forMon(*wk);
    forMon.tm_sec=0;
    forMon.tm_min=0;
    forMon.tm_hour=0;
    return forMon.tm_wday?mktime(&forMon)-(forMon.tm_wday-1)*86400:mktime(&forMon)-60*60*24*6;
}

time_t getLastMonday(struct tm *wk){
    return getThisMonday(wk)-60*60*24*7;
}

time_t getTimeFromLine(string line){
    return stoi(line.substr(0,line.find(DELIMETER)));
}

bool isFinish(string line){
    return line[line.find(DELIMETER)+1]=='F';
}

string getBasePath(){
    return getenv("HOME");
}

string getDirPath(){
    string ret="";
    ret+=getBasePath();
    ret+=FILE_DELIMETER;
    ret+=DIRECTORY;
    return ret;
}

string getFilePath(){
    string ret="";
    ret+=getBasePath();
    ret+=FILE_DELIMETER;
    ret+=DIRECTORY;
    ret+=FILE_DELIMETER;
    ret+=FILENAME;
    return ret;
}

vector<time_t> getTimesFromFile(time_t from){
    time_t t;
    bool first;
    string line;
    ifstream file;
    vector<time_t> times;

    first=true;
    file.open(getFilePath());
    if(file.peek()==ifstream::traits_type::eof())
        return times;
    while(getline(file,line)){
        t=getTimeFromLine(line);
        if(t>=from){
            IS_FINISHED=isFinish(line);
            if(first && IS_FINISHED)
                WAS_OFFSET=true;
            times.push_back(t);
            first=false;
        }
    }
    file.close();
    return times;
}

time_t calculate(vector<time_t> times, time_t from, time_t to){
    unsigned int i;
    time_t sumBeg=0,sumFin=0,total=0;
    if(WAS_OFFSET)
        total+=times.at(0)-from;
    for(i=WAS_OFFSET;i<times.size()-1+IS_FINISHED;i++){
        if((i+WAS_OFFSET)%2==0)
            sumBeg+=times.at(i);
        else
            sumFin+=times.at(i);
    }
    total+=sumFin-sumBeg;
    if(times.size()%2)
        total+=to-times.back();
    TIME_LEFT=W_HOURS-total;
    return total;
}

string toString(struct tm *td, time_t total){
    string ret="";
    ret+=to_string(mktime(td));
    ret+=DELIMETER;
    ret+=IS_FINISHED?"S":"F";
    ret+=DELIMETER;
    ret+=NAME;
    ret+=DELIMETER;
    ret+=getTime(total);
    ret+=DELIMETER;
    ret+=getTimeAndDate(td);
    ret+=IS_DONE?DELIMETER+"Finished":"";
    return ret;
}

void toFile(string str){
    ofstream file;
    file.open(getFilePath(),ios::app);
    file<<str<<endl;
    file.close();
}

string getTitle(){
    string ret="";
    ret+=NAME;
    ret+=STRING_DELIMETER;
    ret+=IS_DONE?IS_FINISHED?"Start"+STRING_DELIMETER+"FINISHED":"End"+STRING_DELIMETER+"FINISHED":IS_STATUS?IS_FINISHED?"Idle":"Running":IS_FINISHED?"Start":"End";
    return ret;
}

time_t timeLeft(time_t total){
    return W_HOURS-total+59;
}

string getBody(time_t total, time_t lastSession){
    if(IS_STATUS)
        return "Done: <b>"+getTime(total)+"</b>"+
            STRING_DELIMETER+
            "Last time: <b>"+getTime(lastSession)+"</b>"+
            STRING_DELIMETER+
            "Time left: <b>"+getTime(timeLeft(total))+"</b>";
    else if(IS_DONE)
        return "Done: <b>"+getTime(total)+"</b>"+
            STRING_DELIMETER+
            "Last time: <b>"+getTime(lastSession)+"</b>";
    else if(IS_FINISHED)
        return "Time left: <b>"+getTime(timeLeft(total))+"</b>";
    else
        return "Done: <b>"+getTime(total)+"</b>"+
            STRING_DELIMETER+
            "Last time: <b>"+getTime(lastSession)+"</b>";
}

string getString(time_t total, time_t lastSession){
    string ret="'";
    ret+=getTitle();
    ret+="' '";
    ret+=getBody(total, lastSession);
    ret+="'";
    return ret;
}

string baseNotify(){
    string str="";
    str+=COMMAND;
    str+=" ";
    str+=PARAMETERS;
    str+=" ";
    return str;
}

void notify(time_t total, time_t lastSession){
    string str="";
    str+=baseNotify();
    str+=getString(total, lastSession);
    system(str.c_str());
}

void notify(string title, string msg){
    string str="";
    str+=baseNotify();
    str+="'";
    str+=title;
    str+="' '";
    str+=msg;
    str+="'";
    system(str.c_str());
}

bool hasParameter(int argc, char* argv[], string param){
    for(int i=0;i<argc;i++)
        if(param.compare(argv[i])==0)
            return true;
    return false;
}
