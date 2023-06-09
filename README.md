# Paperize

Paperize is a program able to lay out a text as a newspaper article. The text to be converted is given as an input `.txt` file and the output gets written as a new `.txt` file (`output.txt` as a default name).

For example, the given input file:
```text
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.

Mauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.
```

Gets converted to the following output file:

```text
Lorem  ipsum   dolor          ullamcorper,                  hendrerit  dui   sed
sit            amet,          accumsan     dapibus          maximus    suscipit.
consectetur                   justo.  Donec   elit          Praesent   tristique
adipiscing     elit.          libero,  ullamcorper          quam erat, eu mollis
Quisque      euismod          id   interdum    et,          ante  malesuada   a.
mauris ac  malesuada          porttitor non metus.          Phasellus     rutrum
sagittis.      Morbi          In ultrices non arcu          tempus  lorem,   vel
imperdiet                     tincidunt                     vulputate       ante
ullamcorper  tortor,          scelerisque.   Morbi          lobortis   in.   Nam
eget   varius    sem          at lectus quis  nunc          lacinia   ut    arcu
ultrices   a.    Nam          faucibus  mattis  id          vitae suscipit.     
molestie   dignissim          et  ex.  Ut  viverra                              
lectus,  et  dapibus          nunc arcu, at mollis          Mauris      volutpat
nunc  tristique  in.          orci ultricies  non.          purus placerat lacus
Sed    eget    neque          Etiam tempor euismod          vestibulum, sit amet
egestas,     aliquam          neque           eget          bibendum        arcu
lacus  non,  rhoncus          imperdiet.     Fusce          tincidunt.     Fusce
ex.      Suspendisse          faucibus sem  velit,          volutpat orci in  ex
metus orci,  gravida          at pellentesque odio          suscipit    euismod.
eget         gravida          sagittis  vel.  Duis          Donec    mi    nibh,

%%%

blandit vitae ligula          varius,          nec
non,          cursus          sollicitudin  ligula
fermentum     dolor.          pulvinar.  Curabitur
Mauris   sit    amet          bibendum  arcu  quis
venenatis      nisi.          erat tincidunt,  sed
Curabitur quis dolor          tempus ex  placerat.
at metus consectetur          Donec ornare, tellus
congue.   Ut   velit          pulvinar            
nunc,  ultrices  sed          pellentesque        
convallis        eu,          elementum, arcu ante
posuere   a    arcu.          pharetra  metus,  ut
Vivamus    tristique          malesuada metus nibh
finibus      sapien,          ut   orci.    Aenean
vitae  lacinia  ante          pulvinar nisi  arcu,
dapibus        eget.          a pharetra mi ornare
Praesent      libero          nec.                
nisl, efficitur  vel
ex in, semper tempor
erat.        Aliquam
pharetra mi nec nisi
```

---------

## Compilation and Use

To compile and run the program, simply run `make` followed by `./paperize`.
The paging layout can be specified with input parameters.

For example, the previous text was converted by running:

```text
./paperize lorem_ipsus.txt -w 20
```

Follow the instructions given in the help message (printable by running `./paperize -h` or `./paperize --help`) to personalize the layout. If some flags are omitted, the **default values** will be used.

```text
CORRECT USAGE
    ./paperize input_filename.txt [FLAGS]

FLAGS
    -h  | --help            Prints this message without executing program

    -c  | --columns         Specifies the number of columns to be used for each page
                            The given value must be a positive integer.
                            If not used, the default value is set to 3.

    -r  | --rows            Specifies the number of rows to be used for each column
                            The given value must be a positive integer.
                            If not used, the default value is set to 20.

    -w  | --width           Specifies the number of rows to be used for each column
                            The given value must be a positive integer.
                            If not used, the default value is set to 40.

    -s  | --spacing         Specifies the number of rows to be used for each column
                            The given value must be a positive integer.
                            If not used, the default value is set to 10.

    -o  | --output          Specifies the name of the output file to be written.
                            If not used, the default file "output.txt" will be written.

    -p  | --parallel        If used, the program will run as 3 parallel sub-processes,
                            speeding up computation.

EXAMPLE USAGE
    ./paperize test.txt -c 3 -r 20 -w 40 -s 10

    Paperizes the given "test.txt" file using 3 columns for each page,
    20 rows for each column, 40 characters for each column row,
    10 characters as spacing between each column (same default
    behaviour as if none of the flags where specified)
```

-------

## Program Structure

### Single-process version

The single-process version consists of 3 steps:
- The input file gets read and converted into pages
- Each page gets enqueued in a Queue
- While writing the output file, each page gets dequeued and formatted

### Multi-process version

The multi-process version consists of 3 processes: the Reader Process, the Pager rocess and the Writer process

The processes are interlinked by using pipes as a comunication method:

```text
┌────────┐                 ┌───────┐                 ┌────────┐
│ READER ├──── pipe_rp ────┤ PAGER ├──── pipe_pw ────┤ WRITER │
└────────┘                 └───────┘                 └────────┘
```

Each process executes an atomic job:
- The Reader process reads each word of the file, sending them though a pipe comunicating with the Pager process
- The Pager process reads the words from the pipe, slowly creating each page. When a page is completed, the Pager process formats it's output strings and sends them through a pipe comunicating with the Writer process
- The Writer process reads the strings from the pipe, writing them to the output file

### Source files structure

The source files are structured as following:
- The `core` directory contains the main functions needed for reading, writing and paging the text
- The `spark` directory contains the entry point to the single-process and multi-process versions of the program
- The `data_structs` directory contains the main data structures used by the `core` directory (Queues and Linked Lists)
- The `utils` directory contains functions used for parsing the given program arguments and managing strings 

Each function returned values, returned errors and actions are documented in each `.c` and `.h` file.

### Pages structure

The program uses the following data structures in order to correctly lay out  the output:
- Each Page contains a Linked List of Columns
- Each Column contains a Queue of Rows
- Each Row contains a Queue of Words
- Each Words contains a String representing a word read from the given input file

```text
QUEUE
  │
  ├─ PAGE 1
  │    │
  │    ├─ COLUMN 1
  │    │    │
  │    │    ├─ ROW 1
  │    │    │   │
  │    │    │   ├─ WORD 1
  │    │    │   │   └─ char*
  │    │    │   │
  │    │    │   ├─ ...
  │    │    │   │   
  │    │    │   └─ WORD k
  │    │    │       └─ char*
  │    │    │
  │    │    ├─ ...
  │    │    │
  │    │    └─ ROW r
  │    │
  │    ├─ ...
  │    │
  │    └─ COLUMN c
  │
  ├─ ...
  │
  └─ PAGE p
```
