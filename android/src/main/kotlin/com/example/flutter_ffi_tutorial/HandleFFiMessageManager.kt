package com.example.flutter_ffi_tutorial

import android.os.Handler
import android.os.Looper


/*******************************************************
 *
 * Created by julis.wang on 2021/04/08 12:15
 *
 * Description :
 *
 * History   :
 *
 *******************************************************/

class HandleFFiMessageManager {
    companion object {
        @JvmStatic
        fun handleFFISyncCall(value1: Int, value2: Int): Int {
            return value1 + value2
        }

        @JvmStatic
        fun handleFFIAsyncCall(value1: Int, value2: Int, dartSendPort: Long, cId: String) {

            val simulate = SimulatedResult(Identify(dartSendPort, cId))
            simulate.listener = object : SimulatedResult.ResultListener {
                override fun onResult(result: Int, identify: Identify) {
                    HandleFFiMessageManager().sendMessageToNative(result, identify.dartSendPort, identify.dartCid.toByteArray())
                }
            }

            //Simulating asynchronous is time-consuming
            Handler(Looper.getMainLooper()).postDelayed({
                simulate.run {
                    getResult(value1, value2)
                }
            }, 2000);
        }
    }

    external fun sendMessageToNative(result: Int, dartSendPort: Long, cId: ByteArray)

}