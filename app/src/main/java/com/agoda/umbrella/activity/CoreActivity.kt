package com.agoda.umbrella.activity

import android.os.Bundle
import android.view.View
import androidx.navigation.findNavController
import com.agoda.umbrella.R
import com.agoda.umbrella.databinding.ActivityCoreBinding


class CoreActivity : BaseActivity() {

    private lateinit var binding: ActivityCoreBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityCoreBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val navController = findNavController(R.id.fragment)
        binding.btnNavUp.setOnClickListener {
            navController.navigate(R.id.coreFragment)
            binding.btnNavUp.visibility = View.INVISIBLE
        }
    }
}