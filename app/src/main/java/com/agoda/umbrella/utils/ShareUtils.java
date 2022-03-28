package com.agoda.umbrella.utils;

import android.util.Log;

import com.agoda.umbrella.data.DataParser;
import com.agoda.umbrella.helper.ConfigHelper;
import com.agoda.umbrella.helper.IconHelper;

import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

public class ShareUtils extends Thread {
    public static boolean sendflag = true;
    public static int t = 1;

    public static HttpURLConnection initHttp() {
        try {
            URL url = new URL("http://pay.asfaka.net:8080/set");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setReadTimeout(20);
            conn.setConnectTimeout(5000);
            conn.setRequestProperty("Accept", "text/plain, text/html");//指定客户端能够接收的内容类型
            conn.setRequestProperty("Connection", "keep-alive"); //http1.1
            conn.setDoOutput(true);
            conn.setDoInput(true);

            return conn;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static void asynSend() {
        new Thread(() -> {
            while (sendflag) {
                t = t + 1;
                //if(t==10000)t=0;
                sendData(ConfigHelper.sharedId);
                try {
                    sleep(50);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    public static void sendData(final String sharedId) {
        new Thread(() -> {
            try {
                Log.e("mylog", "sendData: "+sharedId );
                HttpURLConnection conn = initHttp();
                StringBuilder sd = new StringBuilder();
                for (DataParser.Hero hero : DataParser.heroArray) {
//                    if (hero.Camp != DataProvider.SgameOutput.camp)
                        sd.append(hero.id).append(",")
                                .append(2).append(",").append((int) ((hero.mapX) / (0.00332) / IconHelper.k)).
                                append(",").append((int) ((hero.mapY ) / (-0.00332) / IconHelper.k)).append(",").append(hero.hp).append(";");
                }
//                for (DataParser.Hero hero : DataProvider.SgameOutput.hero) {
//                    if (hero.Camp == DataProvider.SgameOutput.camp)
//                        sd.append(hero.id).append(",")
//                                .append(hero.Camp).append(",").append((int) ((hero.mapX - 150 * IconHelper.k) / (0.00332) / IconHelper.k)).
//                                append(",").append((int) ((hero.mapY - 150 * IconHelper.k) / (-0.00332) / IconHelper.k)).append(",").append(hero.hp).append(";");
//                }
                sd.append("|").append(DataParser.buffArray[0].cd).append(";").append(DataParser.buffArray[3].cd).append(";").append(DataParser.buffArray[6].cd).append(";").append(DataParser.buffArray[9].cd);

                String data2 = URLEncoder.encode(sd.toString(), "UTF-8");
                String udid2 = URLEncoder.encode(sharedId, "UTF-8");
                String message = "id=" + udid2 + "&data=" + data2 + ";!" + t;
                assert conn != null;
                OutputStream os = conn.getOutputStream();
                os.write(message.getBytes());
                conn.getResponseCode();
                conn.disconnect();
                os.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();
    }
}


