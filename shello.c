// Name: Aditi Hande    Student ID: 1002177674  
// Lab Assignment1 CSE 3320-001-Professor Jude Agujiobi
/*  Some example code and prototype - 
    contains many, many problems: should check for return values 
    (especially system calls), handle errors, not use fixed paths,
    handle parameters, put comments, watch out for buffer overflows,
    security problems, use environment variables, etc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <strings.h>          //necessary for strcasecmp
#define MAX_ENTRIES 1024      //Max directory entries = 1024 
#define FILENAME_LENGTH 2048  // Max Length of Filename = 2048 or ;limited by limit.h MAX_LENGTH
#define ENTRIES 5             //entries per screen

typedef struct {
    char name[NAME_MAX + 1];  //Max name chars+ /0;
    int  is_dir;              // 1 = dir, 0 = file/other
    off_t  Size;              // file size
    time_t ModifiedTime;      // last write time
} DirEntry;

static DirEntry entries[MAX_ENTRIES];
static unsigned int entry_count = 0;

void ReadDir(){
    DIR * d;
    struct dirent * de;
    entry_count = 0;
    d = opendir( "." );                                              //"." = current directory 
    while ((de = readdir(d)) && (entry_count<= MAX_ENTRIES)) {       // If entry count> MAX_ENTRIES skip next entries; no buffer overflow
        entries[entry_count].is_dir = (de->d_type & DT_DIR );  //boolean and bitwise masking foe d type to check for dir //.is_dir element for dir element is set to 0 for file and non-zero for dir
        strcpy(entries[entry_count].name, de->d_name);          //Copying dir name to entries array

        // fill entry size and mtime for F/D
        struct stat st;
        if (stat(entries[entry_count].name, &st) == 0) {
            entries[entry_count].Size = st.st_size;
            entries[entry_count].ModifiedTime = st.st_mtime;
        } 
        else {
            entries[entry_count].Size = 0;
            entries[entry_count].ModifiedTime = 0;
        }
        
        entry_count++;                                          //increasing index of entries array
    }
    closedir( d );
}

//Move to Previous or Next Screen untill User hits Done.  
static void printScreenEntries(int start, int page_size){
    int end = start + page_size;                  //end value = (0 or other start value for 2nd,3rd,,, page) + (5)
    if (end > entry_count) end = entry_count;     //if end> entrycount, end now = entrycount, total entries in cd

    printf("----- Showing %d-%d of %d -----\n", start, (end ? end - 1 : 0), entry_count); //Display count: Showing 0-4 of 22(entries)//ternary operator in C(cond? if_true: if_false)Here(If end not 0? end-1=4: if end=0,display 0)

    for (int i = start; i < end; i++){
        char dtype = entries[i].is_dir ? 'D' : 'F';          //if dtype flag= nonzero/1,D , else F (for dtype flag=0)
        printf("%d   %c   %s\n", i, dtype, entries[i].name); //Display print: Index  F/D  entryName
    }
}

void showScreenEntries(void){  //Show 5 entries per Screen like 0,1,2,3,4 etc. 
    int start = 0;
    const int page = ENTRIES;  //const 5 items per page
    char line[32];

    printf( "-----------------------------------------\n" );
    printf("F= File Entries, D= Directory Entries.\n\n");            //Files are marked with F and Directories with D in Display menu

    if (entry_count <= 0){     //If no. of files/dir found inside current working Dir is 0 or less
        printf("(no entries)\n");
        return;
    }

    while (1) {
        printScreenEntries(start, page);                                     //Call fn with start value and fixed screen entries count: ENTRIES=5
        printf("Display Menu: \nEnter command (n=next, p=prev, d=done): ");  //n for next screen, p for previous screen, d to quit display menu

        if (!fgets(line, sizeof line, stdin)) break;             // Read one full line (up to newline). If read fails (EOF/error), exit the loop.
       

        int j = 0;                                               // Use only the first non-whitespace character from the line
        while (line[j] && isspace((unsigned char)line[j])) j++;  // Skip leading whitespace: j++ while current char exists and is space/tab/newline//While line[j]&&(line[j]==whitespace?)-- j++: read next char in line. If line[j]!=whitespace, && result becomes 0, don't j++ or don't go to next char in line-stop
        char ch = tolower((unsigned char)line[j]);               // ch = lowercase char of line[j]

        if (ch == 'd'){
            printf("Done.\n");
            break;                                            //Break,exit menu for Done option.
        }
        else if (ch == 'n'){
            if (start + page < entry_count) start += page;    //if start+ ENTRIESperscreen < total entries in dir(entry_count), start+=5, proceed to next page, if > entry_count can't move to next page 
            else printf("(at last page)\n\n\n");
        }
        else if (ch == 'p'){
            if (start >= page) start -= page;                 //if start>= ENTRIES per screen, decrement and go to previous screen
            else { start = 0; printf("(at first page)\n\n\n"); }  //else,if start<ENTRIES per screen can't go to previous screen
        }
        else{
            printf("Type n, p, or d, then Enter.\n");         //Error case- Maybe error in input scanning-repeat instructions
        }
    }
}
int toIndex(const char *s) {  //convert char to Int for index       //Convert user input(char) of Entry Index in char form to int, but if user input is char entry name and not index, leave it alone 
    if (!s || !*s) return -1;          //error case- s const char pointer to input passed

    int all_digits = 1, all_alpha = 1; // Set flag for all_digits and all_alpha as 1 initially,then check
    for (int i = 0; s[i]; i++) {       //one char at a time from input string
        if (!isdigit((unsigned char)s[i])) all_digits = 0;  //If even one char from string!=1, all_digits=0
    }

    if (all_digits) {
        return atoi(s);                 // convert number string to int
    } else 
        return -1;                      // alpha, mixed/invalid 
}

char *getFilename(const char *s, int isDir){   //Return entry index or entry filename for expected F or D
    int index = toIndex(s);
    if (index == -1)    return s;  //AH return closest match  //returns input string for entry name/index if not all_digits
    else  {
        //printf("%d  %s\n\n",index, s);
        if ( (isDir && entries[index].is_dir ) || (!isDir && !entries[index].is_dir ) )  //If expected dir, (isDir=1) && (D=1)  ,or expected file  !(isDir=0) && !(F=0), continue, else it is wrong entry in terms of enrty type F/D, not what is expected
             return entries[index].name;                                                 //If correct, return entry name
        else {
            printf("Not a valid file or directory!\n\n");                                    //else print error messaqge
            return NULL; 
        }
    }
}
//Sorting Entries based on name/size/time(mtime)
typedef enum { SortByName = 0, SortBySize = 1, SortByDate = 2 } SortMode;
static int currentSortMode = SortByName;

int CompareByName(const void *a, const void *b) {
    const DirEntry *x = a, *y = b;
    if (x->is_dir != y->is_dir) return y->is_dir - x->is_dir; // dirs first
    return strcasecmp(x->name, y->name);
}
int CompareBySize(const void *a, const void *b) {
    const DirEntry *x = a, *y = b;
    if (x->is_dir != y->is_dir) return y->is_dir - x->is_dir;
    if (x->Size < y->Size) return -1;
    if (x->Size > y->Size) return  1;
    return strcasecmp(x->name, y->name);
}
int CompareByDate(const void *a, const void *b) {
    const DirEntry *x = a, *y = b;
    if (x->is_dir != y->is_dir) return y->is_dir - x->is_dir;
    if (x->ModifiedTime < y->ModifiedTime) return -1; // older→newer (flip for newest first)
    if (x->ModifiedTime > y->ModifiedTime) return  1;
    return strcasecmp(x->name, y->name);
}
void SortEntries(void) {
    int (*cmp)(const void*, const void*) = CompareByName;
    if (currentSortMode == SortBySize) cmp = CompareBySize;
    else if (currentSortMode == SortByDate) cmp = CompareByDate;
    qsort(entries, entry_count, sizeof(DirEntry), cmp);
}
// Can give a directory as argument after ./shello <dir> to start in that directory
//AH use argc argv  // use to take arguments from terminal/shell    
int main(int argc, char **argv) {
    pid_t child;
    int i, c, k;
    char s[256], cmd[256];
    time_t t;
    char*   name;

    while (1) {

      t = time( NULL );
      printf( "It is now: %s\n", ctime( &t ));
      printf("-----------------------------------------------\n" );

      if (argc == 2)
       if (chdir(argv[1]) != 0) perror("chdir");  // Change to given directory if provided// if the arg is passed after ./filename to chage to that directory, it is agrv[1], if execution of cd to dir or argv[1] != succes or != 0(0 success in unix/linux -1 failure), print error message chdir
      getcwd(s, 200);                             //os command lib?//
      printf( "\nCurrent Directory: %s \n", s);

      ReadDir();
      //SortEntries();
      //showScreenEntries();


      printf("Action Menu: \nEnter command (q=quit, e=edit, r=run command, f=run file, c=change directory, d=display, s=sort): ");
      // Sometimes user input may appear to be skipped in the Action Menu because of how input is buffered.
      c = getchar();              // get user input for Action Menu = q/e/r/c
      while (getchar() != '\n');  // clear the rest of the line
    
      switch (c) {
          case 'q': 
            printf("Quitting...\n");
            exit(0);
          case 'd':
            showScreenEntries();
            break;
          case 'e':
            printf("Edit what file?: ");
            scanf("%s", s);
            name = getFilename(s,0);   //return index of entry or entry name as per expected file type, here F
            if(name != NULL){
                strcpy(cmd, "nano ");
                strcat(cmd, name);
                system(cmd);
            }
            break;
          case 'r':                                    // runs command with args like man ls
            printf("Run- Enter command with arguments: ");
            if (fgets(cmd, sizeof(cmd), stdin)) {      // remove trailing newline if present
                size_t len = strlen(cmd);
                if (len > 0 && cmd[len - 1] == '\n') {
                    cmd[len - 1] = '\0';
                }
                if (*cmd) {                           // only run if not empty
                    system(cmd);
                }
            }
            break;
            case 'f':                         //running file without arguments
            printf("Run which file?: ");
            scanf("%s", s);
            name = getFilename(s,0);         //function above in this code not inbuilt
            strcpy(cmd, "./");
            strcat(cmd, name);   //return index of entry or entry name as per expected file type, here F
            system(cmd);
            break;
            case 'c':
            printf("Change to what directory?: ");
            scanf("%s", cmd);
            name = getFilename(cmd, 1);  //return index of entry or entry name as per expected entry type, here D
            //printf("%s\n", name);
            if (chdir(name) != 0) perror("chdir");  // Try to change current directory to 'name'; if it fails, print system error message labeled "chdir"
            break;
          case 's': 
            char buf[16];
            printf("Sort by what property? (0=Name, 1=Size, 2=Date): ");
            if (fgets(buf, sizeof buf, stdin)) {
                if (buf[0]=='1') currentSortMode = SortBySize;
                else if (buf[0]=='2') currentSortMode = SortByDate;
                else currentSortMode = SortByName;
            }
            SortEntries();
            showScreenEntries();
            break;
          default:                       //default error case
            printf("Unknown command.\n");
        }

       
    } // go through ReadDir, getFilename...fncs.. int main()// go through shelly.c
}
