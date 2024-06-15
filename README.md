# 简介
一个用c和javascript编写的mini音乐播放器
特点：无socket通信,通过c语言加载配置信息、音乐列表等导出给js

# 使用说明
## 音乐文件路径
config.json文件中配置了存放音乐文件的路径
music_path
默认是在当前目录的music文件夹下

## 启动
*windows*
双击arco_player.exe 弹出命令行窗口,自动拉起浏览器
*linux*
编码转换问题导致中文乱码,待解决 暂不支持

# 源码结构说明
## css目录
music_controller.css
页面基础样式、音乐播放器样式

music_list.css
左侧音乐列表样式

## image目录
存放图标

## music目录
默认的音乐文件存放目录

## js目录
index.js
控制播放逻辑

var.js
c语言程序启动时生成,保存配置信息、音乐列表等

## c
arcojson.c/h
一个非常轻量级的json库

main.c
解析配置文件、加载音乐列表的c源文件

build.sh
编译脚本
