#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "arcojson.h"

#define CONFIG_JSON "config.json"
#define JS_VAR_FILE "js/var.js"
#ifdef _WIN32
    #include <windows.h>
    #define WIN_PAUSE system("pause");
#else
    #define WIN_PAUSE
#endif

/**
 * config table
 */
typedef struct CONFIG {
    char config_name[32];
    char value[128];
} CONFIG;

CONFIG g_config_table[] = {
    {"music_path", "music/"}
};


/**
 * A simple list for music files
 */
typedef struct file_list {
    char name[128];
    struct file_list* next;
} FILE_LIST;

FILE_LIST* init_list()
{
    FILE_LIST* head = (FILE_LIST*) malloc(sizeof(FILE_LIST));
    head->next = NULL;
    return head;
}
FILE_LIST* createNode(char* file_name)
{
    FILE_LIST* node = (FILE_LIST*) malloc(sizeof(FILE_LIST));
    strcpy_s(node->name, sizeof(node->name) - 1, file_name);
    node->next = NULL;
    return node;
}

void addNode(FILE_LIST* list, char* file_name)
{
    if (list->next == NULL) {
        list->next = createNode(file_name);
        return ;
    }
    FILE_LIST* p = list->next;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = createNode(file_name);
}

void print_array(FILE_LIST* list)
{
    FILE_LIST* p = list->next;
    printf("[");
    while (p != NULL) {
        printf("\"%s\"", p->name);
        p = p->next;
        if (p != NULL) printf(",");
    }
    printf("]\n");
}

/**
 * welcome info
 */
void arco_welcome()
{
    printf("[INFO] welcome to arco music cnm player~\n");
}


/**
 * 解析config.json配置文件,填充到全局表
 */
char* read_file(const char* file_path)
{
    int file_size, result;
    char* buffer;
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("[ERRO] open config.json failed\n");
        return NULL;
    }
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    // 分配内存以存储整个文件
    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        printf("[ERRO] malloc %d byte mem failed\n", file_size + 1);
        fclose(file);
        return NULL;
    }
    // 读取整个文件到 buffer
    result = fread(buffer, 1, file_size, file);
    // if (result != file_size) {
    //     // 这个检查貌似没用而且会误判
    //     printf("error reading file end. result=%d file_size=%d\n", result, file_size);
    // }
    // 确保字符串是 null 结尾的
    buffer[file_size] = '\0';
    // 关闭文件
    fclose(file);
    // 打印读取的内容
    printf("[INFO] %s\n", buffer);
    // 返回这块buffer 
    return buffer;
}
int parse_config_json()
{
    char* config_str = read_file(CONFIG_JSON);
    arco_json* j_config = string_to_json(config_str);

    printf("[INFO] config.json:%s\n", json_to_string(j_config));

    // 遍历配置全局表
    int i;
    for (i = 0; i < (sizeof(g_config_table) / sizeof(CONFIG)); i++) {
        printf("[INFO] get config %s\n", g_config_table[i].config_name);
        char* temp_config = get_string_from_object(j_config, g_config_table[i].config_name);
        printf("[INFO] config %s value %s\n", g_config_table[i].config_name, temp_config);
        // 如果没读取到对应的json配置就跳过,直接使用全局表配置的默认value
        if (temp_config == NULL) continue;
        // 如果读取到了就覆盖value
        else {
            strcpy_s(g_config_table[i].value, sizeof(g_config_table[i].value), temp_config);
            // temp_config未释放,有内存问题
        }
    }
    
    printf("[INFO] load config over\n");

    return 0;
}


/**
 * API for register js variables
 */
int clean_var_js(void)
{
    printf("\n[INFO] start clean var js\n");
    FILE* fp = fopen(JS_VAR_FILE, "w");
    if (fp == NULL) {
        printf("[ERRO] clean %s failed\n", JS_VAR_FILE);
    }
    printf("[INFO] clean %s over\n", JS_VAR_FILE);
    fclose(fp);
    return 0;
}

// music_path写入js文件
void add_var_music_path(char* music_path)
{
    // parse_config_json不做详细检查,这里检查music_path的路径最后一位是否为\\或/
    if (music_path[strlen(music_path) - 1] != '\\' || music_path[strlen(music_path) - 1] != '/') {
        music_path[strlen(music_path)] = '/';
        music_path[strlen(music_path) + 1] = 0;
        printf("[INFO] consider last char is not path, set '/' at last\n");
    }

    printf("\n[INFO] start init music path\n");
    // 追加music_path变量
    FILE* fp = fopen(JS_VAR_FILE, "a");
    if (fp == NULL) {
        printf("[ERRO] open %s failed\n", JS_VAR_FILE);
    }
    printf("[INFO] open %s success\n", JS_VAR_FILE);
    fprintf(fp, "var music_path = \"%s\";\n", music_path);
    fclose(fp);
    
    printf("\n[INFO] init music path=%s\n", music_path);
}

// music_list写入js文件
void write_list_to_jsfile(FILE_LIST* list)
{
    // 逐个追加
    FILE* fp = fopen(JS_VAR_FILE, "a");
    FILE_LIST* p = list->next;
    fprintf(fp, "var music_list = [");
    while (p != NULL) {
        fprintf(fp, "\"%s\"", p->name);
        p = p->next;
        if (p != NULL) fprintf(fp, ",");
    }
    fprintf(fp, "];\n");
    fclose(fp);
}
// 注册js音乐列表list
int add_var_music_list(const char* music_path)
{
    int i = 1;
    // char str_gbk[256] = "";
    // char str_utf8[256] = "";
    // char str_utf16[256] = "";
    // 加载当前路径music目录下的音乐列表
    FILE_LIST* list = init_list();
    DIR* dir = opendir(music_path);
    if (dir == NULL) {
        printf("[ERRO] open %s dir failed\n", music_path);
        WIN_PAUSE
        return -1;
    }
    struct dirent* ptr;
    while((ptr = readdir(dir)) != NULL) {
        // 文件名超过259字节会直接导致这里结束
        if (ptr->d_name[0] == '.') continue;
        // WideCharToMultiByte(CP_UTF8, 0, str_utf16, -1, str_utf8, sizeof(str_utf8), NULL, NULL);
        // memset(str_gbk, 0, sizeof(str_gbk));
        // memset(str_utf8, 0, sizeof(str_utf8));
        // memset(str_utf16, 0, sizeof(str_utf16));
        printf("[INFO] register %d music: %s\n", i++, ptr->d_name);
        addNode(list, ptr->d_name);
    }
    
    closedir(dir);
    print_array(list);
    write_list_to_jsfile(list);
    printf("\n[INFO] init music list over\n");
    return 0;
}


// 启动浏览器
void start_web_browser()
{
    // 获取当前路径
    char current_dir[512];
    if (_getcwd(current_dir, sizeof(current_dir))) {
        printf("\n[INFO] current dir is %s\n", current_dir);
    }
    else {
        printf("[ERRO] get current dir failed\n");
        printf("[ERRO] can't start\n");
        return ;
    }
    // 启动浏览器
    char cmd[512];
    sprintf(cmd, "start %s\\index.html", current_dir);
    system(cmd);
}


int main()
{
    // ARCO welcome
    arco_welcome();
    
    // 清理配置文件
    clean_var_js();

    // 解析配置json
    parse_config_json();

    // 注册music_path变量 暂时先固定位置
    add_var_music_path(g_config_table[0].value);

    // 注册music_list变量
    add_var_music_list(g_config_table[0].value);

    // 启动浏览器
    start_web_browser();

    WIN_PAUSE
    return 0;
}