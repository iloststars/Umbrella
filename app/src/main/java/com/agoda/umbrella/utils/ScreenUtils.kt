package com.agoda.umbrella.utils

import android.content.Context
import android.graphics.Point
import android.view.WindowManager
import com.agoda.umbrella.StubApp

class ScreenUtils {
    companion object{
        fun getRealSize(): Point? {
            val wm = StubApp.getContext().getSystemService(Context.WINDOW_SERVICE) as WindowManager
            val display = wm.defaultDisplay
            val p = Point()
            display.getRealSize(p)
            return p
        }

        @JvmStatic
        fun getScreenWidth(): Int? {
            return getRealSize()?.x
        }

        @JvmStatic
        fun getScreenHeight(): Int? {
            return getRealSize()?.y
        }
    }
}