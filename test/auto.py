#!/usr/bin/python
import subprocess as sub;
import os;


def main():
    # Run all the unit tests.
    '''
    line  = "***************************\n";
    line += "*        UNIT TEST        *\n";
    line += "***************************";
    print line;
    run_test("../bin/test");
    '''
    # Run all the demo programs.
    line  = "******************************\n";
    line += "*        DEMO PROGRAM        *\n";
    line += "******************************";
    print line;
    run_test("../bin/demo");

    return;


def run_test(path_test):
    list_case = os.listdir(path_test);
    for name_case in list_case:
        path_case = os.path.join(path_test, name_case);
        prog = sub.Popen([path_case], stdout=sub.PIPE, stderr=sub.STDOUT);
        result = "";
        while True:
            line = prog.stdout.readline();
            if line == "":
                break;
            result += line;
        print result;
        prog.wait();
        rtn_code = prog.returncode;

        if rtn_code != 0:
            exit(rtn_code);

    return


if __name__ == "__main__":
    main();
