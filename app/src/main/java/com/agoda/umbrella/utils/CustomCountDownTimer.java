package com.agoda.umbrella.utils;

import android.os.Handler;

/**
 * Create by YiFang on 2020/2/25 17:12
 * Email:Skaberen@outlook.com
 */
public class CustomCountDownTimer implements Runnable{

    private int time;
    private int countDownTime;
    private final ICountDownHandler countDownHandler;
    private final Handler handler;
    private boolean isRun;

    public CustomCountDownTimer(int time, ICountDownHandler countDownHandler){
        handler = new Handler();
        this.time = time;
        this.countDownTime = time;
        this.countDownHandler = countDownHandler;

    }

    //实现的具体逻辑
    @Override
    public void run() {
        if (isRun){
            if (countDownHandler != null){
                countDownHandler.onTicker(countDownTime);
            }
            if (countDownTime == 0){
                if (countDownHandler != null){
                    countDownHandler.onFinish();
                }
            }else {
                countDownTime = time--;
                handler.postDelayed(this,1);
            }
        }
    }

    //开启倒计时
    public void start(){
        isRun = true;
        handler.post(this);
    }

    //跳出循环 终止
    public void cancel(){
        isRun = false;
        handler.removeCallbacks(this);
    }

    //观察者回调接口(IOC数据回调)
    public interface ICountDownHandler{
        void onTicker(int time);//倒计时回调
        void onFinish();//完成时回调
    }

}
