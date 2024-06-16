// 查找音乐
function searchMusic(){
    const mla = document.querySelector(`#music_list_area`);
    const music_node_list = mla.querySelectorAll('button');

    search_str = document.getElementById("search_ipt").value;
    console.log("search:" + search_str);

    if (search_str.length == 0) return;

    // 清除上次的搜索结果
    music_node_list.forEach(music_node_list => {
        music_node_list.style.color = 'black';
    });

    music_node_list.forEach(music_node_list => {
        if (music_node_list.textContent.includes(search_str))
            music_node_list.style.color = 'coral';
        else music_node_list.style.color = 'black';
    });

}

/**
 * 权重处理:启动
 * 1 arco_player.exe会创建一个Array类型的 music_list
 * 2 检查js路径下有无weight.js文件, 其中的saved_weight_list变量保存有音乐的权值
 *
 * 权重处理:变更
 * 1 增加/减少权值时会变更weight_sum
 * 2 变更会同步更新到show_node和music_weight_list
 * 3 随机播放时的下一首 会按照weight_sum进行轮盘加权运算
 */

class MusicInfo{
    constructor(name, weight){
        this.name = name;
        this.weight = weight;
    }
}

// 结合weight.js和music_list创建内存列表
function loadWeightList(){
    var match_num = 0;
    // 用当前存在的音乐文件列表生成一个未初始化的music_weight_list
    for (var i = 0; i < music_list.length; i++) {
        music_weight_list.push(new MusicInfo(music_list[i], 1));
    }

    // 如果weight.js存在,用saved_weight_list中保存的权重进行替换
    if (!(typeof saved_weight_list === 'undefined')) {
        for (var i = 0; i < music_list.length; i++) {
            for (var j = 0; j < saved_weight_list.length; j++) {
                if (music_list[i] == saved_weight_list[j].name) {
                    music_weight_list[i].weight = saved_weight_list[j].weight;
                    match_num++;
                    break;
                }
            }
            // 打印出未匹配到的歌曲
            if (j == saved_weight_list.length)
                console.log("unmatch music: " + music_weight_list[i].name);
            // 音乐列表没有而weight.js中有的文件名不会被打印出unmatch, 但不会影响权值计算
        }
    }


    // 计算权重总和
    weight_sum = 0;
    for (var i = 0; i < music_list.length; i++) {
        weight_sum += music_weight_list[i].weight;       
    }

    console.log("weight_sum=" + weight_sum);
}

// 减少权重
function decreaseMusicWeight(e){
    var w_x = e.target;
    // 通过id找到对应的index
    var id_str = w_x.id.substring(3);
    var id_int = parseInt(id_str) + 1;
    // 更改sw_x中的数据显示
    let weight_str = document.getElementById('sw_' + id_str).innerText;
    let weight_int = parseInt(weight_str);
    if (weight_int <= 0) return; // 为0不许再减
    weight_int -= 1;
    document.getElementById('sw_' + id_str).innerText = weight_int;
    // 更改music_weight_list
    music_weight_list[id_int - 1].weight = weight_int;
    console.log(music_weight_list[id_int - 1].name + "set weight =>" + music_weight_list[id_int - 1].weight);
    // 更改权重总和
    weight_sum -= 1;
    console.log("weight_sum=" + weight_sum);
}
// 增加权重
function increaseMusicWeight(e){
    var w_x = e.target;
    // 通过id找到对应的index
    var id_str = w_x.id.substring(3);
    var id_int = parseInt(id_str) + 1;
    // 更改sw_x中的数据显示
    let weight_str = document.getElementById('sw_' + id_str).innerText;
    let weight_int = parseInt(weight_str);
    weight_int += 1;
    document.getElementById('sw_' + id_str).innerText = weight_int;
    // 更改music_weight_list
    music_weight_list[id_int - 1].weight = weight_int;
    console.log(music_weight_list[id_int - 1].name + "set weight =>" + music_weight_list[id_int - 1].weight);
    // 更改权重总和
    weight_sum += 1;
    console.log("weight_sum=" + weight_sum);
}


// // 点击音乐绑定的事件
function select_music(e){
    var music_node = e.target;
    changeMusicByClick(music_node.id);
}


// 下载权重配置文件(这里是API的例子,并未实现)
function saveWeightFile(){
    // 1. 转换为字符串，包含变量声明
    var arrayAsString = `// 这是保存当前音乐权重的配置文件,放置在项目的js文件夹下即可加载此权重配置(文件名必须为weight.js)\nvar saved_weight_list = ${JSON.stringify(music_weight_list, null, 2)};\n`;

    // 2. 创建Blob对象
    var blob = new Blob([arrayAsString], { type: 'txt' });

    // 3. 使用URL.createObjectURL()创建一个可以下载的URL
    var downloadURL = URL.createObjectURL(blob);

    // 4. 创建一个锚点元素并设置其href属性
    var link = document.createElement("a");
    link.href = downloadURL;
    link.download = "weight.js"; // 指定下载文件的名称

    // 5. 触发下载
    document.body.appendChild(link);
    link.click();

    // 6. 清理资源，避免内存泄漏
    document.body.removeChild(link);
    URL.revokeObjectURL(downloadURL);
}
