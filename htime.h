#ifndef HTIME_H
#define HTIME_H

#include <ctime>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

const unsigned int ARG_NUM = 5;
const string DIRECTORY = "Mega/Job/Hours"; // in home

const string DELIMETER=" ";
const string TIME_DELIMETER=":";
const string FILE_DELIMETER="/";
const string YEAR_DELIMETER=".";
const string DATE_DELIMETER=". ";
const string TIME_DATE_DELIMETER=" · ";

const string TITLE="Weekter";
const string STRING_DELIMETER=" · ";
const string COMMAND="notify-send";
const string PARAMETERS="--icon=clock";

const string TOGGLE_PARAMETER="t";
const string STATUS_PARAMETER="s";


extern string NAME;
extern string FILENAME;

extern bool IS_DONE;
extern bool IS_STATUS;
extern bool WAS_OFFSET;
extern bool IS_FINISHED;

extern time_t TIME_LEFT;
extern unsigned int W_HOURS;

string leadingZero(string s);
string getDate(struct tm *date);
string getTime(struct tm *time, bool seconds);
string getTime(time_t t);
string getTimeAndDate(struct tm *tad);
string toString(struct tm *td, time_t total);
string getTitle();
string getBody(time_t total, time_t lastSession);
string getString(time_t total, time_t lastSession);
string getBasePath();
string getDirPath();
string getFilePath();
string baseNotify();

time_t ceil(double n);
time_t timeLeft(time_t total);
time_t getThisMonday(struct tm *wk);
time_t getLastMonday(struct tm *wk);
time_t getTimeFromLine(string line);
time_t calculate(vector<time_t> times, time_t from, time_t to);

vector<time_t> getTimesFromFile(time_t from);

bool isFinish(string line);
bool hasParameter(int argc, char* argv[], string param);
bool exist(string path);

void toFile(string str);
void notify(time_t total, time_t lastSession);
void notify(string title, string msg);

#endif // HTIME_H
