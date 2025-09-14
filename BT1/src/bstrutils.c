
#include "../inc/strutils.h"
char* str_reverse(char* str_input) {
    int len = strlen(str_input);
    char* result = malloc(len + 1);

    for (int i = 0; i < len; i++) {
        result[i] = str_input[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}
char* str_trim(char* str_input) {
    char* start = str_input;
    while (isspace((unsigned char)*start)) start++;
    char* end = str_input + strlen(str_input) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    int len = end - start + 1;
    char* result = malloc(len + 1);
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}
int str_to_int(char* str_input) {
    return atoi(str_input);
}