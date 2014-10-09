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

COMMENT_MARK_HORIZONTAL = '-';
COMMENT_MARK_VERTICAL   = '*';
COMMENT_CORNER          = '/';
SPACE_TAB_WIDTH         = "    ";

SECTION_PROLOGUE    = "The common interface for";
SECTION_TABLE_INDEX = "The index mapping table of exported functions.";
SECTION_TABLE_NAME  = "The name mapping table of exported functions.";
SECTION_TABLE_PTR   = "The function pointer types of exported functions.";

FUNCTION_INDEX_PREFIX = "FUNC";
FUNCTION_TOTAL_COUNT  = "NUM_FUNCS";
FUNCTION_PTR_PREFIX   = "FPTR";


def main():

    path_header = sys.argv[1];    
    path_interface = sys.argv[2];

    # Parse the given header file to collect function signatures.
    list_exp_func = list();
    get_export_function_list(path_header, list_exp_func);

    name_interface = os.path.basename(path_interface)[:-2];
    h_interface = open(path_interface, 'w');

    # Generate the import guard for the interface file.
    put_interface_prologue(h_interface, name_interface);
    
    # Generate the index mapping for the exported functions.
    put_interface_index_table(h_interface, list_exp_func);

    # Generate the name mapping for the exported functions.
    put_interface_name_table(h_interface, list_exp_func);

    # Generate the function pointer mapping for the exported functions.
    put_interface_pointer_table(h_interface, list_exp_func);

    # Close the import guard.
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

    # Generate the interface description. 
    interface_title = " %s%s%s \"%s\"%s%s\n" % (COMMENT_MARK_VERTICAL, SPACE_TAB_WIDTH, \
                                                SECTION_PROLOGUE, name_interface, \
                                                SPACE_TAB_WIDTH, COMMENT_MARK_VERTICAL);
    len_title = len(interface_title);
    rep_times = len_title - 4;

    board_up = "/*";
    for idx in xrange(rep_times):
        board_up += COMMENT_MARK_HORIZONTAL;
    board_up += "*\n";    
    
    board_down = " %s" % (COMMENT_MARK_VERTICAL);
    for idx in xrange(rep_times):
        board_down += COMMENT_MARK_HORIZONTAL;
    board_down += "*/\n";
    
    h_interface.write(board_up);
    h_interface.write(interface_title);
    h_interface.write(board_down);
    h_interface.write("\n\n");
    return;


def put_interface_epilogue(h_interface):

    line = "%s\n" % (MACRO_ENDIF);
    h_interface.write(line);
    return;


def put_interface_index_table(h_interface, list_exp_func):

    # Generate the description for function index table.
    line = "/* %s */\n" % (SECTION_TABLE_INDEX);
    h_interface.write(line);

    # Generate the function index table.
    line = "enum {\n";
    h_interface.write(line);
    
    for dict_func in list_exp_func:
        name = dict_func[KEY_FUNCTION_NAME];
        caps_name = name.upper();
        line = "%s%s_%s,\n" % (SPACE_TAB_WIDTH, FUNCTION_INDEX_PREFIX, caps_name);
        h_interface.write(line);
    
    line = "%s%s\n" % (SPACE_TAB_WIDTH, FUNCTION_TOTAL_COUNT);
    h_interface.write(line);

    line = "};\n\n\n";
    h_interface.write(line);
    return;


def put_interface_name_table(h_interface, list_exp_func):

    # Generate the description for function name table.
    line = "/* %s */\n" % (SECTION_TABLE_NAME);
    h_interface.write(line);

    # Generate the function name table.
    line = "#define gTableFuncName ((char const *[]) {";
    h_interface.write(line);
    
    str_alias = "";
    len_alias = len(line);
    for idx in xrange(len_alias):
        str_alias += " ";

    count_exp_func = len(list_exp_func);
    if count_exp_func < 1:
        return;

    idx_bgn = 1;
    idx_end = count_exp_func - 1;
    line = " \"%s\", \\\n" % (list_exp_func[idx_bgn][KEY_FUNCTION_NAME]);
    h_interface.write(line);

    for idx in xrange(1, count_exp_func - 1):
        line = "%s \"%s\", \\\n" % (str_alias, list_exp_func[idx][KEY_FUNCTION_NAME]);
        h_interface.write(line);

    line = "%s \"%s\"" % (str_alias, list_exp_func[idx_end][KEY_FUNCTION_NAME]);
    h_interface.write(line);

    line = " })\n\n\n";
    h_interface.write(line);
    return;


def put_interface_pointer_table(h_interface, list_exp_func):

    # Generate the description for function pointer table.
    line = "/* %s */\n" % (SECTION_TABLE_PTR);
    h_interface.write(line);

    # Generate the function pointer table.
    for dict_func in list_exp_func:
        return_n_qualify = dict_func[KEY_RETURN_AND_QUALIFY];
        name = dict_func[KEY_FUNCTION_NAME];
        caps_name = name.upper();
        param = dict_func[KEY_PARAMETER];
        line = "typedef %s (*%s_%s) (%s);\n" % (return_n_qualify, FUNCTION_PTR_PREFIX, \
                                                caps_name, param);
        h_interface.write(line);

    h_interface.write("\n\n");
    return;


if __name__ == "__main__":
    main();
