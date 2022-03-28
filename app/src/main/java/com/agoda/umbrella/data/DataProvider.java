package com.agoda.umbrella.data;

import android.content.pm.PackageManager;

import com.agoda.umbrella.R;
import com.agoda.umbrella.StubApp;
import com.agoda.umbrella.helper.ConfigHelper;
import com.agoda.umbrella.utils.FileUtils;
import com.agoda.umbrella.utils.ShellUtils;


public class DataProvider {
    public static int timmer = 0;

    public static void Init(){
        FileUtils.copyFilesFromRaw(StubApp.getContext(), R.raw.part1,"part1", FileUtils.getFilesPath());
        FileUtils.copyFilesFromRaw(StubApp.getContext(), R.raw.part2,"part2", FileUtils.getFilesPath());
        ShellUtils.execCommand("chmod 777 " + FileUtils.getFilesPath() + "/part1", true);
        ShellUtils.execCommand("chmod 777 " + FileUtils.getFilesPath() + "/part2", true);
    }

    public static void runBinary() {
        new Thread() {
            @Override
            public void run() {
                String partName ="part1";
                if (ConfigHelper.mode.equals("root"))
                    partName = "part2";
                DataProvider.Timmer();
                String packageName = ConfigHelper.package_name;
                String workspace = FileUtils.getFilesPath();
                String singleCode = ConfigHelper.singleCode;
                String version = ConfigHelper.version;
                String key = ConfigHelper.key;
                String cmd =  String.format("%s/%s %s %s %s %s %s", FileUtils.getFilesPath(),partName,packageName,workspace,singleCode,version,key);
                ShellUtils.execCommand(cmd, true);
//                Log.d(TAG, "run: "+result);
//                FileUtils.writeDataFile(StubApplication.getContext(),"aaa",result.successMsg+"\n"+result.errorMsg);
            }
        }.start();
    }

    public static void killBinary() {
        new Thread() {
            @Override
            public void run() {
                ShellUtils.execCommand("pkill part", true);
            }
        }.start();
    }


    public static void Timmer() {
        new Thread(() -> {
            while (true) {

                if (timmer == 100) timmer = 0;
                timmer = timmer + 1;
                try {
                    Thread.sleep(13);//毫秒 50 100 185
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }


}
