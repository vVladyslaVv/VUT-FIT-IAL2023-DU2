/**
 * tady to da do tabulky kazde slovo na vstupu
 * 
 * takze da se spustit napriklad
 * cat /usr/dict/words | ./test_muj
 * 
 * a nebo
 * 
 * cat /usr/dict/words > slova.txt
 * valgrind ./test_muj < slova.txt
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include "hashtable.h"

#define MAX_WORD_LEN 1024

/* pocet slov v /usr/share/dict/american-english-insane */
#define WRD_CNT 654936

int read_word(char *s, int max, FILE *f) {

    // first_letter
    int first_let;

    // projizdi to pres bile znaky dokud nenarazi na pismenko
    while (isspace( (char)(first_let = fgetc(f)) ) && first_let != EOF) {}
    
    // bud vrati eof nebo zapise prvni pismenko
    if (first_let != EOF) {
        s[0] = first_let;
    } else {
        return EOF;
    }

    // projizdi dalsi pismenka dokud nenarazi na bile nebo konec souboru
    int i;
    int c;
    for (i = 1; i < max - 1; i++) {

        // je to pismenko
        if (!isspace( (char)(c = fgetc(f)) )) {
            s[i] = c;

        // neni to pismenko
        } else {
            s[i] = '\0';
            break;
        }

        // je to eof
        if (c == EOF) {
            s[i] = '\0';
            break;
        }
    }

    return i;
}

int main() {
    ht_table_t table_arr;
    ht_table_t *table = &table_arr;
    ht_init(table);
    char str[MAX_WORD_LEN];
    unsigned int i = 0;

    /* tenth of a percent */
    unsigned int pct_tth = 0;

    while (read_word(str, MAX_WORD_LEN, stdin) != EOF) {
        ht_insert(table, str, 0.0);
        i++;
        if (i % (WRD_CNT / (100 * 10)) == 0) {
            pct_tth++;
            printf("%u,%u %%\n", pct_tth / 10 - 1, pct_tth - ((pct_tth / 10) * 10));
        }
    }
    printf("nacteno %u slov\n", i);
    ht_delete_all(table);
    return 0;
}