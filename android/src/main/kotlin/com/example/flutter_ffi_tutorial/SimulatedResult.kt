package com.example.flutter_ffi_tutorial

/*******************************************************
 *
 * Created by julis.wang on 2021/04/09 14:11
 *
 * Description :
 *
 * History   :
 *
 *******************************************************/

class SimulatedResult(identify: Identify) {


    var identify: Identify? = identify
    var listener: ResultListener? = null

    fun getResult(value1: Int, value2: Int) {
        listener?.onResult(value1 + value2, identify!!)
    }

    interface ResultListener {
        fun onResult(result: Int, identify: Identify)
    }
}