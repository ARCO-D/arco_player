//
// Created by arco on 2023/8/19.
//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arcojson.h"

int g_json_char_num = 0;
char* g_json_str = NULL;

int init_new_json(arco_json* json, int json_type)
{
    json->type = json_type;
    json->child_num = 0;
    json->seq = 0;
    json->key = NULL;
    json->value = NULL;
    json->next = NULL;
}

arco_json* new_json_object()
{
    arco_json* json = malloc(sizeof(arco_json));
    init_new_json(json, json_type_object);
    return json;
}

arco_json* new_json_array()
{
    arco_json* json = malloc(sizeof(arco_json));
    init_new_json(json, json_type_array);
    return json;
}

arco_json* new_json_string(char* value)
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    init_new_json(json, json_type_string);
    json->value = (char*) malloc(strlen(value) + 1);
    memcpy(json->value, value, strlen(value) + 1);
    return json;
}

arco_json* new_json_long(long value)
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    init_new_json(json, json_type_long);
    json->value = (long*) malloc(sizeof(long));
    *(long*) json->value = value;
    return json;
}

arco_json* new_json_empty()
{
    // 分配内存
    arco_json* json = malloc(sizeof(arco_json));
    init_new_json(json, json_type_empty);
    return json;
}

int get_json_type(arco_json* json)
{
    if (json != NULL) return json->type;
    else return -1;
}

int json_object_add(arco_json* json, char* key, arco_json* j_add)
{
    if (json->type != json_type_object) {
        printf("json type isn't object, can't add kv\n");
        return -1;
    }
    int i;
    // if cur json has none value
    if (json->value == NULL) {
        json->value = j_add;
        j_add->parent = json;
        j_add->key = malloc(strlen(key) + 1);
        memcpy(j_add->key, key, strlen(key) + 1);
        json->child_num++;
    }
    else {
        arco_json* arco = json->value;
        for (i = 0; i < json->child_num - 1; i++) {
            arco = arco->next;
        }
        j_add->key = malloc(strlen(key) + 1);
        memcpy(j_add->key, key, strlen(key) + 1);
        arco->next = j_add;
        j_add->parent = arco->parent;
        json->child_num++;
    }
    return 0;
}

int json_array_add(arco_json* json, arco_json* j_add)
{
    if (json->type != json_type_array) {
        printf("json type isn't array, can't add object\n");
        return -1;
    }
    int i;
    // if cur json has none value
    if (json->value == NULL) {
        json->value = j_add;
        json->child_num++;
    }
    else {
        arco_json* arco = json->value;
        for (i = 0; i < json->child_num - 1; i++) {
            arco = arco->next;
        }
        arco->next = j_add;
        j_add->parent = arco->parent;
        json->child_num++;
    }
    return 0;
}

typedef void (*deal_callback) (char*, ...);

void json_depth_expand(arco_json* json, int depth, deal_callback callback)
{
    if (get_json_type(json) == json_type_array) {
        if (json->key != NULL && depth > 0)
            callback("\"%s\":", json->key);
        callback("[");
        if (json->value != NULL)
            json_depth_expand(json->value, depth + 1, callback);
    }
    if (get_json_type(json) == json_type_object) {
        if (json->key != NULL && depth > 0)
            callback("\"%s\":", json->key);
        callback("{");
        if (json->value != NULL)
            json_depth_expand(json->value, depth + 1, callback);
    }
    if (json->type == json_type_string) {
        callback("\"%s\":", json->key);
        callback("\"%s\"", (char*) json->value);
        if (json->next != NULL) callback(",");
    }
    if (json->type == json_type_long) {
        callback("\"%s\":", json->key);
        callback("%d", *(long*) json->value);
        if (json->next != NULL) callback(",");
    }

    if (get_json_type(json) == json_type_array) {
        callback("]");
        if (json->next != NULL && depth > 0) callback(",");
    }
    if (get_json_type(json) == json_type_object) {
        callback("}");
        if (json->next != NULL && depth > 0) callback(",");
    }

    // 横向搜索
    if (json->next != NULL && depth > 0) {
        json_depth_expand(json->next, depth, callback);
    }
}

void calculate_callback(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[64];
    vsprintf(str, fmt, args);
    g_json_char_num += (int) strlen(str);
    va_end(args);
}

void tostring_callback(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[64];
    vsprintf(str, fmt, args);
    strcat(g_json_str, str);
    va_end(args);
}

int calculate_json_str_length(arco_json* json)
{
    g_json_char_num = 0;
    json_depth_expand(json, 0, calculate_callback);
    return g_json_char_num;
}

char* json_to_string(arco_json* json)
{
    int size = calculate_json_str_length(json);
    g_json_str = malloc(size + 1);
    memset(g_json_str, '\0', size + 1);
    json_depth_expand(json, 0, tostring_callback);
    char* json_str = malloc(strlen(g_json_str) + 1);
    memcpy(json_str, g_json_str, strlen(g_json_str) + 1);
    free(g_json_str);
    g_json_str = NULL;
    return json_str;
}

char* str_get_here_to_there(char* str, int position, char c)
{
    int i, size = 1;
    char* dst = NULL;
    for (i = position; i < strlen(str); i++) {
        if (str[i] != c) size++;
        else break;
    }
    dst = malloc(sizeof(char) * size);
    for (i = position; i < strlen(str); i++) {
        if (str[i] != c) dst[i - position] = str[i];
        else {
            dst[i - position] = '\0';
            return dst;
        }
    }
    return NULL;
}

// 返回值是解析的数值的字符串长度(需要跳过的长度
int parse_num_value(char* str, void* value)
{
    int i, start = 0, val_len = 0;
    long rate = 1;
    long* num_val = malloc(sizeof(long));
    char arr[16];
    memset(arr, '\0', sizeof(arr));

    if (str[0] == '-') start = 1;
    val_len += start;

    for (i = start; i < strlen(str) && i < sizeof(arr) - 1; i++) {
        if (str[i] < '0' || str[i] > '9') break;
        arr[i - start] = str[i];
        val_len++;
    }
    for (i = strlen(arr) - 1; i >= 0; i--) {
        *num_val += (arr[i] - '0') * rate;
        rate *= 10;
    }
    if (start) *num_val *= -1;

    *(long*) value = *num_val;

    return val_len;
}

arco_json* string_to_json(char* str)
{
    int i, str_len = (int) strlen(str), need_new = 0;
    int yh_flag = 0, value_flag = 0;
    arco_json* json = new_json_empty();

    arco_json* p_json = json;
    for (i = 0; i < str_len; i++) {
        /**
         * 紧随{或[后的第一个json还没有new出来
         */
        if (need_new) {
            arco_json* j_tmp = new_json_empty();
            p_json->value = j_tmp;
            j_tmp->parent = p_json;
            p_json = p_json->value;
            need_new = 0;
        }
        /**
         * 截取第1-2个引号之间的值作为key, 如果有第3-4个引号那就作为value
         */
        if (str[i] == '"') {
            yh_flag++;
            if (yh_flag == 1) {
                p_json->key = str_get_here_to_there(str, i + 1, '"');
            }
            else if (yh_flag == 2) {

            }
            else if (yh_flag == 3) {
                p_json->value = str_get_here_to_there(str, i + 1, '"');
                p_json->type = json_type_string;
            }
            else if (yh_flag == 4) {
                yh_flag = 0;
            }
        }

        /**
         * 处理冒号后紧随的第一个
         */
        if (value_flag) {
            if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-') {
                p_json->type = json_type_long;
                p_json->value = (long*)malloc(sizeof(long));
                i += parse_num_value(&str[i], p_json->value);
                yh_flag = 0;
            }
            value_flag = 0;
        }

        if (str[i] == ':') {
            value_flag = 1;
        }

        if (str[i] == '{') {
            yh_flag = 0;
            need_new = 1;
            p_json->type = json_type_object;
        }

        if (str[i] == '[') {
            yh_flag = 0;
            need_new = 1;
            p_json->type = json_type_array;
        }

        if (str[i] == ',') {
            // 创建一个空json, 挂到当前json的next
            arco_json* j_tmp = new_json_empty();
            j_tmp->seq = p_json->seq + 1;
            p_json->next = j_tmp;
            // 拷贝上级json
            j_tmp->parent = p_json->parent;
            // 如果是第1个确保当前json的上级的value指向正确
            if (p_json->seq == 0) {
                arco_json* q_json = p_json->parent;
                q_json->value = p_json;
            }
            // 移动当前json
            p_json = p_json->next;
        }

        if (str[i] == '}' || str[i] == ']') {
            arco_json* j_tmp = p_json->parent;
            p_json = j_tmp;
        }
    }
    return json;
}


char* get_string_from_object(arco_json* json, char* key)
{
    if (json == NULL) return NULL;
    if (json->type != json_type_object) return NULL;
    if (json->value == NULL) return NULL;

    arco_json* p_json = json->value;
    while (p_json != NULL) {
        if (p_json->type == json_type_string) {
            if (strcmp((char*) p_json->key, key) == 0) {
                size_t length = strlen((char*) p_json->value);
                char* res = malloc(sizeof(length + 1));
                memcpy(res, p_json->value, length + 1);
                return res;
            }
        }
        p_json = p_json->next;
    }
    return NULL;
}

long get_long_from_object(arco_json* json, char* key)
{
    if (json == NULL) return -1;
    if (json->type != json_type_object) return -1;
    if (json->value == NULL) return -1;

    arco_json* p_json = json->value;
    while (p_json != NULL) {
        if (p_json->type == json_type_long) {
            if (strcmp((char*) p_json->key, key) == 0) {
                long res = *(long*) p_json->value;
                return res;
            }
        }
        p_json = p_json->next;
    }
    return -1;
}

arco_json* get_object_from_object(arco_json* json, char* key)
{
    if (json == NULL) return NULL;
    if (json->type != json_type_object) return NULL;
    if (json->value == NULL) return NULL;

    arco_json* p_json = json->value;
    while (p_json != NULL) {
        if (p_json->type == json_type_object) {
            if (strcmp((char*) p_json->key, key) == 0) {
                arco_json* res = malloc(sizeof(arco_json));
                memcpy(res, p_json, sizeof(arco_json));
                return res;
            }
        }
        p_json = p_json->next;
    }
    return NULL;
}

arco_json* get_object_from_array(arco_json* json, int idx)
{
    if (json == NULL) return NULL;
    if (json->type != json_type_array) return NULL;
    if (json->value == NULL) return NULL;

    int i = 0;
    arco_json* p_json = json->value;
    while (p_json != NULL) {
        if (p_json->type == json_type_object) {
            if (i == idx) {
                arco_json* res = malloc(sizeof(arco_json));
                memcpy(res, p_json, sizeof(arco_json));
                return res;
            }
        }
        p_json = p_json->next;
        i++;
    }
    return NULL;
}
