#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./multiple_files",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 0:
    print("Did not exit with code 0, exit_code=" + str(proc.returncode))
    exit(1)

if "[P] TestCase1::test" not in str(proc.stdout):
    print("Did not output correct feedback on passing test in first test case,"
          " output='" + str(proc.stdout) + "'")
    exit(1)

if "[P] TestCase2::test" not in str(proc.stdout):
    print("Did not output correct feedback on passing test in second test case,"
          " output='" + str(proc.stdout) + "'")
    exit(1)

if "Tests passed: 2" not in str(proc.stdout):
    print("Did not output correct feedback on number of passed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "Tests failed: 0" not in str(proc.stdout):
    print("Did not output correct feedback on number of failed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)
