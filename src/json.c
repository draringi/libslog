/**
 * @file
 * @author Michael Williams <michael@mgwsoftware.com>
 * @version 0.1
 */

#include "json.h"
#include <stdlib.h>
typedef unsigned char u_char;
#include <bsd/stdlib.h>
#include <stdio.h>

enum ITEM_TYPE {
	LIST_T,
	OBJECT_T,
	STRING_T,
	INT_T,
	LONG_T,
	FLOAT_T,
	DOUBLE_T,
	BOOL_T,
	NULL_T
};

struct JSON_LIST {
	struct JSON_ITEM* value;
	struct JSON_LIST* next;
};

struct JSON_OBJECT {
	struct JSON_ENTRY* entry;
	struct JSON_OBJECT* next;
};

/**
 * @struct JSON_ITEM json.h "json.h"
 * @brief JSON value, such as a numeric, string, boolean, list or object.
 */
struct JSON_ITEM {
	/**
	 * @brief Internal type of stored item, such as the C type, or list or object.
	 */
	enum ITEM_TYPE type;
	union {
		struct JSON_LIST* t_list;
		struct JSON_OBJECT* t_object;
		char* t_string;
		int t_int;
		long t_long;
		float t_float;
		double t_double;
		bool t_bool;
	} value;
};

/**
 * @struct JSON_ENTRY json.h "json.h"
 * @brief Key-Value JSON Pair, with a key, and a value.
 */
struct JSON_ENTRY {
	/**
	 * @brief Key for an entry in a JSON Object.
	 */
	char* key;
	/**
	 * @brief Value for an entry in a JSON Object.
	 */
	struct JSON_ITEM* value;
};

char* json_escape(char* input);

int json_print_item(FILE* output, struct JSON_ITEM* item);
int json_print_entry(FILE* output, struct JSON_ENTRY* entry);
int json_print_list(FILE* output, struct JSON_LIST* list);
int json_print_object(FILE* output, struct JSON_OBJECT* object);

struct JSON_ITEM* json_list(){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = LIST_T;
	item->value.t_list = NULL;
	return item;
}

struct JSON_ITEM* json_dictionary(){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = OBJECT_T;
	item->value.t_object = NULL;
	return item;
}

struct JSON_ITEM* json_string(char* value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = STRING_T;
	item->value.t_string = value;
	return item;
}

struct JSON_ITEM* json_int(int value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = INT_T;
	item->value.t_int = value;
	return item;
}

struct JSON_ITEM* json_long(long value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = LONG_T;
	item->value.t_long = value;
	return item;
}

struct JSON_ITEM* json_float(float value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = FLOAT_T;
	item->value.t_float = value;
	return item;
}

struct JSON_ITEM* json_double(double value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = DOUBLE_T;
	item->value.t_double = value;
	return item;
}

struct JSON_ITEM* json_bool(bool value){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = BOOL_T;
	item->value.t_bool = value;
	return item;
}

struct JSON_ITEM* json_null(){
	struct JSON_ITEM* item = malloc(sizeof(struct JSON_ITEM));
	item->type = NULL_T;
	return item;
}

struct JSON_ENTRY* json_entry(char* key, struct JSON_ITEM* value){
	struct JSON_ENTRY* entry = malloc(sizeof(struct JSON_ENTRY));
	entry->key = key;
	entry->value = value;
	return entry;
}

int json_dictionay_add_entry(struct JSON_ITEM* dictionary, struct JSON_ENTRY* entry){
	if(!dictionary||dictionary->type != OBJECT_T){
		return JSON_ERR_INVALID;
	}
	struct JSON_OBJECT* object;
	if(dictionary->value.t_object){
		object = dictionary->value.t_object;
		while (object->next){
			object = object->next;
		}
		object->next = malloc(sizeof(struct JSON_OBJECT));
		object = object->next;
	} else {
		object = malloc(sizeof(struct JSON_OBJECT));
		dictionary->value.t_object = object;
	}
	object->entry = entry;
	object->next = NULL;
	return JSON_SUCCESS;
}

int json_list_add_item(struct JSON_ITEM* list, struct JSON_ITEM* item){
	if(!list||list->type != LIST_T){
		return JSON_ERR_INVALID;
	}
	struct JSON_LIST* list_entry;
	if(list->value.t_list){
		list_entry = list->value.t_list;
		while(list_entry->next){
			list_entry = list_entry->next;
		}
		list_entry->next = malloc(sizeof(struct JSON_LIST));
		list_entry = list_entry->next;
	} else {
		list_entry = malloc(sizeof(struct JSON_LIST));
		list->value.t_list = list_entry;
	}
	list_entry->value = item;
	list_entry->next = NULL;
	return JSON_SUCCESS;
}

int json_print(FILE* output, struct JSON_ITEM* root){
	return json_print_item(output, root);
}

int json_print_item(FILE* output, struct JSON_ITEM* item){
	int result = JSON_SUCCESS;
	switch(item->type){
	case INT_T:
		fprintf(output, "%d", item->value.t_int);
		break;
	case LONG_T:
		fprintf(output, "%ld", item->value.t_long);
		break;
	case FLOAT_T:
		fprintf(output, "%f", item->value.t_float);
		break;
	case DOUBLE_T:
		fprintf(output, "%lf", item->value.t_double);
		break;
	case STRING_T:
		fprintf(output, "\"%s\"", json_escape(item->value.t_string));
		break;
	case BOOL_T:
		;
		char* str = (item->value.t_bool ? "true" : "false");
		fprintf(output, "%s", str);
		break;
	case OBJECT_T:
		break;
	case LIST_T:
		break;
	default:
		result = JSON_ERR_INVALID;
	}
	return result;
}

int json_print_entry(FILE* output, struct JSON_ENTRY* entry);

int json_print_list(FILE* output, struct JSON_LIST* list);

int json_print_object(FILE* output, struct JSON_OBJECT* object);

char* json_escape(char* input){
	size_t len = 0, i = 0;
	while(input[i] != '\0'){
		if(input[i]=='\n'||input[i]=='\b'||input[i]=='\r'||input[i]=='\a'||input[i]=='\''||input[i]=='\"'||input[i]=='\\'){
			len += 2;
		} else {
			len++;
		}
		i++;
	}
	char* output = reallocarray(NULL, len + 1, sizeof(char));
	i = 0;
	size_t j = 0;
	char c;
	while ((c = input[i++]) != '\0'){
		switch(c) {
		case '\n':
			output[j++] = '\\';
			output[j++] = 'n';
			break;
		case '\r':
			output[j++] = '\\';
			output[j++] = 'r';
			break;
		case '\b':
			output[j++] = '\\';
			output[j++] = 'b';
			break;
		case '\a':
			output[j++] = '\\';
			output[j++] = 'a';
			break;
		case '\t':
			output[j++] = '\\';
			output[j++] = 't';
			break;
		case '\'':
			output[j++] = '\\';
			output[j++] = '\'';
			break;
		case '\"':
			output[j++] = '\\';
			output[j++] = '\"';
			break;
		case '\\':
			output[j++] = '\\';
			output[j++] = '\\';
			break;
		default:
			output[j++] = c;
		}
	}
	output[j] = '\0';
	return output;
}
