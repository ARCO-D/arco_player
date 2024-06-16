// 这是保存当前音乐权重的配置文件,放置在项目的js文件夹下即可加载保存的配置
var saved_weight_list = [
  {
    "name": "Aki阿杰 - 大风起兮.mp3",
    "weight": 1
  },
  {
    "name": "Aki阿杰 - 闲云志.mp3",
    "weight": 1
  },
  {
    "name": "Amaranthe - Unreal.mp3",
    "weight": 1
  },
  {
    "name": "COP _ AKINO (川満愛希信) _ 初音未来 (初音ミク) - 世末歌者（日语ver_）.mp3",
    "weight": 1
  },
  {
    "name": "FORMOSA - 前世今生.mp3",
    "weight": 1
  },
  {
    "name": "JMJ _ 闻人听書_ - 醉我.mp3",
    "weight": 1
  },
  {
    "name": "Katy Perry _ Juicy J - Dark Horse.mp3",
    "weight": 1
  },
  {
    "name": "Maksim _ Nik?a Brato? _ Remi Kazinoti _ Craig Pruess _ Orchestra - Croatian Rhapsody (Arr_ Bratos & Kazinoti).mp3",
    "weight": 1
  },
  {
    "name": "Maroon 5 - This Summer's Gonna Hurt Like A Motherf____r.mp3",
    "weight": 1
  },
  {
    "name": "Maroon 5 _ Adam Levine - Lost Stars.mp3",
    "weight": 1
  },
  {
    "name": "Maroon 5 _ Wiz Khalifa - Payphone.mp3",
    "weight": 1
  },
  {
    "name": "Martin Garrix - Animals.mp3",
    "weight": 1
  },
  {
    "name": "nomico (のみこ) - Bad Apple!!.mp3",
    "weight": 1
  },
  {
    "name": "RADWIMPS (ラッドウィンプス) - なんでもないや (没什么大不了) (Movie ver_).mp3",
    "weight": 1
  },
  {
    "name": "Sarah Brightman - Scarborough Fair.mp3",
    "weight": 1
  },
  {
    "name": "SILENT SIREN (サイレント?サイレン) - secret base ~君がくれたもの~ (secret base ~你给我的所有~).mp3",
    "weight": 1
  },
  {
    "name": "Studio Sunset - Yesterday Once More.mp3",
    "weight": 1
  },
  {
    "name": "Super Junior-M - Break Down.mp3",
    "weight": 1
  },
  {
    "name": "SuperARCO.mp3",
    "weight": 1
  },
  {
    "name": "sweet ARMS (スウィートアームズ) - 恋色折紙.mp3",
    "weight": 1
  },
  {
    "name": "S_H_E - 不想长大 (Live).mp3",
    "weight": 1
  },
  {
    "name": "T-ara (???) - DAY BY DAY (Remix Version).mp3",
    "weight": 1
  },
  {
    "name": "Taylor Swift - Last Christmas.mp3",
    "weight": 1
  },
  {
    "name": "Taylor Swift - Shake It Off.mp3",
    "weight": 1
  },
  {
    "name": "Taylor Swift - Starlight.mp3",
    "weight": 1
  },
  {
    "name": "Taylor Swift _ The Civil Wars - Safe & Sound (from The Hunger Games Soundtrack).mp3",
    "weight": 1
  },
  {
    "name": "Various Artists - 第九交响曲“自新大陆”第三乐章_德沃夏克.mp3",
    "weight": 1
  },
  {
    "name": "Various Artists - 第九交响曲“自新大陆”第四乐章_德沃夏克.mp3",
    "weight": 1
  },
  {
    "name": "Vitas - Звезда.mp3",
    "weight": 1
  },
  {
    "name": "V_K克 _ 双笙 (陈元汐) - 纯白.mp3",
    "weight": 1
  },
  {
    "name": "Wizard See - See You Again (Clone Mix).mp3",
    "weight": 1
  },
  {
    "name": "Wonder Girls (????) - Nobody (korean Ver_).mp3",
    "weight": 1
  },
  {
    "name": "Zero Blank零度空白 _ 闻人听書_ - 月出.mp3",
    "weight": 1
  },
  {
    "name": "不才 - 参商.mp3",
    "weight": 1
  },
  {
    "name": "中岛美嘉 (なかしま みか) - 桜色舞うころ (樱花纷飞时) (Live).mp3",
    "weight": 1
  },
  {
    "name": "云の泣 - 青玉案.mp3",
    "weight": 1
  },
  {
    "name": "五音Jw _ 银临 - 生死劫.mp3",
    "weight": 1
  },
  {
    "name": "以冬 - 我的一个道姑朋友.mp3",
    "weight": 1
  },
  {
    "name": "任宇翔 - 悟空 (王者荣耀版).mp3",
    "weight": 1
  },
  {
    "name": "伊格赛听 _ 平生不晚 - 御龙定乾坤.mp3",
    "weight": 1
  },
  {
    "name": "倪浩毅 _ 闻人听書_ - 冥.mp3",
    "weight": 1
  },
  {
    "name": "儿歌 - 我们是共产主义接班人.mp3",
    "weight": 1
  },
  {
    "name": "兔裹煎蛋卷 _ 子弥 - 闻说笔端有风月.mp3",
    "weight": 1
  },
  {
    "name": "冯提莫 - 你就不要想起我.mp3",
    "weight": 1
  },
  {
    "name": "刘珂矣 - 一袖云.mp3",
    "weight": 1
  },
  {
    "name": "刘珂矣 - 半壶纱.mp3",
    "weight": 1
  },
  {
    "name": "刘珂矣 - 墨痕.mp3",
    "weight": 1
  },
  {
    "name": "刘珂矣 - 忘尘谷.mp3",
    "weight": 1
  },
  {
    "name": "刘珂矣 - 风筝误.mp3",
    "weight": 1
  },
  {
    "name": "初音未来 (初音ミク) - 千本桜 (千本樱).mp3",
    "weight": 1
  },
  {
    "name": "初音未来 (初音ミク) - 李白 (日文版).mp3",
    "weight": 1
  }
];
