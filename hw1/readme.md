# Implement a 'lsof'-like program

> In this homework, you have to implement the 'lsof' tool by yourself. 'lsof' is a tool to list open files. It can be used to list all the files opened by processes running in the system. The output of your homework is required to strictly follow the spec, the TAs will use the 'diff' tool to compare your output directly against our prepared sample sample testdata. Spaces and tabs are compressed into a single character when comparing the outputs.

- A sample output from this homework is demonstrated as follows:
    ```
    $ ./hw1  -c bash
        bash
        COMMAND PID     USER      	FD     TYPE     NODE     NAME      
        bash    26884   terrynini38514	cwd    DIR      57779    /media/psf/Home/Desktop
        bash    26884   terrynini38514	root   DIR      2        /         
        bash    26884   terrynini38514	exe    REG      1179741  /usr/bin/bash
        bash    26884   terrynini38514	mem    REG      1179741  /usr/bin/bash
        bash    26884   terrynini38514	mem    REG      1186555  /usr/lib/x86_64-linux-gnu/libnss_files-2.31.so
        bash    26884   terrynini38514	mem    REG      1185120  /usr/lib/locale/locale-archive
        bash    26884   terrynini38514	mem    REG      1185791  /usr/lib/x86_64-linux-gnu/libc-2.31.so
        bash    26884   terrynini38514	mem    REG      1185926  /usr/lib/x86_64-linux-gnu/libdl-2.31.so
        bash    26884   terrynini38514	mem    REG      1186902  /usr/lib/x86_64-linux-gnu/libtinfo.so.6.2
        bash    26884   terrynini38514	mem    REG      1708797  /usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache
        bash    26884   terrynini38514	mem    REG      1185576  /usr/lib/x86_64-linux-gnu/ld-2.31.so
        bash    26884   terrynini38514	0u     CHR      3        /dev/pts/0
        bash    26884   terrynini38514	1u     CHR      3        /dev/pts/0
        bash    26884   terrynini38514	2u     CHR      3        /dev/pts/0
        bash    26884   terrynini38514	255u   CHR      3        /dev/pts/0
    ```
    
- Program Arguments
    - -c REGEX: a regular expression (REGEX) filter for filtering command line. For example -c sh would match bash, zsh, and share.
    - -t TYPE: a TYPE filter. Valid TYPE includes REG, CHR, DIR, FIFO, SOCK, and unknown. TYPEs other than the listed should be considered as invalid. For invalid types, your program have to print out an error message Invalid TYPE option. in a single line and terminate your program.
    - -f REGEX: a regular expression (REGEX) filter for filtering filenames.

