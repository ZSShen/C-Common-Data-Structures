#!/usr/bin/python

import sys;
import re;
import os;


HINT_EXPORT_FUNCTION    = "#ifdef BUILD_PLUGIN";
PAT_FUNCTION_SIGNATURE  = "([a-zA-Z0-9_\* ]+) ([a-zA-Z0-9_]+)\(([a-zA-Z0-9_ ,\*\(\)]*)\);";

KEY_RETURN_AND_QUALIFY  = 'r';
KEY_FUNCTION_NAME       = 'n';
KEY_PARAMETER           = 'p';

MACRO_IFNDEF = "#ifndef";
MACRO_DEF    = "#define";
MACRO_ENDIF  = "#endif";

COMMENT_SPACE_INTERVAL  = "     ";
COMMENT_MARK_HORIZONTAL = '-';
COMMENT_MARK_VERTICAL   = '*';
COMMENT_CORNER          = '/';

SECTION_PROLOGUE    = "The common interface for";
SECTION_TABLE_INDEX = "The index mapping table of exported functions.";
SECTION_TABLE_NAME  = "The name mapping table of exported functions.";
SECTION_PTR_PROTO   = "The function pointer types of exported functions.";


def main():

    path_header = sys.argv[1];    
    path_interface = sys.argv[2];

    list_exp_func = list();
    get_export_function_list(path_header, list_exp_func);

    name_interface = os.path.basename(path_interface)[:-2];
    h_interface = open(path_interface, 'w');
    put_interface_prologue(h_interface, name_interface);
    put_interface_epilogue(h_interface);
    h_interface.close();
    return;


def get_export_function_list(path_header, list_exp_func):

    h_header = open(path_header, 'r');
    have_exp = False;
    for line in h_header:    
        line = line[:-1];

        # The mark of export function list is found.
        if line == HINT_EXPORT_FUNCTION:
            have_exp = True;
            continue;            

        if have_exp == True:
            # A function signature is found.
            mat_func_sig = re.match(PAT_FUNCTION_SIGNATURE, line);
            if mat_func_sig:

                # Match the return type and function qualifier.    
                return_n_qualify = mat_func_sig.group(1);
                # Match the function name.
                name = mat_func_sig.group(2);
                # Match the parameters. 
                param = mat_func_sig.group(3);

                dict_func = dict();
                dict_func[KEY_RETURN_AND_QUALIFY] = return_n_qualify;
                dict_func[KEY_FUNCTION_NAME] = name;
                dict_func[KEY_PARAMETER] = param;
                list_exp_func.append(dict_func);

    h_header.close();
    return;


def put_interface_prologue(h_interface, name_interface):

    upper_name = name_interface.upper();
    line = "%s _%s_H_\n" % (MACRO_IFNDEF, upper_name);
    h_interface.write(line);
    line = "%s _%s_H_\n" % (MACRO_DEF, upper_name);    
    h_interface.write(line);
    h_interface.write('\n');

    interface_title = " %s%s%s \"%s\"%s%s\n" % (COMMENT_MARK_VERTICAL, COMMENT_SPACE_INTERVAL, \
                                                SECTION_PROLOGUE, name_interface, \
                                                COMMENT_SPACE_INTERVAL, COMMENT_MARK_VERTICAL);
    len_title = len(interface_title);
    rep_times = len_title - 4;

    board_up = "%s%s" % (COMMENT_CORNER, COMMENT_MARK_VERTICAL);
    for idx in xrange(rep_times):
        board_up += COMMENT_MARK_HORIZONTAL;
    board_up += "%s\n" % (COMMENT_MARK_VERTICAL);    
    
    board_down = " %s" % (COMMENT_MARK_VERTICAL);
    for idx in xrange(rep_times):
        board_down += COMMENT_MARK_HORIZONTAL;
    board_down += "%s%s\n" % (COMMENT_MARK_VERTICAL, COMMENT_CORNER);
    
    h_interface.write(board_up);
    h_interface.write(interface_title);
    h_interface.write(board_down);
    h_interface.write("\n\n");
    return;


def put_interface_epilogue(h_interface):

    line = "%s\n" % (MACRO_ENDIF);
    h_interface.write(line);
    return;


if __name__ == "__main__":
    main();
