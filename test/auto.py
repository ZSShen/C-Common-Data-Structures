#!/usr/bin/python
import subprocess as sub
import os


def main():

    path_test = "../bin/test"
    list_case = os.listdir(path_test)
    for name_case in list_case:

        path_case = os.path.join(path_test, name_case)
        prog = sub.Popen(["valgrind", path_case], stdout=sub.PIPE, stderr=sub.STDOUT)
        result = "";
        while True:
            line = prog.stdout.readline();
            if line == "":
                break
            result += line
        print result
        prog.wait()
        rtn_code = prog.returncode

        if rtn_code != 0:
            exit(rtn_code)

    return


if __name__ == "__main__":
    main()
