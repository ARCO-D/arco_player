// 加载music_list music_path变量
document.write("<script type='text/javascript' src='js/var.js' charset='gb2312'></script>");
// 加载saved_weight_list
document.write("<script type='text/javascript' src='js/weight.js' charset='utf-8'></script>");


var play_or_pause = 0;           // 0为当前在暂停 1为在播放
var music;                       // 音乐节点
var music_index;
var previous_music_index = 0;    // 上一首音乐的索引
var next_music_index = 0;        // 下一首音乐的索引
var play_pause_bt;
var play_mode = "random";        // 播放模式 默认为随机
var music_progress;
var total_progress;
var current_progress;
var player_display_state = 1;
var music_list_display_state = 1;
var volume_display;
var music_weight_list = [];      // 全局音乐权重表
var weight_sum = 2726;           // 权重总和, 每次更改权重后更新, 避免在随机播放时进行运算

function init(){
    // 初始化音乐列表
    var music_list_area = document.getElementById("music_list_area");

    // 加载权重列表
    loadWeightList();

    for (var i = 0; i < music_list.length; i++) {
        // 创建音乐元素
        var node = document.createElement("button");
        node.innerText = music_list[i];
        node.id = "music_" + i;
        node.onclick = select_music;
        if (i % 2 == 0) node.className = "single-music-even";
        else node.className = "single-music-odd";

        // 在音乐块后创建减少权重的按钮
        var dw_node = document.createElement("button");
        dw_node.innerText = '-';
        dw_node.id = "dw_" + i; // decrease weight
        dw_node.onclick = decreaseMusicWeight;
        dw_node.className = 'decrease-weight-bt';

        // 显示权重的节点(直接现实loadWeightList加载出的权重
        var show_node = document.createElement("button");
        show_node.innerText = music_weight_list[i].weight;
        show_node.id = "sw_" + i; // show weight
        show_node.className = 'show-weight-bt';

        // 增加权重的按钮
        var iw_node = document.createElement("button");
        iw_node.innerText = '+';
        iw_node.id = "iw_" + i; // increads
        iw_node.onclick = increaseMusicWeight;
        iw_node.className = 'increase-weight-bt';
        
        music_list_area.append(node);
        music_list_area.append(dw_node);
        music_list_area.append(show_node);
        music_list_area.append(iw_node);
    }
    music_list_area.append(document.createElement("br"));

    // 挂载首个音乐节点
    music = document.getElementById("music");
    play_pause_bt = document.getElementById("playpauseBt");
    music.src = music_path + music_list[0];
    music.volume = 0.05; // 默认初始音量5%
    var str = music_list[0].substring(0, music_list[0].lastIndexOf('.')); // 显示去掉后缀的名称
    document.getElementById("currentMusic").innerText = str;
    music_index = 0;

    // 为播放结束绑定事件
    music.addEventListener('ended', musicPlayOver);

    // 绑定进度条节点
    total_progress = document.getElementById("totalProgress");
    music_progress = document.getElementById("currentProgress");
    music_progress.style.width = 0;
    music_progress_timer_id = setInterval("progressInc()", 100); //启动进度定时器

    // 挂载音量节点
    volume_display = document.getElementById("displayVolumeBt");
}


// 通过索引更换音乐(点击和下一首播放时调用)
function changeMusicByIndex(dst_music_index){
    if (play_or_pause == 1) playPauseMusic();
    // 通过索引更换音乐
    music.src = music_path + music_list[dst_music_index];
    music_index = dst_music_index;
    // 重新加载音乐
    music.load();
    // 切换音乐信息
    var str = music_list[dst_music_index].substring(0, music_list[dst_music_index].lastIndexOf('.'));
    document.getElementById("currentMusic").innerText = str;
    playPauseMusic();
}
// 点击列表切换播放音乐
function changeMusicByClick(dst_music_id){
    // 通过id找到对应的index
    var dst_music_index = dst_music_id.substring(6);
    // 调用[通过索引更换音译]方法
    changeMusicByIndex(dst_music_index);
}
// 点击音乐绑定的事件
function select_music(e){
    var music_node = e.target;
    changeMusicByClick(music_node.id);
}

// 暂停/播放音乐
function playPauseMusic(){
    if (play_or_pause == 0) {
        changeToPlayState();
        play_or_pause = 1;
    }
    else {
        changeToPauseState();
        play_or_pause = 0;
    }
}

function changeToPlayState(){
    music.play();
    play_pause_bt.style.backgroundImage = "url(image/pause_music.png)";
}

function changeToPauseState(){
    music.pause();
    play_pause_bt.style.backgroundImage = "url(image/play_music.png)";
}


function allocRandomValueByWeight(){
    var random_hit = Math.floor(Math.random() * weight_sum);
    console.log(random_hit);
    for (var i = 0; i < music_list.length; i++) {
        // 减去第i首歌的权重, 如果随机值命中了这个范围,那么返回这首歌的索引
        console.log("random_hit residue: " + random_hit + ". will sub " + music_weight_list[i].weight);
        random_hit -= music_weight_list[i].weight;
        if (random_hit <= 0) {
            console.log("random hit: " + music_list[i]);
            return i;
        }
    }
}

// 播放结束后根据模式默认执行的事件
function musicPlayOver(){
    var next_index;
    switch(play_mode){
        case "single_cycle":
            next_index = music_index;
            break;
        case "list_cycle":
            if (music_index == (music_list.length - 1)){
                next_index = 0; // 到结尾的话从头开始
            }
            else next_index = (parseInt(music_index) + 1);
            break;
        case "random":
            // 按照权重分配随机值
            next_index = allocRandomValueByWeight();
            break;
    }
    console.log("next_index:" + next_index);
    changeMusicByIndex(next_index);
}

// 播放下一首,单曲和列表循环都是+1
function nextMusic(){
    var next_index;
    switch(play_mode){
        case "random":
            // 按照权重分配随机值
            next_index = allocRandomValueByWeight();
            break;
        default:
            if (music_index == (music_list.length - 1)){
                next_index = 0; // 到结尾的话从头开始
            }
            else next_index = (parseInt(music_list) + 1);
    }
    changeMusicByIndex(next_index);
}

// 播放上一首,暂不支持随机模式回到上一首,会返回一个随机值
function previousMusic(){
    var next_index;
    switch(play_mode){
        case "random":
            // 按照权重分配随机值
            next_index = allocRandomValueByWeight();
            break;
        default:
            if (music_index == 0){
                next_index = (music_list.length - 1);
            }
            else next_index = (parseInt(music_list) - 1);
    }
    changeMusicByIndex(next_index);
}

// 切换播放模式
function switchPlayMode(){
    var play_mode_node = document.getElementById("playModeBt");
    if (play_mode == "single_cycle") {
        play_mode = "list_cycle";
        play_mode_node.style.backgroundImage = "url(image/list_cycle.png)";
    }
    else if (play_mode == "list_cycle") {
        play_mode = "random";
        play_mode_node.style.backgroundImage = "url(image/random.png)";
    }
    else if (play_mode == "random") {
        play_mode = "single_cycle";
        play_mode_node.style.backgroundImage = "url(image/single_cycle.png)";
    }
}

// 进度定时器
function progressInc(){
    // 播放时才自动更新
    if (play_or_pause == 1) {
        var unit_1s_length = (total_progress.offsetWidth / music.duration);
        music_progress.style.width = (music.currentTime * unit_1s_length) + "px";
    }
}

// 折叠播放器区域
function foldPlayer(){

}

// 折叠音乐列表区域
function foldMusicList(){

}

// 点击进度条更改播放位置
function changePlayPosition(event){
    var total = total_progress.offsetWidth;
    var percent = event.offsetX /total;
    music.currentTime = music.duration * percent;
    // 如果处在暂停状态,手动更新一次进度条
    if (play_or_pause == 0) music_progress.style.width = (total * percent) + "px";
}

// 改变音量
function changeVolume(p){
    if (p == 'inc') {
        if (music.volume <= 0.95) {
            music.volume += 0.05;
        }
        else music.volume = 1;
        volume_display.innerText = Math.round(music.volume * 100) + "%";
    }
    else {
        if (music.volume >= 0.05) {
            music.volume -= 0.05;
        }
        else music.volume = 0;
        volume_display.innerText = Math.round(music.volume * 100) + "%";
    }
}
