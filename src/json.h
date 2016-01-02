/**
 * @file
 * @author Michael Williams <michael@mgwsoftware.com>
 * @version 0.1
 */

#ifndef JSON_H
#define JSON_H

#define JSON_SUCCESS 0
#define JSON_ERR_INVALID 1

#include <stdio.h>
#include <stdbool.h>

/**
 * @struct JSON_ITEM
 * @brief JSON value, such as a numeric, string, boolean, list or object.
 */
struct JSON_ITEM;

/**
 * @struct JSON_ENTRY
 * @brief Key-Value JSON Pair, with a key, and a value.
 */
struct JSON_ENTRY;

struct JSON_ITEM* json_list();
struct JSON_ITEM* json_dictionary();
struct JSON_ITEM* json_string(char* value);
struct JSON_ITEM* json_int(int value);
struct JSON_ITEM* json_long(long value);
struct JSON_ITEM* json_float(float value);
struct JSON_ITEM* json_double(double value);
struct JSON_ITEM* json_bool(bool value);
struct JSON_ITEM* json_null();
struct JSON_ENTRY* json_entry(char* key, struct JSON_ITEM* value);
int json_dictionay_add_entry(struct JSON_ITEM* dictionary, struct JSON_ENTRY* entry);
int json_list_add_item(struct JSON_ITEM* list, struct JSON_ITEM* item);

int json_print(FILE* output, struct JSON_ITEM* root);

#endif /* JSON_H */
