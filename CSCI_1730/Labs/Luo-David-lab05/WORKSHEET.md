1. wget https://ftp.gnu.org/gnu/ncurses/ncurses-6.0.tar.gz
2. -zxvf is filter the archive through gzip, extract, verbosely list the files
extracted, and use the file specified, repsectively.
3. It configures the library to work on many different systems.
4. It specifies where to put the library files.
5. 
6. It is used to specify extra directories that the library loader should look
through.
7. There is already an existing LD_LIBRARY_PATH environmental variable in 
.bash_login. You're just attaching more stuff to the beginning of it.
8. So that it sets the environmental variables on login.
9. A initializes the screen. B prints "Hello World !!!" onto the screen. C 
refreshes the screen to show step B. D pauses the console until a key is hit. E 
closes the screen.
10. It tells the compiler that there is a library that it needs to take into account.
11. It tells it to include the library in the executable.
