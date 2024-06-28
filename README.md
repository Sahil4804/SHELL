# Description
- To run the shell write the following commands should be entered in order
1. make
2. ./a.out (if there are permissions issues then use sudo ./a.out instead of ./a.out).

### Display 
---
+ The prompt display is same as VS code's standard terminal, in prompt it considers the directory from which the terminal is started as the home directory as mentioned in the project description.
+ Home is represented with  `~` and the syntax followed is
`Username@Systemname:~/$`.
+ When the user changes the directory if it is deeper than the home directory then path after `~` will be printed else whole path will be printed .
### Tokenization
---
+ The input first tokenized according `&` and `;` and then for implementing pipes and redirection, it is first tokenized using `|` and the stdout and stdin are change according to the piping required.
    #### Separator `;`
    It is just for separating the commands without any specific change.
    #### Separator `&`
    It is used for executing the process in background(by default the process is executed in foreground).

+ The command then is tokenized according to redirections `>` , `>>` and `<` for performing redirections.

### Warp
---
USAGE:

    warp [ARGS]*

 
+ warp command is used for changing directories and path is printed everytime warp is called.
+ the accepted arguments are `.` (for the printing the current directory files and directories) , `..` (for moving to the parent directory) , `~` (for moving to the home directory) and  `-` ( for moving to the previous directory(if it is the first time then it will remain in the current directory)).
+ For multiple inputs to the command it executes them sequentially and reference changes dynamically according to the current directory.


### Peek
---

USAGE : 



    peek -a <path/name>
    peek -l <path/name>
    peek -a -l <path/name>
    peek -l -a <path/name>
    peek -la <path/name>
    peek -al <path/name>
+ Peek command prints all the files and directories in the current working directory.
+ The accepted arguments are `-l` and `-a`.
    #### `-l`
    It prints all the details of the files and directories following the syntax
    + `Block size`
    + `permissions` 
    + `Number of links`
    + `Username`
    + `Groupname`
    + `Size`
    + `Month of creation`
    + `Date of creation`
    + `Year or time of creation`
    + `File or Directory name`
    #### `-a`
    It prints all the files including the hidden files and directories


+ The color scheme is of printing the files/Directories is green for executables, white for files and blue for directories


### pastevents

Usage : `pastevents execute [0<=index<=15]`
- Here, specifying only pastevents, will print previous 15 commands and are stored in **past.txt**
- If the new command entered is same as **previous** command, then it will not be stored in past.txt
- **pastevents purge** clears past.txt file
- **pastevents execute** index will Execute the command at position in pastevents
---

### proclore 

Usage : `proclore [pid]`

- Displays the pid, process status, memory consumption and executable path for the specified process
- Passing a valid `pid` displays the info related to the specified `pid`
- If no arguments are passed, instead displays the info related to the current *Terminal* process

---
### seek
Usage : `seek <flags> <search> <target_directory>`

- Flags :
  - -d : Only look for directories (ignore files even if name matches)
  - -f : Only look for files (ignore directories even if name matches)
  - -d and -f flag can’t be used at the same time.
  - If No flags are given, then it will look for both files and directory
  - -e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it

- search :
  - Write the file or directory that needs to be searched inside the `<target_directory>`  tree

- target_directory :
  - Write the folder in whose the file or folder is to be printed

- “No match found!” will be printed in case no matching files/directories is found.


-  It returns a list of relative paths (from target directory) of all matching files/directories *(files in green and directories in blue)*** separated with a newline character.




### Signals
- `<ctrl>C` or `SIGINT` does not kill *Terminal*
- `<ctrl>Z` or `SIGTSTP` does not kill *Terminal*
- `<ctrl>D` or an `EOF` safely logs out of the shell (functionally equivalent to the `exit` command)
---
### Activities 
- **activities** print a list of all the processes currently running that were spawned by shell in **lexicographic** order.
- This list will contain the following information about all processes :
  - Command Name
  - pid
  - state : running, stopped and finished

- If a process is **finished**, then it will be printed only once. 


---
### fg 

Usage: `fg [pid]`

- The `pid` can be any of the pids listed using the command `activities`


### bg 
---
Usage: `bg [pid]`

- The `job_number` can be any of the pids listed using the command `activities`
---
### Neonate 

Usage: `neonate -n N`


  - `-n` followed by a positive integer `[N > 0]` which specifies the interval for printing the command output
  - This prints the PID of the most recently created process on the system after every `N` seconds
- A keypress of `x` stops execution of the command immediately .
---
### Ping 

Usage : `ping [pid] [signal_number]`

- The `pid` can be any of the job numbers listed using the command `jobs`
- The `signal_number` can be any of the signals listed under the manpage signal(7)


# Assumptions
- the whole sheel assumes that the input string sizes are less than 1024.

- if wrong commmand is entered then the error message printed will be "Invalid ccc".


- in Seek if the file doesnot have permission to read or write then the use is assumend to use the sheel in root mode (to do so replace the step 2 in running the shell to "sudo ./a.out").

- all the pastevent commands(atmost 15) are stored in the file `past.txt`.

- please enter `.` if you want to use home directory 