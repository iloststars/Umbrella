package com.agoda.umbrella.fragment

import android.annotation.SuppressLint
import android.content.ClipData
import android.content.ClipboardManager
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.agoda.umbrella.R
import com.agoda.umbrella.StubApp
import com.agoda.umbrella.data.DataProvider
import com.agoda.umbrella.databinding.FragmentCoreBinding
import com.agoda.umbrella.helper.ConfigHelper
import com.agoda.umbrella.helper.DrawHelper
import com.agoda.umbrella.service.SurfaceService
import com.agoda.umbrella.utils.FileUtils
import com.agoda.umbrella.utils.ShareUtils
import com.agoda.umbrella.view.DrawView
import com.agoda.umbrella.view.ISeekBarListener
import com.lzf.easyfloat.EasyFloat
import com.lzf.easyfloat.anim.DefaultAnimator
import com.lzf.easyfloat.enums.ShowPattern
import java.io.UnsupportedEncodingException
import java.net.URLEncoder

class CoreFragment : Fragment() {
    private lateinit var binding: FragmentCoreBinding
    private var canvasService: Intent? = null
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        binding = FragmentCoreBinding.inflate(inflater)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        binding.coreTvVersion.text = "Umbrella. App version:${ConfigHelper.version}"
        binding.tvDeadline.text = ConfigHelper.deadline
        binding.btnStart.setOnClickListener { showFloatMenu() }
        binding.btnClose.setOnClickListener { EasyFloat.dismiss("float_menu") }

        binding.rgMode.check(if(ConfigHelper.mode == "inject") R.id.rb_inject else R.id.rb_root)

        binding.rgMode.setOnCheckedChangeListener { group, checkedId ->
            if(checkedId == R.id.rb_inject)
                ConfigHelper.mode = "inject";
            else
                ConfigHelper.mode = "root"
            ConfigHelper.save()
        }

        binding.btnInject.setOnClickListener {
                FileUtils.copyFilesFromRaw(
                    StubApp.getContext(),
                    R.raw.part3,
                    "part3",
                    FileUtils.getFilesPath()
                )
            val path = FileUtils.getFilesPath()
            //Log.d("myjni", "onActivityCreated: "+path)
            o0o0000oo0(path)
            Toast.makeText(
                StubApp.getContext(),
                "注入成功",
                Toast.LENGTH_SHORT
            ).show()
        }

        binding.btnClean.setOnClickListener {
            FileUtils.copyFilesFromRaw(
                StubApp.getContext(),
                R.raw.part4,
                "part4",
                FileUtils.getFilesPath()
            )
            oo0000ooo0(FileUtils.getFilesPath())
            Toast.makeText(
                StubApp.getContext(),
                "清理注入完成",
                Toast.LENGTH_SHORT
            ).show()
        }

        binding.btnShare.setOnClickListener {
            Toast.makeText(activity?.applicationContext, "已复制到剪切板!", Toast.LENGTH_SHORT).show()
            val clipboard =
                (activity?.getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager?)!!
            var uri: Uri? = null
            try {
                clipboard.setPrimaryClip(
                    ClipData.newPlainText(
                        null,
                        "http://pay.asfaka.net:8080/?id=" + ConfigHelper.sharedId
                    )
                )
                uri = Uri.parse(
                    "http://pay.asfaka.net:8080/?id=" + URLEncoder.encode(
                        ConfigHelper.sharedId,
                        "UTF-8"
                    )
                )
            } catch (e: UnsupportedEncodingException) {
                e.printStackTrace()
            }
            val intent = Intent(Intent.ACTION_VIEW, uri)
            startActivity(intent)
        }
//        binding.btnSetting.setOnClickListener { findNavController().navigate(R.id.action_coreFragment_to_settingFragment) }
        binding.btnHelp.setOnClickListener { findNavController().navigate(R.id.action_coreFragment_to_helpFragment) }
        canvasService = Intent(StubApp.getContext(), SurfaceService::class.java)
    }

    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private fun showFloatMenu() {
        context?.let {
            EasyFloat.with(it)
                .setTag("float_menu")
                .setShowPattern(ShowPattern.ALL_TIME)
                .setDragEnable(true)
                .setAnimator(DefaultAnimator())
                .setLayout(R.layout.float_menu) { root ->
                    val imageButton = root.findViewById<ImageButton>(R.id.imageButton)
                    val floatMenu = root.findViewById<View>(R.id.float_menu)
                    val btnMenuClose = root.findViewById<View>(R.id.btn_menu_close)

                    val switchRay = root.findViewById<Switch>(R.id.switch_ray)
                    val switchBox = root.findViewById<Switch>(R.id.switch_box)
                    val switchBuff = root.findViewById<Switch>(R.id.switch_buff)
                    val switchAnimal = root.findViewById<Switch>(R.id.switch_animal)
                    val switchSoldier = root.findViewById<Switch>(R.id.switch_soldier)
                    val switchCd = root.findViewById<Switch>(R.id.switch_cd)
                    val switchRecord = root.findViewById<Switch>(R.id.switch_record)

                    switchRay.isChecked = DrawHelper.draw_ray
                    switchBox.isChecked = DrawHelper.draw_rect
                    switchBuff.isChecked = DrawHelper.draw_buff
                    switchAnimal.isChecked = DrawHelper.draw_animal
                    switchSoldier.isChecked = DrawHelper.draw_soldier
                    switchCd.isChecked = DrawHelper.draw_cd
                    switchRecord.isChecked = ConfigHelper.isHide

                    floatMenu.visibility = View.GONE
                    imageButton.setOnClickListener {
                        floatMenu.visibility = View.VISIBLE
                        imageButton.visibility = View.GONE
                    }
                    btnMenuClose.setOnClickListener {
                        floatMenu.visibility = View.GONE
                        imageButton.visibility = View.VISIBLE
                    }

                    //switch监听
                    root.findViewById<Switch>(R.id.switch_run).also {
                        it.setOnCheckedChangeListener { _, b ->
                            if (b) {//TODO 开启
                                ConfigHelper.save()
                                //执行内核
                                DataProvider.runBinary()
                                //创建画布
                                StubApp.getContext().startService(canvasService)
                                DrawView.controlFlag = true
                            } else { //TODO 关闭
                                ConfigHelper.save()
                                Toast.makeText(
                                    StubApp.getContext(),
                                    "功能关闭",
                                    Toast.LENGTH_SHORT
                                ).show()
                                StubApp.getContext().stopService(canvasService)
                                DrawView.controlFlag = false
                                DataProvider.killBinary()
                            }
                        }
                    }

                    switchRay.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_ray = isChecked
                        DrawHelper.save()
                    }


                    switchBox.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_rect = isChecked
                        DrawHelper.save()
                    }



                    switchBuff.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_buff = isChecked
                        DrawHelper.save()
                    }


                    switchAnimal.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_animal = isChecked
                        DrawHelper.save()
                    }


                    switchSoldier.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_soldier = isChecked
                        DrawHelper.save()
                    }


                    switchCd.setOnCheckedChangeListener { buttonView, isChecked ->
                        DrawHelper.draw_cd = isChecked
                        DrawHelper.save()
                    }

                    root.findViewById<Switch>(R.id.switch_share).also {
                        it.setOnCheckedChangeListener { buttonView, isChecked ->
                            if (isChecked) {
                                ShareUtils.sendflag = true
                                ShareUtils.asynSend()
                            } else {
                                ShareUtils.sendflag = false
                            }
                        }
                    }

                    root.findViewById<Switch>(R.id.switch_record).also {
                        it.setOnCheckedChangeListener { buttonView, isChecked ->
                            ConfigHelper.isHide = isChecked
                            ConfigHelper.save()
                            Toast.makeText(
                                StubApp.getContext(),
                                "请重新开启绘制确保生效",
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    }

                    //seekbar
                    root.findViewById<SeekBar>(R.id.seekbarX)
                        .setOnSeekBarChangeListener(object : ISeekBarListener() {
                            override fun onProgressChanged(seekBar: SeekBar?, i: Int, b: Boolean) {
                                super.onProgressChanged(seekBar, i, b)
                                DrawHelper.icon_offset_x = i
                                DrawHelper.save()
                            }
                        })

                    root.findViewById<SeekBar>(R.id.seekbarY)
                        .setOnSeekBarChangeListener(object : ISeekBarListener() {
                            override fun onProgressChanged(seekBar: SeekBar?, i: Int, b: Boolean) {
                                super.onProgressChanged(seekBar, i, b)
                                DrawHelper.icon_offset_y = i
                                DrawHelper.save()
                            }
                        })

                }
                .show()


        }
    }

    override fun onStart() {
        super.onStart()
        activity?.findViewById<Button>(R.id.btn_navUp)?.visibility = View.INVISIBLE
    }

    //注入
    external fun o0o0000oo0(filePath: String)

    //替换
    external fun oo0000ooo0(filePath: String)
}