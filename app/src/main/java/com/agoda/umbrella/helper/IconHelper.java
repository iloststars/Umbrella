package com.agoda.umbrella.helper;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

import com.agoda.umbrella.R;
import com.agoda.umbrella.StubApp;
import com.agoda.umbrella.utils.FileUtils;
import com.agoda.umbrella.utils.HttpUtils;

import net.lingala.zip4j.ZipFile;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

public class IconHelper {
    public static HashMap<Integer, Bitmap> iconCache = new HashMap<>();
    public static HashMap<Integer, String> name = new HashMap<>();
    public static float k = 1;
    public static float radius = 22.0f;

    //下载头像到本地（不加载到内存，用到时加载）
    //缓存id-name
    public static void init() {
        new File(FileUtils.getFilesPath(), "tttttt").mkdir();
        File file = new File(FileUtils.getFilesPath() + "tttttt/999.png");
        if (!file.exists()) {
            FileUtils.copyFilesFromRaw(StubApp.getContext(), R.raw.umbrella, "umbrella", FileUtils.getFilesPath());
            try {
                new ZipFile(FileUtils.getFilesPath() + "/umbrella").extractAll(FileUtils.getFilesPath() + "/tttttt");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        name.put(105, "廉颇");
        name.put(106, "小乔");
        name.put(107, "赵云");
        name.put(108, "墨子");
        name.put(109, "妲己");
        name.put(110, "嬴政");
        name.put(111, "孙尚香");
        name.put(112, "鲁班七号");
        name.put(113, "庄周");
        name.put(114, "刘禅");
        name.put(115, "高渐离");
        name.put(116, "阿珂");
        name.put(117, "钟无艳");
        name.put(118, "孙斌");
        name.put(119, "扁鹊");
        name.put(120, "白起");
        name.put(121, "芈月");
        name.put(123, "吕布");
        name.put(124, "周瑜");
        name.put(126, "夏侯惇");
        name.put(127, "甄姬");
        name.put(128, "曹操");
        name.put(129, "典韦");
        name.put(130, "宫本武藏");
        name.put(131, "李白");
        name.put(132, "马可波罗");
        name.put(133, "狄仁杰");
        name.put(134, "达摩");
        name.put(135, "项羽");
        name.put(136, "武则天");
        name.put(139, "老夫子");
        name.put(140, "关羽");
        name.put(141, "貂蝉");
        name.put(142, "安琪拉");
        name.put(144, "程咬金");
        name.put(146, "露娜");
        name.put(148, "姜子牙");
        name.put(149, "刘邦");
        name.put(150, "韩信");
        name.put(152, "王昭君");
        name.put(153, "兰陵王");
        name.put(154, "花木兰");
        name.put(155, "艾琳");
        name.put(156, "张良");
        name.put(157, "不知火舞");
        name.put(162, "娜可露露");
        name.put(163, "橘右京");
        name.put(166, "亚瑟");
        name.put(167, "孙悟空");
        name.put(168, "牛魔");
        name.put(169, "后裔");
        name.put(170, "刘备");
        name.put(171, "张飞");
        name.put(173, "李元芳");
        name.put(174, "虞姬");
        name.put(175, "钟馗");
        name.put(177, "成吉思汗");
        name.put(178, "杨戬");
        name.put(183, "雅典娜");
        name.put(184, "蔡文姬");
        name.put(186, "太乙真人");
        name.put(180, "哪吒");
        name.put(190, "诸葛亮");
        name.put(192, "黄忠");
        name.put(191, "大乔");
        name.put(187, "东皇太一");
        name.put(182, "干将莫邪");
        name.put(189, "鬼谷子");
        name.put(193, "铠");
        name.put(196, "百里守约");
        name.put(195, "百里玄策");
        name.put(194, "苏烈");
        name.put(198, "梦奇");
        name.put(179, "女娲");
        name.put(225, "傀儡");
        name.put(501, "明世隐");
        name.put(199, "公孙离");
        name.put(176, "杨玉环");
        name.put(502, "裴擒虎");
        name.put(197, "奕星");
        name.put(503, "狂铁");
        name.put(504, "米莱迪");
        name.put(125, "元歌");
        name.put(510, "孙策");
        name.put(137, "司马懿");
        name.put(509, "盾山");
        name.put(508, "伽罗");
        name.put(312, "沈梦溪");
        name.put(507, "李信");
        name.put(513, "上官婉儿");
        name.put(515, "嫦娥");
        name.put(511, "猪八戒");
        name.put(529, "盘古");
        name.put(505, "瑶");
        name.put(506, "云中君");
        name.put(522, "曜");
        name.put(518, "马超");
        name.put(523, "西施");
        name.put(525, "鲁班大师");
        name.put(524, "蒙犽");
        name.put(531, "镜");
        name.put(527, "蒙恬");
        name.put(528, "澜");
        name.put(533, "阿古朵");
        name.put(536, "夏洛特");
        name.put(537, "司空震");
        name.put(538, "云缨");
        name.put(999, "无");


        try {
            File dir = new File(FileUtils.getFilesPath() + "/tttttt");
            String[] iconss = dir.list();
            for (String icon : iconss) {
                try {
                    InputStream is = new FileInputStream(FileUtils.getFilesPath() + "/tttttt/" + icon);
                    Bitmap mBitmap = BitmapFactory.decodeStream(is);
                    int bw = mBitmap.getWidth();
                    int bh = mBitmap.getHeight();
                    float scale = Math.min(radius * 2 / bw, radius * 2 / bh);   //此处更改大小可以适配2k
                    mBitmap = scaleBitmap(mBitmap, scale);
                    iconCache.put(getID(icon), mBitmap);
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static Bitmap getGameIcon(int HeroID) {
        try {
            if (!iconCache.containsKey(HeroID)) {
                return iconCache.get(999);
            }
        } catch (NullPointerException e) {
            init();
            return getGameIcon(999);
        }
        return iconCache.get(HeroID);
    }

    public static Bitmap getSkillIcon(int SkillID) {
        try {
            if (!iconCache.containsKey(SkillID)) {
                return iconCache.get(4);
            }
        } catch (NullPointerException e) {
            init();
            return getGameIcon(4);
        }
        return iconCache.get(SkillID);
    }


    private static int getID(String string) {
        return Integer.parseInt(string.replace(".png", ""));
    }
    //提供外部调用获取HeroIcon （缓存）


    //提供外部调用获取HeroName
    public static String getHeroName(int id) {
        try {
            if (!name.containsKey(id)) {
                return name.get(999);
            }
        } catch (NullPointerException e) {
            return name.get(999);
        }
        return name.get(id);
    }


    private static Bitmap scaleBitmap(Bitmap origin, float scale) {
        if (origin == null) {
            return null;
        }
        int height = origin.getHeight();
        int width = origin.getWidth();
        Matrix matrix = new Matrix();
        matrix.postScale(scale, scale);// 使用后乘
        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (!origin.isRecycled()) {
            origin.recycle();
        }
        return newBM;
    }

    //下载头像
    private static void downHeadById(int id) {
        String url = "https://game.gtimg.cn/images/yxzj/img201606/heroimg/" + id + "/" + id + ".jpg";
        String des = FileUtils.getFilesPath() + "/" + id;
        FileUtils.streamToLocal(HttpUtils.syncGet(url), des);
    }
}
