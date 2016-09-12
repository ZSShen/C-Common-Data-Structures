#include "cds.h"


static const int BUF_SIZE = 32;


int main()
{
    /* We should initialize the container before any operations. */
    Trie* trie = TrieInit();

    const char* alpha_cap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
    const char* alpha = "abcdefghijklmnopqrstuvwxyz\0";

    /* Insert strings into the trie. */
    char buf[BUF_SIZE];
    int i;
    for (i = 0 ; i < 26 ; ++i) {
        buf[i] = alpha_cap[i];
        buf[i + 1] = 0;
        trie->insert(trie, buf);
    }
    for (i = 0 ; i < 26 ; ++i) {
        buf[i] = alpha[i];
        buf[i + 1] = 0;
        trie->insert(trie, buf);
    }

    /* Apply bulk insertion to put an array of strings into the trie. */
    const char* nums[6];
    nums[0] = "202-555-0104\0";
    nums[1] = "202-555-0175\0";
    nums[2] = "202-556-0171\0";
    nums[3] = "202-555-9857\0";
    nums[4] = "202-552-0180\0";
    nums[5] = "202-455-7104\0";
    trie->bulk_insert(trie, nums, 6);

    /* Search for the existing strings. */
    assert(trie->has_exact(trie, "abcdefghijklmn") == true);
    assert(trie->has_exact(trie, "bcdefghijklmn") == false);

    /* Search for the strings matching the specified prefixes. */
    assert(trie->has_prefix_as(trie, "A") == true);
    assert(trie->has_prefix_as(trie, "BCD") == false);

    /* Remove the string in the trie. */
    trie->remove(trie, nums[3]);

    /* Get the array of strings matching the specified prefix. */
    const char** strs;
    unsigned size;
    trie->get_prefix_as(trie, "202", &strs, &size);
    assert(size == 5);

    /* The returned strings are sorted by lexical order. */
    assert(strcmp(strs[0], nums[5]) == 0); /* 202-455-7104 */
    assert(strcmp(strs[1], nums[4]) == 0); /* 202-552-0180 */
    assert(strcmp(strs[2], nums[0]) == 0); /* 202-555-0104 */
    assert(strcmp(strs[3], nums[1]) == 0); /* 202-555-0175 */
    assert(strcmp(strs[4], nums[2]) == 0); /* 202-556-0171 */

    /* Remember to free the returned array of strings. */
    for (i = 0 ; i < size ; ++i)
        free((char*)strs[i]);
    free(strs);

    TrieDeinit(trie);
    return 0;
}
