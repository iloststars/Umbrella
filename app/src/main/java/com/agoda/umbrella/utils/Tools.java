package com.agoda.umbrella.utils;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.List;
import java.util.Map;

public class Tools
{
	public static void copyFile(String oldPath, String newPath)
	{
		try
		{
			InputStream fis = new FileInputStream(oldPath);
			OutputStream fos = new FileOutputStream(newPath);
			int i;
			byte[] buf = new byte[1024];
			while ((i = fis.read(buf)) != -1)
			{
				fos.write(buf, 0, i);
			}
			fis.close();
			fos.close();
		}
		catch (Exception e)
		{
			showException(e);
		}
	}
	
	public static Activity getGameActivity()
	{
		Activity act = null;
		try
		{
			Class wegameClz = Class.forName("com.tencent.msdk.WeGame");
			Method getInstance = wegameClz.getDeclaredMethod("getInstance");
			Object instance = getInstance.invoke(wegameClz);
			Method getActivity = wegameClz.getDeclaredMethod("getActivity");
			act = (Activity) getActivity.invoke(instance);
		}
		catch (Exception e)
		{
			Tools.showException(e);
		}
		return act;
	}

	public static Activity getCurrentActivity()
	{
		try
		{
			Class activityThreadClass = Class.forName("android.app.ActivityThread");
			Method currentActivityThreadMtd = activityThreadClass.getDeclaredMethod("currentActivityThread");
			currentActivityThreadMtd.setAccessible(true);
			Object currentActivityThread = currentActivityThreadMtd.invoke(null);
			Method getApplicationMtd = currentActivityThread.getClass().getMethod("getApplication");
			Context application = (Context) getApplicationMtd.invoke(currentActivityThread);
			String topActivity = ((ActivityManager) application.getSystemService(Context.ACTIVITY_SERVICE)).getRunningTasks(1).get(0).topActivity.getClassName();
			Field mActivitiesFd = activityThreadClass.getDeclaredField("mActivities");
			mActivitiesFd.setAccessible(true);
			Map mActivities = (Map) mActivitiesFd.get(currentActivityThread);
			for (Object value : mActivities.values())
			{
				Field activityFd = value.getClass().getDeclaredField("activity");
				activityFd.setAccessible(true);
				Activity activity = (Activity) activityFd.get(value);
				if (topActivity.equals(activity.getClass().getName()))
				{
					return activity;
				}
			}
		}
		catch (Exception e)
		{
		}
		return null;
	}

	public static void showAlert(final Context context,final String title,final String content,final String[] button,final AlertDialog.OnClickListener[] onClickList)
	{
		Runnable run = new Runnable(){
			public void run()
			{
				AlertDialog.Builder ab = new AlertDialog.Builder(context);
				ab.setTitle(title);
				ab.setMessage(content);
				ab.setCancelable(true); 
				//右侧按钮
				if(button.length>=1)
					if(button[0]!=null)
						ab.setPositiveButton(button[0], onClickList==null?null:onClickList[0]); 
				//中间按钮
				if(button.length>=2)
					if(button[1]!=null)
						ab.setNegativeButton(button[1], onClickList==null?null:onClickList[1]);
				//左侧按钮
				if(button.length>=3)
					if(button[2]!=null)
						ab.setNeutralButton(button[2], onClickList==null?null:onClickList[2]);
				final AlertDialog dialog = ab.create();
				if (Build.VERSION.SDK_INT >= 26)
				{
					dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY);
				}
				else
				{
					dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
				}
				dialog.show();
			}
		};
		new Handler(Looper.getMainLooper()).post(run);
	}
	public static void showAlert(Context context,String content){
		showAlert(context,"",content,new String[]{null},null);
	}
	public static void showAlert(Context context,String title,String content){
		showAlert(context,title,content,new String[]{null},null);
	}
	public static void showAlert(Context context,String title,String content,String button1){
		showAlert(context,title,content,new String[]{button1},null);
	}
	public static void showAlert(Context context,String title,String content,String button1,AlertDialog.OnClickListener click){
		showAlert(context,title,content,new String[]{button1},new AlertDialog.OnClickListener[]{click});
	}
	
	public static void showException(Throwable th)
	{
		final String throwableMessage = Log.getStackTraceString(th);
		AlertDialog.Builder ab = new AlertDialog.Builder(getCurrentActivity());
		ab.setTitle("很抱歉，程序出错了");
		ab.setMessage(throwableMessage);
		ab.setCancelable(false);
		ab.setNegativeButton("退出应用", new DialogInterface.OnClickListener(){
				public void onClick(DialogInterface di, int i)
				{
					di.dismiss();
					exit();
				}
			});
		ab.setPositiveButton("复制日志", new DialogInterface.OnClickListener(){
				public void onClick(DialogInterface di, int i)
				{
					ClipboardManager cm = (ClipboardManager) getCurrentActivity().getSystemService(Context.CLIPBOARD_SERVICE);
					cm.setText(throwableMessage);
					cm.setPrimaryClip(ClipData.newPlainText("TianShaException", throwableMessage));
					showToast("复制完成");
				}
			});
		ab.setNeutralButton("继续运行", new DialogInterface.OnClickListener(){
				public void onClick(DialogInterface di, int i)
				{
					di.dismiss();
				}
			});
		AlertDialog dialog = ab.create();
		dialog.show();
	}

	public static void exit()
	{
		ActivityManager mActivityManager = (ActivityManager) getCurrentActivity().getSystemService(Context.ACTIVITY_SERVICE);
		List<ActivityManager.RunningAppProcessInfo> mList = mActivityManager.getRunningAppProcesses();
		for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : mList) 
		{
			if (runningAppProcessInfo.pid != android.os.Process.myPid()) 
			{
				android.os.Process.killProcess(runningAppProcessInfo.pid);
			}
		}
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}

	public static void showToast(final String str)
	{
		new Handler(Looper.getMainLooper()).post(() -> Toast.makeText(getCurrentActivity(), str, Toast.LENGTH_LONG).show());
	}

}
