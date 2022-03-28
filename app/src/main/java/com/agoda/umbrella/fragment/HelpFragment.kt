package com.agoda.umbrella.fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import com.agoda.umbrella.R
import com.agoda.umbrella.databinding.FragmentHelpBinding
import io.noties.markwon.Markwon


class HelpFragment : Fragment(){
    private lateinit var binding: FragmentHelpBinding
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View{
        binding = FragmentHelpBinding.inflate(inflater)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        Markwon.create(requireContext()).setMarkdown(binding.helpTextView,getString(R.string.help_content))

    }

    override fun onStart() {
        super.onStart()
        activity?.findViewById<Button>(R.id.btn_navUp)?.visibility = View.VISIBLE
    }

}