package com.example.flutter_ffi_tutorial;

import android.os.Handler;
import android.os.Looper;

import org.jetbrains.annotations.NotNull;


/*******************************************************
 *
 * Created by julis.wang on 2021/04/09 14:52
 *
 * Description :
 *
 * History   :
 *
 *******************************************************/

public class HandleFFiMessageManagerJava {
    public static int handleFFISyncCall(int value1, int value2) {
        return value1 + value2;
    }

    public static void handleFFIAsyncCall(final int value1, final int value2, long dartSendPort, String cId) {
        final SimulatedResult simulatedResult = new SimulatedResult(new Identify(dartSendPort, cId));
        simulatedResult.setListener(new SimulatedResult.ResultListener() {
            @Override
            public void onResult(int result, @NotNull Identify identify) {
                sendMessageToNative(result, identify.getDartSendPort(), identify.getDartCid().getBytes());
            }
        });


        //Simulating asynchronous is time-consuming
        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                simulatedResult.getResult(value1, value2);
            }
        }, 2000);
    }


    native static void sendMessageToNative(int result, long dartSendPort, byte[] cid);
}
