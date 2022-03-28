package com.agoda.umbrella.activity

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.agoda.umbrella.R
import com.agoda.umbrella.databinding.ActivitySplashBinding
import com.agoda.umbrella.utils.CustomCountDownTimer
import com.hjq.permissions.Permission
import com.hjq.permissions.XXPermissions

class SplashActivity : BaseActivity() {
    private lateinit var binding: ActivitySplashBinding
    private var timer: CustomCountDownTimer? = null

    init {
        System.loadLibrary("native-lib");
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySplashBinding.inflate(layoutInflater)
        setContentView(binding.root)
        XXPermissions.with(this)
            .permission(Permission.SYSTEM_ALERT_WINDOW)
            .permission(Permission.MANAGE_EXTERNAL_STORAGE)
            .request { _, all ->
                if (all) {
                    timer =
                        CustomCountDownTimer(1000, object : CustomCountDownTimer.ICountDownHandler {
                            override fun onTicker(time: Int) {}
                            override fun onFinish() {
                                gotoLogin() // 跳转主界面
                                finish() // 销毁界面
                            }
                        })
                    timer!!.start()
                }
            }

    }

    //跳转到主界面
    private fun gotoLogin() {
        startActivity(Intent(this@SplashActivity, LoginActivity::class.java))
        finish()
    }

    override fun onDestroy() {
        super.onDestroy()
        timer?.cancel()
    }

}