package com.agoda.umbrella.fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import com.agoda.umbrella.R
import com.agoda.umbrella.databinding.FragmentSettingBinding

class SettingFragment : Fragment() {
    private lateinit var binding: FragmentSettingBinding
    private lateinit var aaa:String

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View{
        binding = FragmentSettingBinding.inflate(inflater)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
//        binding.rgMode.check(if (ConfigHelper.mode ==1) binding.rbInject.id else binding.rbRoot.id)
//        binding.rgMode.setOnCheckedChangeListener { group, checkedId ->
//            ConfigHelper.mode = if (checkedId == binding.rbInject.id) 1 else 2
//            ConfigHelper.save()
//
//        }
//
//        binding.btnInject.setOnClickListener {
//            FileUtils.copyFilesFromRaw(
//                StubApplication.getContext(),
//                R.raw.part3,
//                "part3",
//                FileUtils.getFilesPath()
//            )
//            val path = FileUtils.getFilesPath()
//            //Log.d("myjni", "onActivityCreated: "+path)
//            o0o0000oo0(path)
//        }
//
//        binding.btnClean.setOnClickListener {
//            FileUtils.copyFilesFromRaw(
//                StubApplication.getContext(),
//                R.raw.part4,
//                "part4",
//                FileUtils.getFilesPath()
//            )
//            oo0000ooo0(FileUtils.getFilesPath())
//        }
    }

    override fun onStart() {
        super.onStart()
        activity?.findViewById<Button>(R.id.btn_navUp)?.visibility = View.VISIBLE
    }

//    //注入
//    external fun o0o0000oo0(filePath:String)
//    //替换
//    external fun oo0000ooo0(filePath:String)

}