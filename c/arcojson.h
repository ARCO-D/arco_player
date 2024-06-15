#ifndef ARCOJSON_ARCOJSON_H
#define ARCOJSON_ARCOJSON_H

#define VERSION v0.1
enum json_type{
    json_type_empty,
    json_type_object,
    json_type_array,
    json_type_string,
    json_type_long
};

typedef struct arco_json{
    enum json_type type;
    int child_num;
    int seq;
    char* key;
    void* value;
    struct arco_json* parent;
    struct arco_json* next;
}arco_json;

/**
 * function: new_json_object
 * 功能:创建一个json对象
 */
arco_json* new_json_object();

/**
 * function: new_json_array
 * 功能:创建一个json数组
 */
arco_json* new_json_array();

/**
 * function:
 * 功能:创建一个json, value是字符串
 */
arco_json* new_json_string(char* value);

/**
 * function: new_json_long
 * 功能:创建一个json, value是long类型的字符串
 * 说明:为了代码简洁, 仅使用long来表示数值
 */
arco_json* new_json_long(long value);

/**
 * function: get_json_type
 * 功能:返回json的类型
 */
int get_json_type(arco_json* json);

/**
 * function: json_object_add
 * 功能:给json对象添加键值对
 */
int json_object_add(arco_json* json, char* key, arco_json* j_add);

/**
 * function: json_array_add
 * 功能:给json数组添加对象
 */
int json_array_add(arco_json* json, arco_json* j_add);

/**
 * function: json_to_string
 * 功能:json对象转json格式字符串
 */
char* json_to_string(arco_json* json);

/**
 * function: string_to_json
 * 功能:json格式字符串转json对象
 */
arco_json* string_to_json(char* str);

/**
 * function: get_string_from_object
 * 功能:通过key获取字符串类型的value
 */
char* get_string_from_object(arco_json* json, char* key);

/**
 * function: get_long_from_object
 * 功能:通过key获取数值类型的value
 */
long get_long_from_object(arco_json* json, char* key);

/**
 * function: get_object_from_object
 * 功能:通过key获取object类型的value
 */
arco_json* get_object_from_object(arco_json* json, char* key);

/**
 * function: get_object_from_array
 * 功能:获取json array的第idx个对象(从0开始
 */
arco_json* get_object_from_array(arco_json* json, int idx);

#endif //ARCOJSON_ARCOJSON_H