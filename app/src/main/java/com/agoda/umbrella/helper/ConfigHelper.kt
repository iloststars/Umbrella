package com.agoda.umbrella.helper

import android.content.Context
import android.content.SharedPreferences
import com.agoda.umbrella.StubApp
import com.agoda.umbrella.utils.AppUtils
import com.agoda.umbrella.utils.RandomUtils

class ConfigHelper {

    companion object {

        private var shp: SharedPreferences? = null

        const val api_singleCode = "https://w.eydata.net/71986B53DE5788CC" //单码登录
        const val api_deadline = "https://w.eydata.net/511F322E0799032A" //到期时间
        const val api_bulletin = "https://w.eydata.net/7B63767C4FD1A678" //程序公告
        const val api_version = "https://w.eydata.net/9DFF384DFBBB4DAD" //检测版本
        const val api_unbind = "https://w.eydata.net/827C2BB591B50648" //解绑

        @JvmField
        var ScreenX = 2340

        @JvmField
        var ScreenY = 1080

        @JvmField
        var statusCode = ""

        @JvmField
        var deadline = ""

        @JvmField
        var singleCode: String = ""

        @JvmField
        var version:String

        @JvmField
        var mac :String

        @JvmField
        var fps = 50

        @JvmField
        var package_name = AppUtils.getPackageName(StubApp.getContext())

        @JvmField
        var key = "0"

        @JvmField
        var mode = "inject"

        @JvmField
        var isHide = true

        @JvmField
        var sharedId = RandomUtils.getRandomString(8)



        init {
            shp = StubApp.getContext().getSharedPreferences("Oo0Oo0", Context.MODE_PRIVATE)
            deadline = shp?.getString("deadline", "").toString()
            singleCode = shp?.getString("singleCode", "").toString()
            version = shp?.getString("version", AppUtils.getVersionName(StubApp.getContext())).toString()
            mac = shp?.getString("mac", "").toString()
            fps = shp?.getInt("fps", 50)!!
            mode = shp?.getString("mode", "inject")!!
            isHide = shp?.getBoolean("isHide", true)!!
        }

        fun save() {
            shp?.edit()
                ?.putString("deadline", deadline)
                ?.putString("singleCode", singleCode)
                ?.putString("version", version)
                ?.putString("mac", mac)
                ?.putString("package_name", package_name)
                ?.putInt("ScreenX", ScreenX)
                ?.putInt("ScreenY", ScreenY)
                ?.putInt("fps", fps)
                ?.putString("mode", mode)
                ?.putBoolean("isHide", isHide)
                ?.apply()
        }
    }

}