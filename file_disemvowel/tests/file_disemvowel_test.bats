#!/usr/bin/env bats

# Create a temporary scratch directory to work with
setup() {
    BATS_TMPDIR=`mktemp --directory`
}

# Remove the scratch directory
teardown() {
    rm -rf "$BATS_TMPDIR"
}

@test "file_disemvowel exists" {
    [ -f "file_disemvowel" ]
}

@test "file_disemvowel is executable" {
    [ -x "file_disemvowel" ]
}

@test "file_disemvowel runs successfully" {
    run ./file_disemvowel < tests/small_input > "$BATS_TMPDIR"/small_output
    [ "$status" -eq 0 ]
}

@test "file_disemvowel works with standard input and standard output" {
    ./file_disemvowel < tests/small_input > "$BATS_TMPDIR"/small_output
    run diff -wbB tests/small_output "$BATS_TMPDIR"/small_output
    [ "$status" -eq 0 ]
}

@test "file_disemvowel works with large files stdin/stdout" {
    ./file_disemvowel < tests/as_you_like_it.txt > "$BATS_TMPDIR"/as_you_like_it.output
    run diff -wbB tests/as_you_like_it_disemvowelled.txt "$BATS_TMPDIR"/as_you_like_it.output
    [ "$status" -eq 0 ]
}

# If this test hangs, you're probably trying to read from standard input
# instead of from the file provided on the command line.
# Hit ^D to terminate the program and check that your program
# handles command line arguments properly.
@test "file_disemvowel works with specified input and standard output" {
    # skip "Remove the 'skip' line in this test when you've implemented command line argument handling."
    ./file_disemvowel tests/small_input > "$BATS_TMPDIR"/small_output
    run diff -wbB tests/small_output "$BATS_TMPDIR"/small_output
    [ "$status" -eq 0 ]
}


# If this test hangs, you're probably trying to read from standard
# input instead of from the file provided on the command line.
# Hit ^D to terminate the program and check that your program
# handles command line arguments properly.
#
# If this fails and it looks like you're printing out the result
# of disemvoweling, then you may be writing to standard output
# instead of to the file specified as a command line argument.
@test "file_disemvowel works with specified input and output files" {
    # skip "Remove the 'skip' line in this test when you've implemented command line argument handling."
    rm -f "$BATS_TMPDIR"/small_output
    ./file_disemvowel tests/small_input "$BATS_TMPDIR"/small_output
    run diff -wbB tests/small_output "$BATS_TMPDIR"/small_output
    [ "$status" -eq 0 ]
}

# If this test hangs, you're probably trying to read from standard
# input instead of from the file provided on the command line.
# Hit ^D to terminate the program and check that your program
# handles command line arguments properly.
#
# If this fails and it looks like you're printing out the result
# of disemvoweling, then you may be writing to standard output
# instead of to the file specified as a command line argument.
@test "file_disemvowel works with large specified files" {
    # skip "Remove the 'skip' line in this test when you've implemented command line argument handling."
    ./file_disemvowel tests/as_you_like_it.txt "$BATS_TMPDIR"/as_you_like_it.output
    run diff -wbB tests/as_you_like_it_disemvowelled.txt "$BATS_TMPDIR"/as_you_like_it.output
    [ "$status" -eq 0 ]
}

