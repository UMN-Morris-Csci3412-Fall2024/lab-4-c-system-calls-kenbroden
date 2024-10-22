[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/vFu6gESk)
# C System Calls <!-- omit in toc -->

[![Disemvowel tests](../../workflows/file-disemvowel-tests/badge.svg)](../../actions?query=workflow%3A"file-disemvowel-tests")
[![Disemvowel Valgrind](../../workflows/file-disemvowel-valgrind/badge.svg)](../../actions?query=workflow%3A"file-disemvowel-valgrind")
[![Summarize Tree tests](../../workflows/summarize-tree-tests/badge.svg)](../../actions?query=workflow%3A"summarize-tree-tests")
[![Summarize Tree Valgrind](../../workflows/summarize-tree-valgrind/badge.svg)](../../actions?query=workflow%3A"summarize-tree-valgrind")
[![Summarize Tree FTW Valgrind](../../workflows/summarize-tree-ftw-valgrind/badge.svg)](../../actions?query=workflow%3A"summarize-tree-ftw-valgrind")

- [Overview](#overview)
- [Getting started](#getting-started)
- [File disemvowel](#file-disemvowel)
  - [Read from standard input, write to standard output](#read-from-standard-input-write-to-standard-output)
  - [Read from a file, write to standard output](#read-from-a-file-write-to-standard-output)
  - [Read from a file, write to a file](#read-from-a-file-write-to-a-file)
  - [Test scripts and design suggestions](#test-scripts-and-design-suggestions)
- [Summarizing directories](#summarizing-directories)
  - [C using `stat()`](#c-using-stat)
    - [Static variables as "fields"](#static-variables-as-fields)
    - [Calling `stat()`](#calling-stat)
    - [Reading directories](#reading-directories)
    - [Checking for errors](#checking-for-errors)
  - [C using `ftw()`](#c-using-ftw)
  - [The shell using `find`](#the-shell-using-find)
- [To Do](#to-do)

## Overview

This repository contains the starter code and tests for the "C system calls" lab.
The primary goal of this lab is to gain some experience with (Unix) file system calls in
C, and compare these to using the shell.

For this lab, you will need to have (or develop) some familiarity with
several C programming concepts, including:

- Using command line arguments
- Using 2-Dimensional arrays
- Basics of file-system calls, including standard input (`stdin`) and standard output (`stdout`)
- Using `stat()`
- Using `struct`

For comparison, we will also use shell commands to traverse directories.

To accomplish these goals, you will work on solving two distinct
problems, both of which illustrate how we interact with the file system
from (C) programs. The first is revisiting an old friend from
[a previous C Lab](https://github.com/UMM-CSci-Systems/C-strings-and-memory-management#disemvowel), but this time
we disemvowel entire files instead of single lines. The second is three
different solutions to the problem of summarizing files; two
solutions will be in C and one uses shell commands.

There's a lot here, and only some of it is described below. You will definitely
need to do some reading on at least the following using your preferred resource(s):

- C file I/O, including stdin, stdout, `fopen()`, `fwrite()`, `fclose()`
- Command line arguments in C
- C structs
- C tools for processing directories, including `stat()`, `readdir()`, `chdir()`
- Function pointers in C (that's a head trip)
- The C `ftw()` (file tree walk) system call

## Getting started

There are two directories in the repository, one for each of the two
major parts of this lab. We would recommend doing them in the order
listed below; there's no overwhelming reason that you need to do them in
any particular order, however, and it would be far better to move on to
the next one rather than get buried in one and not make any progress.

---

## File disemvowel

This is an extension of the disemvoweling exercise from [a previous C Lab](https://github.com/UMM-CSci-Systems/C-strings-and-memory-management#disemvowel). Here
you should write a program that disemvowels an entire file, reading from
standard input or a file specified on the command line, and writing to
standard output or to a file specified on the command line.

It's common for command line utilities to be very flexible in how they
handle both input and output. This allows them to be easily used as both
"stand alone" utilities, and as combined with other programs using
pipes. To illustrate what that looks like from a programmer's standpoint,
you'll write a program `file_disemvowel` that can be called in several
different ways, each of which is listed below.

The difference in all of these is where the input comes from and the output
goes to. If the user doesn't provide any command line arguments, for example,
the the code should read from the "file" `stdin` and write to the "file"
`stdout`, where both `stdin` and `stdout` are pre-defined constants provided
in `<stdio.h>`. Your code should probably set up your file I/O using `stdin`
and `stdio` as the defaults, and then replace them with files specified by
the user if/when the user provides them. You'll use `fopen()` to open files
specified by the user, and you need to remember to use `fclose()` to close
the files when you finish. (Failing to close can lead to some
of the last data you wrote out not actually making it into the file, which
can cause tests to fail.)

### Read from standard input, write to standard output

```bash
$ ./file_disemvowel
Some text
consisting of 2 lines.
^D
Sm txt
cnsstng f 2 lns.
```

Note that we can also use I/O redirection and pipes to modify standard
input and standard output:

```bash
$ cat small_input
Vulcan once stood proud here,
until he spewed his parts from his dirty mouth,
shattering his body and the land alike.
-- Thomas McPhee, 20 Jul 2010
$ ./file_disemvowel < small_input > /tmp/output
$ cat /tmp/output
Vlcn nc std prd hr,
ntl h spwd hs prts frm hs drty mth,
shttrng hs bdy nd th lnd lk.
-- Thms McPh, 20 Jl 2010
```

### Read from a file, write to standard output

Your program should be able to take a single command line argument that
is interpreted as the name of the file to be disemvoweled. The output will
then go to standard output.

```bash
$ cat small_input
Vulcan once stood proud here,
until he spewed his parts from his dirty mouth,
shattering his body and the land alike.
-- Thomas McPhee, 20 Jul 2010
$ ./file_disemvowel small_input
Vlcn nc std prd hr,
ntl h spwd hs prts frm hs drty mth,
shttrng hs bdy nd th lnd lk.
-- Thms McPh, 20 Jl 2010
```

### Read from a file, write to a file

If your program is provided with two command line arguments, it should
interpret the first as the input file and the second as the file where
the output should be written.

```bash
$ cat small_input
Vulcan once stood proud here,
until he spewed his parts from his dirty mouth,
shattering his body and the land alike.
-- Thomas McPhee, 20 Jul 2010
$ ./file_disemvowel small_input /tmp/output
$ cat /tmp/output
Vlcn nc std prd hr,
ntl h spwd hs prts frm hs drty mth,
shttrng hs bdy nd th lnd lk.
-- Thms McPh, 20 Jl 2010
```

### Test scripts and design suggestions

The directory `file_disemvowel/tests` has a `bats` test
script (`file_disemvowel_test.bats`) and some test data. You want to start by trying to get
those tests to pass at a minimum but, as always, we make no guarantees
that those tests are in any way complete enough to ensure correctness. To
run the tests enter

```{bash}
bats tests/file_disemvowel_test.bats
```

in the `file_disemvowel` directory.

The basic structure of our solution is shown below. You should certainly feel
free to copy code from your earlier lab to help as appropriate, but you may
need to tweak things some to fit the new circumstances.

```C
#include <stdio.h>
#include <stdbool.h>

#define BUF_SIZE 1024

bool is_vowel(char c) {
    /*
     * Returns true if c is a vowel (upper or lower case), and
     * false otherwise.
     */
}

int copy_non_vowels(int num_chars, char* in_buf, char* out_buf) {
    /*
     * Copy all the non-vowels from in_buf to out_buf.
     * num_chars indicates how many characters are in in_buf,
     * and this function should return the number of non-vowels that
     * that were copied over.
     */
}

void disemvowel(FILE* inputFile, FILE* outputFile) {
    /*
     * Copy all the non-vowels from inputFile to outputFile.
     * Create input and output buffers, and use fread() to repeatedly read
     * in a buffer of data, copy the non-vowels to the output buffer, and
     * use fwrite to write that out.
     */
}

int main(int argc, char *argv[]) {
    // This sets these to `stdin` and `stdout` by default.
    // You then need to set them to user specified files when the user
    // provides files names as command line arguments.
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;

    // Code that processes the command line arguments
    // and sets up inputFile and outputFile.

    disemvowel(inputFile, outputFile);

    return 0;
}
```

A few comments:

- We've used `#define` to create a named constant which we used for the
    size of the two buffers in the function `disemvowel()`. The choice
    of 1024 for the buffer size is largely arbitrary. You often see these
    in multiples of a kilobyte. If you have a lot of RAM (which modern
    machines typically do) you might slightly improve performance by
    increasing this buffer size, but you'd be unlikely to be able to
    tell the difference without careful measurements.
- Rather than using the very low-level I/O tools `open()`, `read()`,
    and `write()`, we recommend using the slightly higher level tools `fopen()`,
    `fread()`, and `fwrite()`, as they're more like what you'd actually
    use in a text-processing program like this. You'll want to look at
    the man pages for `fopen()`, `fread()`, and `fwrite()` for the
    details, and definitely ask questions if you're not sure what's
    going on there.
- This uses command line arguments in C. You probably want to start by
    just reading standard input and writing to
    standard input, as that allows you to initially ignore the command
    line arguments and focus on the file operations; once you get that
    right, you should pass the first of the three tests. When that's
    done that you can start worrying about the command line arguments.
    There are lots of on-line resources for handling command line
    arguments in C, e.g., in the wikibook [A Little C Primer](http://en.wikibooks.org/wiki/A_Little_C_Primer/C_Command_Line_Arguments).

:bangbang: Until you get the command line argument handling working, the
last two tests are likely to hang indefinitely waiting for you to close
standard input. Both those tests are marked as `skip` to prevent them
from hanging when you first start the project. **You need to make sure
to remove (or comment out) the `skip` lines when think you have command
line argument processing working.** Otherwise your handling of command
line arguments won't actually be tested.

## Summarizing directories

Here we'll generate several different solutions to the same
problem, two in C (using different system tools), and one using shell commands.
The problem we're solving in each case is to write a program
that takes a single command line argument and looks at *every* file and
directory from there down (in sub-directories, sub-sub-directories,
etc.) and summarizes how many directories it finds and how many regular
files (non-directories) it finds. The output should look like:

```text
    Processed all the files from <test_data/loads_o_files/>.
    There were 1112 directories.
    There were 10001 regular files.
```

There is a test script in
`summarize_tree_test.bats` that will
only pass when all three programs exist and work using these names:

- `summarize_tree`
- `summarize_tree_ftw`
- `summarize_tree.sh`

The two files `test_data/small_dir_sizes` and `test_data/large_dir_sizes`
in the repo are necessary for the tests to pass; you shouldn't remove
or change them.

Remember to also use `valgrind` to look for memory leaks in both your
C versions.

:bangbang: **Before working on this you should expand the `test_data.tgz`
`tar` archive.** This contains large collections of test directories and
files (over 10,000 files) that we keep compressed in the repository so
we don't burden Github (and you) with pushing and pulling these large
collections of files. So execute

```bash
cd test_data
tar -zxf test_data.tgz
```

That should give you two directories: `loads_o_files` and `fewer_files`.
The former is a large directory structure with thousands of files and
directories. The latter is a smaller subset. The tests assume those
directories exist and won't pass unless they do.

### C using `stat()`

We'll start with a "traditional" C solution using the `stat()` and
`readdir()` library functions. We've given you a substantial stub for
this in `summarize_tree/summarize_tree.c` which
is also included below:

```C
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

static int num_dirs, num_regular;

bool is_dir(const char* path) {
    /*
     * Use the stat() function (try "man 2 stat") to determine if the file
     * referenced by path is a directory or not. Call stat, and then use
     * S_ISDIR to see if the file is a directory. Make sure you check the
     * return value from stat() in case there is a problem, e.g., maybe the
     * the file doesn't actually exist.
     *
     * You'll need to allocate memory for a buffer you pass to stat(); make
     * sure you free up that space before you return from this function or
     * you'll have a substantial memory leak (think of how many times this
     * will be called!).
     */
}

/*
 * This is necessary this because the mutual recursion means there's no way to
 * order them so that all the functions are defined before they're called.
 */
void process_path(const char*);

void process_directory(const char* path) {
    /*
     * Update the number of directories seen, use opendir() to open the
     * directory, and then use readdir() to loop through the entries
     * and process them. You have to be careful not to process the
     * "." and ".." directory entries, or you'll end up spinning in
     * (infinite) loops. Also make sure you closedir() when you're done.
     *
     * You'll also want to use chdir() to move into this new directory,
     * with a matching call to chdir() to move back out of it when you're
     * done.
     */
}

void process_file(const char* path) {
    /*
     * Update the number of regular files.
     * This is as simple as it seems. :-)
     */
}

void process_path(const char* path) {
    if (is_dir(path)) {
        process_directory(path);
    } else {
        process_file(path);
    }
}

int main (int argc, char *argv[]) {
    // Ensure an argument was provided.
    if (argc != 2) {
        printf ("Usage: %s <path>\n", argv[0]);
        printf ("       where <path> is the file or root of the tree you want to summarize.\n");
        return 1;
    }

    num_dirs = 0;
    num_regular = 0;

    process_path(argv[1]);

    printf("There were %d directories.\n", num_dirs);
    printf("There were %d regular files.\n", num_regular);

    return 0;
}
```

#### Static variables as "fields"

The `static int` declaration on line 9 gives us two semi-global
variables that are visible to all the functions *in this file*, but
invisible outside of this file. This gives us something sort of like
fields in a class in a language like Java,
except that you can't make "instances" of this file
so there are never more than these two integer variables. (This means if
you tried to use this approach for something like a stack, then you
couldn't ever have more than one stack.)

#### Calling `stat()`

In `is_dir()`, you'll want to use the `stat()` system call
(try `man 2 stat`) to get the "status" of the file identified by `path`.
The signature for `stat()` is

```C
int stat(const char *path, struct stat *buf);
```

This is typical of many library
functions where the "return value" is actually being written into a
pointer argument (`buf` in this case), and what's returned is an error
code. If the error code is 0, then `stat` worked fine, and its results
are stored in
`buf`; if the error code is non-zero then there was a problem
(e.g., the file didn't actually exist) and you can't count on `buf`
containing any meaningful information.

When it succeeds, what you get in `buf` is a whole
bunch of information including the [inode](https://en.wikipedia.org/wiki/Inode) number, access and change
times, etc. (see the `man` page for more). What we care about is the
`st_mode` field; to access that field you need to dereference the
pointer (i.e., `*buf`) and then use the dot notation to access the
field: `(*buf).st_mode`. (The parens
are necessary because dot `.` binds more tightly than star `*`.)
This syntax is sufficiently awkward that the
designers of C provided an alternative syntax for this common case
that allows us to use `x->y` instead of `(*x).y`. In our case that
means we can use `buf->st_mode` instead of `(*buf).st_mode`.

This `st_mode` field contains all the file permission information,
which includes whether it's a directory or not. We could try to extract
that information from `st_mode` directly, but they've given us a
`S_ISDIR()` macro that does that work for us. Then the key expression is
something like

```C
S_ISDIR(buf->st_mode)
```

An important point about `stat()` is that the function *doesn't* allocate space for
`buf`; you're supposed to have done that before you call `stat()`. You
can do this with `malloc()`, or you can
actually declare a `struct stat` locally (`struct stat buf`), and pass
it's address to `stat()` with `&buf`. If you do that, `buf` isn't a
pointer, so you don't need the arrow notation above and can just use

```C
S_ISDIR(buf.st_mode)
```

#### Reading directories

In `process_directory` you'll need to have a loop that reads
all the entries in the directory named by the given path, making
recursive calls to process those sub-paths. The basic approach is to use
`opendir()` to open the directory, repeatedly call `readdir()` to read
the next entry from the directory, and then call `closedir()` at the
end. There are a few bits you need to be careful of:

- You need use `chdir()` at the start of `process_directory()` to
    actually move into the directory you're processing. You'll then to
    use `chdir("..")` to move back up to the parent directory at the end
    of `process_directory()`.
- You need to check whether `readdir()` gave you either "." or ".." as
    the `d_name`. It will eventually do that since every directory
    contains both those entires. You *don't* want to make recursive
    calls on those, because you'll end up in an infinite loop.

#### Checking for errors

Since C tends to just crash horribly when something goes wrong, you want
to get in the habit of checking for errors so you can try to give your
user some useful feedback. I've provided an example in `main()` where I
check that the program is called with the right number of command line
arguments. You also want to check the result code returned by `stat()`
in `is_dir()`. You should also probably check for errors from things
like `opendir()`, `readdir()`, `closedir()`, and `chdir()`, although
errors there are less likely.

There is [a nice C function `perror()`](https://www.tutorialspoint.com/c_standard_library/c_function_perror.htm)
that will print out a somewhat
human readable error when something goes wrong. You might find that useful.

### C using `ftw()`

Having gotten the version with `stat()` to work, we're now going to do
this again, using a nice library tool called `ftw()` (for "file tree
walk"). Here you just need to call `ftw()` (which you can do straight
from `main()` after checking the command line arguments), and it does
all the recursive traversal of the file system for you. What you need to
do is define a function

```C
static int callback(const char *fpath, const struct stat *sb, int typeflag)
```

and pass it in as the second
argument of `ftw()`, something like this:

```C
static int callback(const char *fpath, const struct stat *sb, int typeflag) {
    // Define stuff here
}

#define MAX_FTW_DEPTH 16

int main(int argc, char** argv) {
    // Check arguments and set things up

    ftw(argv[1], callback, MAX_FTW_DEPTH);

    // Print out the results
}
```

`ftw()` will call your `callback()` function once for every file it
finds, giving you the opportunity to do whatever you wish with that
file. In our case we just determine whether it's a directory or not (the
`typeflag` parameter is very useful here) and update the appropriate
counter. In more general applications, however, you could check for file
types (image files, for example), copy things, compute their sizes, or
whatever.

### The shell using `find`

An alternative to writing this in C is to use shell commands.
Here we'd recommend using `find` to do the traversal for you (its
`-type` flag is probably useful), and let `wc` do the counting.
If you find that `wc` gives you white space you don't want, [you
can use `xargs` to strip that off](http://stackoverflow.com/a/12973694).

## To Do

The canvas rubric provides detailed information on how you will be graded.  The main topics revolve around

- [ ] Create the four executable files:
  - [ ] `disemvowel`
  - [ ] `summarize_tree`
  - [ ] `summarize_tree_ftw`
  - [ ] `summarize_tree.sh`
- [ ] Ensure that the programs pass their tests
- [ ] Ensure that the C programs pass valgrind tests
- [ ] Code and commits should be understandable and useful
