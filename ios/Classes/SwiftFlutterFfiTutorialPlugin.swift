import Flutter
import UIKit

public class SwiftFlutterFfiTutorialPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "flutter_ffi_tutorial", binaryMessenger: registrar.messenger())
    let instance = SwiftFlutterFfiTutorialPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    result("iOS " + UIDevice.current.systemVersion)
  }
}
