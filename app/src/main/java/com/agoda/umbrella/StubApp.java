package com.agoda.umbrella;

import android.app.Application;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import com.agoda.umbrella.utils.Tools;
import com.hjq.permissions.XXPermissions;


public class StubApp extends Application {


    private static Context mContext;
    private static Handler mHandler;

    @Override
    protected void attachBaseContext(Context base)
    {
        super.attachBaseContext(base);
        initExceptionCatcher();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mContext = getApplicationContext();
        mHandler = new Handler();
        XXPermissions.setScopedStorage(true);
    }

    //mInstance必然不会为null，onCreate在调用此方法前已执行
    public static Context getContext() {
        return mContext;
    }

    public static Handler getHandler() {
        return mHandler;
    }

    private void initExceptionCatcher()
    {
        new Handler(Looper.getMainLooper()).post(() -> {
            while (true)
            {
                try
                {
                    Looper.loop();
                }
                catch (final Throwable e)
                {
                    new Handler(Looper.getMainLooper()).post(new Runnable(){
                        public void run()
                        {
                            Tools.showException(e);
                        }
                    });
                }
            }
        });
        final Thread.UncaughtExceptionHandler uncatch = (t, e) -> new Handler(Looper.getMainLooper()).post(() -> Tools.showException(e));
        Thread.setDefaultUncaughtExceptionHandler(uncatch);
    }

}
