package com.agoda.umbrella.activity

import android.app.AlertDialog
import android.content.DialogInterface
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.View
import android.widget.Toast
import com.agoda.umbrella.StubApp
import com.agoda.umbrella.databinding.ActivityLoginBinding
import com.agoda.umbrella.helper.ConfigHelper
import com.agoda.umbrella.helper.EydataHelper
import com.agoda.umbrella.utils.AppUtils
import com.agoda.umbrella.utils.HttpUtils
import kotlin.system.exitProcess

class LoginActivity : BaseActivity() {

    private lateinit var binding: ActivityLoginBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)
        Thread{
            ConfigHelper.mac = AppUtils.getMac()
            ConfigHelper.save()
        }.start()

        //填充历史信息
        binding.etSingleCode.setText(ConfigHelper.singleCode)
        binding.loginTvVersion.text = "Umbrella. App version:${ConfigHelper.version}"

        checkUpdate()
        binding.btnLogin.setOnClickListener {
            binding.progressBar.visibility  = View.VISIBLE
            //登录
            Thread{
                ConfigHelper.singleCode = binding.etSingleCode.text.toString().trim { it <= ' ' }
                ConfigHelper.save()
                StubApp.getHandler().postDelayed(Thread {
                    var data: String = o0o0oo0o0()
                    data = if (data.length == 32) "登录成功" else EydataHelper.error(data)
                    Toast.makeText(this, data, Toast.LENGTH_SHORT).show()
                    if (data == "登录成功") {
                        ConfigHelper.key = oo0000oo0()
                        ConfigHelper.deadline = "到期时间:" + HttpUtils.sendPost(ConfigHelper.api_deadline, "UserName=" + ConfigHelper.singleCode)
                        ConfigHelper.save()
                        startActivity(Intent(this,CoreActivity::class.java))
                        finish()
                    }
                    binding.progressBar.visibility  = View.INVISIBLE
                },100)

            }.start()

        }

        binding.btnUnbind.setOnClickListener {
            StubApp.getHandler().postDelayed({
                val data = EydataHelper.unbind(
                    ConfigHelper.api_unbind,
                    binding.etSingleCode.text.toString(),
                    "",
                    ConfigHelper.mac
                )
                Toast.makeText(StubApp.getContext(), data, Toast.LENGTH_LONG).show()
            }, 100)
        }
    }

    private fun checkUpdate() {
        StubApp.getHandler().postDelayed({
            //强制更新
            var data = EydataHelper.checkUpdate(ConfigHelper.api_version,ConfigHelper.version)
            if (data != "最新版本") {
                val builder =
                    AlertDialog.Builder(this)
                builder.setTitle("检测到更新")
                builder.setMessage("请联系上级获取最新最新版本")
                builder.setCancelable(false)
                builder.setNegativeButton(
                    "官网"
                ) { _: DialogInterface?, _: Int ->
                    startActivity(
                        Intent(
                            Intent.ACTION_VIEW,
                            Uri.parse("http://yangsan.tk")
                        )
                    )
                    exitProcess(0)
                }
                builder.show()
            }else{
                //获取公告
                data = EydataHelper.getBoard(ConfigHelper.api_bulletin)
                showDialog("公告", data)
            }
        }, 1000)
    }

    //弹窗
    private fun showDialog(title: String, data: String) {
        runOnUiThread {
            if (this@LoginActivity.isFinishing) return@runOnUiThread
            val builder =
                AlertDialog.Builder(this@LoginActivity)
            builder.setTitle(title)
            builder.setMessage(data)
            builder.setCancelable(false)
            builder.setPositiveButton("确定", null)
            builder.show()
        }
    }

    //易游登录
    external fun o0o0oo0o0(): String
    //获取地址
    external fun oo0000oo0(): String
}