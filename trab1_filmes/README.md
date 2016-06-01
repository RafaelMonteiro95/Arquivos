Makefile rules

all: compile project<br>
run: run executable<br>
clean: clean object files and zip/tar<br>
zip/tar: compress project folder<br>

======================

set 'debug=1' to compile/run in debug mode<br>
set 'ARGS=*args*' to feed the program with *args*<br>
set 'IN=*filename*' to feed a file to the program<br>
set 'OUT=*filename*' to write program output to designed file<br>
use CFLAGS+=*flags*\ to add *flags*<br>
