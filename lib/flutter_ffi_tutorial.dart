
import 'dart:async';

import 'package:flutter/services.dart';

class FlutterFfiTutorial {
  static const MethodChannel _channel =
      const MethodChannel('flutter_ffi_tutorial');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
