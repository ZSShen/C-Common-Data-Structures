#!/usr/bin/python

import random;
import string;
import os;


DICTIONARY_SIZE     = 8192;
SEARCH_CASE_SIZE    = 4096;
DELETE_CASE_SIZE    = 1024;
WORD_LENGTH         = 8;
MAJOR_STRUCTURE     = "Tree";
MINOR_STRUCTURE     = "SparseTrie";
CASE_INSERT         = "insert.case";
CASE_SEARCH         = "search.case";
CASE_DELETE         = "delete.case";


def main():

    # Prepare the dictionary.
    dictionary = list();
    char_pool = string.lowercase;
    for idx in xrange(DICTIONARY_SIZE):
        word_length = random.randrange(1, WORD_LENGTH);
        word = "".join(random.sample(char_pool, word_length));
        dictionary.append(word);
    
    # Split the dictionary into insertion, searching, and deletion test cases.
    path_misc = os.path.abspath(os.path.dirname(__file__));
    path_proj_root = os.path.dirname(path_misc);
    path_sparse_trie = os.path.join(path_proj_root, MAJOR_STRUCTURE, MINOR_STRUCTURE);
    path_insert = os.path.join(path_sparse_trie, CASE_INSERT);
    path_search = os.path.join(path_sparse_trie, CASE_SEARCH);
    path_delete = os.path.join(path_sparse_trie, CASE_DELETE);
    dump_words(path_insert, dictionary, DICTIONARY_SIZE, DICTIONARY_SIZE);
    dump_words(path_search, dictionary, SEARCH_CASE_SIZE, DICTIONARY_SIZE);
    dump_words(path_delete, dictionary, DELETE_CASE_SIZE, DICTIONARY_SIZE);

    return;


def dump_words(path_output, dictionary, dump_size, random_limit):

    hdle_output = open(path_output, "w");
    for count in xrange(dump_size):
        idx = random.randrange(random_limit);
        line = "%s\n" % (dictionary[idx]);
        hdle_output.write(line);
    hdle_output.close();
    return;


if __name__ == "__main__":
    main();


