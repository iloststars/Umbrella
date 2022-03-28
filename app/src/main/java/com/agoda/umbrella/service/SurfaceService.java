package com.agoda.umbrella.service;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.TextureView;
import android.view.WindowManager;

import androidx.annotation.Nullable;

import com.agoda.umbrella.R;
import com.agoda.umbrella.helper.ConfigHelper;
import com.agoda.umbrella.view.DrawView;


public class SurfaceService extends Service {

    private WindowManager windowManager;
    private WindowManager.LayoutParams mWmParam;
    private TextureView surfaceView;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        initSurfaceWindow();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            manager.createNotificationChannel(new NotificationChannel("GURA","GURA", NotificationManager.IMPORTANCE_HIGH));
            Notification.Builder builder = new Notification.Builder(this, "GURA");
            builder.setSmallIcon(R.drawable.logo);
            builder.setContentTitle("Umbrella");
            builder.setContentText("Umbrella运行中");
            builder.setWhen(System.currentTimeMillis()); // 设置该通知发生的时间
            startForeground(2233, builder.build());
        }
        showFloatingWindow();
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        try{
            if (surfaceView!=null) {
                windowManager.removeView(surfaceView);
            }
        }catch (Exception e){

        }
        stopForeground(true);
        this.stopSelf();
        super.onDestroy();
    }

    private int computeFlags(int curFlags) {
        boolean mTouchable = false;
        if (!mTouchable) {
            curFlags |= WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
        } else {
            curFlags &= ~WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
        }

        boolean mFocusable = false;
        if (!mFocusable) {
            curFlags |= WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        } else {
            curFlags &= ~WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }

        boolean mTouchModal = true;
        if (!mTouchModal) {
            curFlags |= WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
        } else {
            curFlags &= ~WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
        }

        boolean mOutsideTouchable = false;
        if (mOutsideTouchable) {
            curFlags |= WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH;
        } else {
            curFlags &= ~WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH;
        }

        return curFlags;
    }

    //初始化悬浮视图
    private void initSurfaceWindow() {
        //Log.d("mylog", "initSurfaceWindow: ");

        if(windowManager == null){
            windowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        }
        if(mWmParam == null){
            mWmParam = new WindowManager.LayoutParams();
            mWmParam.width = WindowManager.LayoutParams.MATCH_PARENT;
            mWmParam.height = WindowManager.LayoutParams.MATCH_PARENT;
            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.O) {
                // android 8.0及以后使用
                mWmParam.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
                mWmParam.flags = computeFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL);
            } else {
                // android 8.0以前使用
                mWmParam.type = WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY;
                mWmParam.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                mWmParam.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            }
            mWmParam.gravity = Gravity.LEFT | Gravity.TOP;
            mWmParam.format = PixelFormat.RGBA_8888;
            if(ConfigHelper.isHide) {
                mWmParam.setTitle("com.miui.screenrecorder");
                mWmParam.flags = 4136 | 8 | 16 | 400 | 512;
            }
        }
    }

    private void showFloatingWindow() {
        surfaceView = new DrawView(this);
        windowManager.addView(surfaceView, mWmParam);
    }
}
