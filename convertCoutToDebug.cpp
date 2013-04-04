#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


const int MaxFileName = 200;
const int MAXDEBUGCHANNELS = 24;


const string DEBUG_CHANNELS[MAXDEBUGCHANNELS] = {"OWN_POS_LATENCY","UNIT_SA_DEBUG","VERSION_VIEWER_DEBUG","QUICKSENDSS_DEBUG","SYS_TIME_DEBUG","LOGMGR_DEBUG","MESSAGE_MGR_DEBUG",
"PFF_DEBUG","DATASTOREMGR_DEBUG","CPLOG_TOOL_DEBUG","MAPSET_DEBUG","SCM_DEBUG","RDM_DEBUG","SLV_DEBUG","LV_DEBUG","MAPSET_MDL_PLUGIN_DEBUG","LOG_MDL_PLUGIN_DEBUG",
"CLOS_MODULE_DEBUG","NAV_MODULE_DEBUG","MM_COMMON_DEBUG","MM_MAP_COMPONENT_DEBUG","FILTERS_MODULE_DEBUG","HOOK_MODULE_DEBUG","OVERLAYS_MODULE_DEBUG"};


vector<string> data;
string debug_name;
string debug_level;
char file_name[MaxFileName + 1];
char notify_every;
bool notify = false;
int num_converted = 0;

const string function = "__FUNCTION__";


string convertCout(string x)
{
    int leading_spaces = 0;
    leading_spaces = x.find('c');

    string spaces = "";
    for(int i = 0;i<leading_spaces;i++)
        spaces = spaces + " ";
 
    int begin = x.find("\"");

    string temp = x.substr(begin+1);
    int end = 0;
    end = temp.find("\"");

    string comment = temp.substr(0, end);

    string debug_line = spaces + "_debug->printdv(" + debug_name + ",DBG_DEBUG" + debug_level + ",\"%s " + comment + "\"," + function + ");";
    return debug_line;



}


void isCout(string x)
{
    int found;
    char replace;
    bool converted;
    string debug_line = "";
    found = x.find("cout");
    if (found!=string::npos)
    {
        debug_line = convertCout(x);
        converted = true;
        //data.push_back(debug_line);
    }
    if(notify)
    {
        if(converted)
        {
            cout<<endl;
            cout<<"---------------------------------------------------------------------------------------"<<endl;
            cout<<"Found : "<<x<<endl;
            cout<<"Replace with : "<<debug_line<<endl<<" (y/n?)";
            cin>>replace;
            cout<<endl;
            if(replace == 'y' || replace == 'Y')
            {
                data.push_back(debug_line);
                num_converted++;
            }
            if(replace == 'n' || replace == 'N')
                data.push_back(x);
            if(replace != 'n' && replace != 'N' && replace != 'Y' && replace != 'y')
                data.push_back(x);
            cout<<"---------------------------------------------------------------------------------------"<<endl;
        }
        else
            data.push_back(x);
    }
    else
    {
        if(converted)
            {
                num_converted++;
                data.push_back(debug_line);
            }
        else
            data.push_back(x);
    }
}




void Print()
{
    ofstream outputFile;
    outputFile.open(file_name);
    for(int i = 0; i<data.size();i++)
        outputFile<<data[i]<<endl;
    outputFile.close();


}

void PrintDebugChannels()
{
    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------"<<endl;
    cout<<"                                  POPULAR DEBUG CHANNELS                               "<<endl;
    for(int i = 0; i < MAXDEBUGCHANNELS; i++)
    {
        cout<<"--> "<<DEBUG_CHANNELS[i]<<endl;
    }

    cout<<"----------------------------------------------------------------------------------------"<<endl<<endl;

    cout<<endl<<"Please enter the debug channel name : ";
    debug_name = "";
    cin>>debug_name;
}
int main()
{
    cout<<endl<<"----------DISCLAIMER: Will only work on cout statements that do not print variables---------------------"<<endl;

    cout<<endl<<"Please enter the debug channel name (i.e. FILTERS_MODULE_DEBUG)"<<endl<<"You can also enter c to view a list of a few popular channels"<<endl;
    cin>>debug_name;
    if(debug_name == "c")
        PrintDebugChannels();
    cout<<endl<<"Please enter the debug level (i.e. 1, 2, or 3) : ";
    cin>>debug_level;
    cout<<endl<<"Please enter the filename you wish to apply these changes to : ";
    cin>>file_name;
    cout<<endl<<"Would you like to be notified about each statement? (y/n) ";
    cin>>notify_every;
    cout<<endl;

        if(notify_every == 'y' || notify_every == 'Y')
            notify = true;

     string LINE;
     ifstream myReadFile;
     myReadFile.open(file_name);
     if (myReadFile.is_open()) 
     {
        while (!myReadFile.eof()) 
        {
            getline(myReadFile,LINE);
            isCout(LINE);
        }
     }
     else
         cout<<"File does not exist or path is wrong"<<endl;
    myReadFile.close();
    Print();
    cout<<endl<<"----> A total of "<<num_converted<<" statements were converted this session!"<<endl;
    cout<<"----> See you next time!"<<endl<<endl;
}

