#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "Analyse.h"
#include "Process.h"

#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

using namespace std;

/* Mảng các câu lệnh command được hỗ trợ */
const char *command[] = {
    "cd",     
    "cls",    
    "echo",  
    "exit", 
    "help", 
    "del",
    "dir",
    "mkdir", 
    "date",
    "time",
    "pc",
    "calc",
    "run"
};

/* Mảng các lệnh command*/
int (*activate_command[])(char **) = { 
  &cd,
  &cls,
  &echo,
  &exit,
  &help,
  &del,
  &dir,
  &mk_dir,
  &date,
  &time_cmd,
  &pc,
  &calc,
  &runbat
}; 

/**
 * Chạy lệnh với lệnh là arg[0] như cd, dir, exit, help
 **/
int execute(char ** args){
    /**
     * Chạy lệnh với lệnh là arg[0] như cd, dir, exit, help
     **/
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ /* Kiểm tra xem người dùng nhập lệnh nào trong tập lệnh */
            return (*activate_command[i])(args);
        }
    }
    return 0;
}


/**
 * Trả về số câu lệnh trong tập các lệnh 
 **/
int size_of_command(){
    /**
     * Trả về số câu lệnh trong tập các lệnh 
     **/
    return sizeof(command) / sizeof(char *);
}


//////////////////////////////////////////
////////// Danh sách câu lệnh //////////// 
//////////////////////////////////////////

int help(char **args){
    if (args[1] == NULL)
	{
		printf("Type \"help -[command]\" for more information about a specific command.\n");
		printf("Suppoted commands:\n cd\n date\n time\n dir\n cls\n echo\n del\n mkdir\n pc\n run\n exit\n ");
		printf("Usage:\n\t <command> [option]\n\tEXAMPLE: help cd\n");
		printf("%-30s%s\n%-30s%s", " cd",
			"Change the current directory. You must write the new directory after this command.",
			" ", "EXAMPLES: \"cd C:/\"\n\n");
		printf("%-30s%s\n%-30s%s", " date",
			"Show the today's date.",
			" ", "EXAMPLES: \"date\"\n\n");
		printf("%-30s%s\n%-30s%s", " time",
			"Show the current time.",
			" ", "EXAMPLES: \"time\"\n\n");
		printf("%-30s%s\n%-30s%s", " dir",
			"Show all files and folders in the current directory.",
			" ", "EXAMPLES: \"dir\"\n\n");
        printf("%-30s%s\n%-30s%s", " cls",
			"Clear the console screen.",
            " ", "EXAMPLES: \"cls\"\n\n");
        printf("%-30s%s\n%-30s%s", " echo",
			"Print a message on the screen.",
            " ", "EXAMPLES: \"echo [message]\"\n\n");
        printf("%-30s%s\n%-30s%s", " del",
			"Delete a file or folder in the current directory.",
            " ", "EXAMPLES: \"del [Folder/Filename]\"\n\n");
        printf("%-30s%s\n%-30s%s", " mkdir",
			"Make a new directory.",
            " ", "EXAMPLES: \"mkdir[Foldername]\"\n\n");
        printf("%-30s%s\n%-30s%s", " run",
			"Run .bat file only.",
            " ", "EXAMPLES: \"run [filename.bat]\"\n\n");
        printf("%-30s%s\n%-30s%s\n%-30s%s", " pc",
            "Create process.", " ",
            "You must enter the options in the 2nd argument, such as f and b",
            " ", "EXAMPLES: \"pc bg\"\n\n");
		printf("%-30s%s", " exit", "Exit this tiny shell :((((\n");
	}
    else if(!strcmp(args[1],"cd")){
        cout << "Change the current directory." << endl;
        cout << "       cd           : Show the current directory" << endl;
        cout << "       cd ..        : Show the parent directory of the current directory" << endl;
        cout << "       cd [path]    : Change the current directory to [path]" << endl;
        cout << "EXAMPLE: cd C:\\Users\\Admin\\ => change current directory to C:\\User\\Admin" << endl;
    }
    else if(!strcmp(args[1],"date")){
        cout << "Display the current date to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"time")){
        cout << "Diplay the current time to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"dir")){
        cout << "Display the list of files and folder in the directory to the screen." << endl;
        cout << "       dir         : Show the files and folders in the current directory" << endl;
        cout << "       dir [path]  : Show the files and folders in the [path]" << endl;
        cout << "EXAMPLE: \"dir\", \"dir C:\\Users\\Admin\"" << endl;
    }
    else if(!strcmp(args[1],"cls")){
        cout << "Clear all line displaying on the console screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"echo")){
        cout << "Print the message on the screen." << endl;
        cout << "This command does not support any options." << endl;
        cout << "EXAMPLE: \"echo Hello World\"" << endl;
        cout << "       => \"Hello World\"" << endl;
    }
    else if(!strcmp(args[1],"del")){
        cout << "Delete file or folder in the current directory." << endl;
        cout << "Recommend: Folder/File name contains no space." << endl;
        cout << "       del [Folder/Filename] : Delete folder/file (Folder/Filename) in the current dirrectory" << endl;
        cout << "EXAMPLE: \"del ABC\"" << endl; 
    }
    else if(!strcmp(args[1],"mkdir")){
        cout << "Make a new directory in the current directory." << endl;
        cout << "Recommend: Foldername contains no space." << endl;
        cout << "       mkdir [Foldername] : Make folder [Foldername] in the current directory" << endl;
        cout << "EXAMPLE: \"mkdir ABC\"" << endl;
    }
    else if (!strcmp(args[1], "run")){
        cout << "Run .bat file only with commands which our shell supports" << endl;
        cout << "       run [Filename.bat] : Run .bat file with commands which our shell supports " << endl;
        cout << "EXAMPLE: \"run command.bat\"" << endl;
    }
    else if(!strcmp(args[1],"pc")){
        cout << "Supported options:" << endl;
        cout << "        all     Show list of all running processes" << endl;
		cout << "        find    Get pid of specific program(s) by name" << endl;
		cout << "        suspend Suspend a program by process id" << endl;
		cout << "        resume  Resume a program by process id" << endl;
		cout << "        kill    Terminate all program by name" << endl;
        cout << "        killid  Terminate a program by process id" << endl;
        cout << "        bg      Run a program in background mode by path to program" << endl;
		cout << "        fg      Run a program in foregound mode by path to program" << endl;
        cout << "EXAMPLE: \"pc fg C:\\Users\\Admin\\ABC\"" << endl;
    }
    else if(!strcmp(args[1],"exit")){
        cout << "Exit the TinyShell." << endl;
        cout << "This command does not support any options." << endl; 
    }
    return 0;
}


/**
 * Chuyển directory hiện tại sang directory mới
 * Câu lệnh: cd [path] 
 * cd : trả về đường dẫn directory hiện tại 
 * cd .. :trả về đường dẫn directory cha
 * cd [path]: chuyển current working directory sang directory mới
 * 
 **/
int cd(char **args){
    /**
     * Chuyển directory hiện tại sang directory mới
     * Câu lệnh: cd [path] 
     * cd : trả về đường dẫn directory hiện tại 
     * cd .. :trả về đường dẫn directory cha
     * cd [path]: chuyển current working directory sang directory mới
     * 
     **/
    if(args[1] == NULL){ /* Nếu chỉ gõ lệnh cd */
        system("cd"); /* Dùng luôn lệnh cd có sẵn của Windows (Chơi bẩn nhưng kệ) */
        cout << endl;
        return EXIT_SUCCESS;
    }
    /* Nếu cd [path] */
    else {
        char* path = combinePath(args,1); /* Chuẩn hóa path */
        if(SetCurrentDirectory(path)==FALSE){ /* Tìm đường dẫn nếu có */
            // cout << args[1] << endl;
            fprintf(stdout,"Not able to set current working directory\n");
        }
    }

    return 0;
}

/**
 * Liệt kê các folder, file trong directory 
 * Câu lệnh: dir [path] 
 * 
 **/
int dir(char **args){
    /**
     * Liệt kê các folder, file trong directory 
     * Câu lệnh: dir [path] 
     * 
     **/
    
    /* Đống này chôm chỉa về sửa một chút t cx chưa hiểu hết :)))))) */
    char *cur_dir = (char*)malloc(MAX_CWD_LENS*sizeof(char));
    GetCurrentDirectory(MAX_CWD_LENS, cur_dir);
	WIN32_FIND_DATA data;
	_FILETIME time_lastwrite;
	_SYSTEMTIME convert_time;
	string date;
	string time;
	char *char_date = (char *)calloc(15, sizeof(char));
	char *char_time = (char *)calloc(15, sizeof(char));
	const char add[] = "\\*";
    char *path = (char*)malloc(MAX_CWD_LENS*sizeof(char));
    if(args[1] == NULL){
        strcpy(path, cur_dir);
        strcat(path,"\\*");
    }
    else {
        path = combinePath(args,1);
        strcat(path,"\\*");
    }
	HANDLE han = FindFirstFile(path, &data);
	printf("%-15s%-15s%-15s%-15s\n", "Date", "Time", "Type", "Name");
	printf("=========================================================================\n");
	if (han != INVALID_HANDLE_VALUE)
	{
		do
		{
			time_lastwrite = data.ftLastWriteTime;
			FileTimeToSystemTime(&time_lastwrite, &convert_time);

			// Convert date, time to string & char *
			date = to_string(convert_time.wDay) + '/' + to_string(convert_time.wMonth) + '/' + to_string(convert_time.wYear);
			time = to_string(convert_time.wHour) + ':' + to_string(convert_time.wMinute);
			strcpy(char_date, date.c_str());
			strcpy(char_time, time.c_str());

			if (data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "<FILE>", data.cFileName);
			}
			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "<FOLDER>", data.cFileName);
			}
		} while (FindNextFileA(han, &data) != 0);
        delete cur_dir;
		FindClose(han);
		delete char_time;
		delete char_date;
		return EXIT_SUCCESS;
	}
	else
	{
        delete cur_dir;
        delete char_time;
		delete char_date;
		return EXIT_FAILURE;
	}
}


/**
 * Tạo folder trong directory hiện tại
 * Câu lệnh: mkdir [foldername]
 * 
 **/
int mk_dir(char **args){
    /**
     * Tạo folder trong directory hiện tại
     * Câu lệnh: mkdir [foldername]
     * 
     **/
    if(args[1] == NULL){ /* Chỉ gõ lệnh mkdir */
        cout << "ERROR: Command mk_dir need filename" << endl;
        cout << "Command: mkdir [filename]" << endl;
        cout << "Recommend: filename should not have any space" << endl;
        return 0;
    }
    mkdir(args[1]); /* Lệnh mkdir tạo folder có sẵn */
    return 0;
}


/**
 * In ra message đi kèm với echo
 * Câu lệnh: echo [message]
 *  
 **/
int echo(char **args){
    /**
     * In ra message đi kèm với echo
     * Câu lệnh: echo [message]
     *  
     **/
    if(args[1] == NULL){
        cout << "ERROR: Echo + [message]" << endl;
        return 0;
    }
    int i=0;
    while(args[++i] != NULL){
        for(int j=0; j<strlen(args[i]); j++){
            cout << args[i][j];
        }
        cout << " ";
    }
    cout << endl;
    return 0;
}

/**
 * Clear toàn màn hình console
 * Câu lệnh: cls
 **/
int cls(char **args){ 
    /**
     * Clear toàn màn hình console
     * Câu lệnh: cls
     * 
     * Cũng chôm chỉa về nốt :)))
     **/
    
    if(strcmp(args[0],"cls") == 0){
        HANDLE hConsole; 
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        SMALL_RECT scrollRect;
        COORD scrollTarget;
        CHAR_INFO fill;

        // Get the number of character cells in the current buffer.
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            return 0;
        }

        // Scroll the rectangle of the entire buffer.
        scrollRect.Left = 0;
        scrollRect.Top = 0;
        scrollRect.Right = csbi.dwSize.X;
        scrollRect.Bottom = csbi.dwSize.Y;

        // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
        scrollTarget.X = 0;
        scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

        // Fill with empty spaces with the buffer's default text attribute.
        fill.Char.UnicodeChar = TEXT(' ');
        fill.Attributes = csbi.wAttributes;

        // Do the scroll
        ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

        // Move the cursor to the top left corner too.
        csbi.dwCursorPosition.X = 0;
        csbi.dwCursorPosition.Y = 0;

        SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
    }
    return 0;
}


/**
 * In ra màn hình ngày hiện tại
 * Câu lệnh: date
 * 
 **/
int date(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"date\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current Date: " << now->tm_mday << '/'
		<< (now->tm_mon + 1) << '/'
		<< (now->tm_year + 1900)
		<< endl;
	return 0;
}


/**
 * In ra màn hình thời gian (giờ:phút:giây) hiện tại
 * Câu lệnh: time
 * 
 */
int time_cmd(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"time\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current time: " << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;
	return 0;
}

/**
 * Delete file hoặc folder
 * Câu lệnh: del [file/foldername]
 *  
 **/
int del(char **args){
    /**
     * Delete file hoặc folder
     * Câu lệnh: del [file/foldername]
     *  
     **/
    if(args[1] == NULL){
        cout << "ERROR: Command 'del' need path" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    else if (args[2] != NULL){
        cout << "ERROR: Command 'del' cannot delete more than 1 file" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    if(remove(args[1]) == -1 && rmdir(args[1]) == -1){ /* Nếu arg[1] là file thì xóa file, folder thì xóa folder. Nếu k có thì in chỗ dưới */
        cout << "ERROR: Unable to find file to be deleted. Try again" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    return 0;
}


/**
 * Thoát chương trình
 * Câu lệnh: exit
 * 
 **/
int exit(char **args){
    /**
     * Thoát chương trình
     * Câu lệnh: exit
     * 
     **/
    char* term = (char*)malloc(64*sizeof(char));
    if(args[1] != NULL){
        term = combinePath(args,1);
        cout << "ERROR: Term " << term << "is not recognized for EXIT" << endl;
        cout << "Command: exit" << endl;
        return 0;
    }
    /* Trả 1 về cho biết stop trong main */
    return 1;
}


/**
 * Các lệnh với tiến trình
 * Câu lệnh: pc (Process)
 * 
 **/
int pc(char **args) {
    if (args[1] == NULL) {
        cout << "ERROR: Too few argument" << endl;
        return 0;
    }
    if (strcmp(args[1], "all") == 0) {
        if (getProcessListAll()) {
            return 0;
        }
        return 1;
    }
    if (strcmp(args[1], "find") == 0){
		if (args[2] == NULL)
		{
			printf("ERROR: Too few arguments\n");
			return 0;
		}
		// Tìm ID Process
		if (findProcessID(args[2]))
			return 0;
        return 1;
	}

    if (strcmp(args[1], "kill") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }

        if (killProcess(args[2])) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "killid") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        if (killProcessID(process_id)) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "suspend") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        suspendProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "resume") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        resumeProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "bg") == 0 || strcmp(args[1], "fg") == 0) {
        if(args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        } else {
            createNewProcess(args);
        }
        return 0;
    } 
    else {
        cout << "ERROR: Too few argument" << endl;
        return 0;
    }
}

/**
 * Bật calculator của Windows 
 * Câu lệnh: calc
 * 
 **/
int calc(char **args){
    system("calc");
    return 0;
}


//////////////////////////////////////////
//////// Dành riêng cho file .bat ////////
//////////////////////////////////////////

/**
 * Kiểm tra xem câu lệnh có được hỗ trợ trong shell không
 * 
 **/
bool cmdCheck(char **args){
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ /* Kiểm tra xem người dùng nhập lệnh nào trong tập lệnh */
            return true;
        } 
    }
    return false;
}

/**
 * Chạy riêng cho file .bat  
 * Câu lệnh: run [Filename.bat]
 * 
 **/

int runbat(char **args){
    char w[255],a[255];
    char *run_file = combinePath(args, 1);
    FILE *f=fopen(run_file,"rt");
    if(f==NULL)
	{
        printf("\nLoi doc file.\n");
        // getch();
        return 0;
	}
    else
    {
        while(!feof(f))
        {
            fgets(w,255,f);
            strcpy(a,w);
            char **arg = separate_line(a); 
            printf("");
            // cout << w << endl; 
            if(cmdCheck(arg)){
                int stop = execute(arg); // system ???
            }
            else {
                system(w);
                
            }
        }
        fclose(f);
    }
    return 0;
}