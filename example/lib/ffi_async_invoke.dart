import 'dart:ffi';
import 'dart:isolate';
import 'package:ffi/ffi.dart';
import 'package:flutter_ffi_tutorial_example/ffi_invoke.dart';
import 'dart:async';
import 'package:uuid/uuid.dart';

final int Function(Pointer<Void> p1) initializeApi =
    ffiLib.lookupFunction<IntPtr Function(Pointer<Void>), int Function(Pointer<Void>)>('InitDartApiDL');

final void Function(Pointer<Work> p1) executeCallback =
    ffiLib.lookupFunction<Void Function(Pointer<Work>), void Function(Pointer<Work>)>('ExecuteCallback');

class Work extends Opaque {}

void requestExecuteCallback(dynamic message) {
  final int workAddress = message as int;
  final Pointer<Work> work = Pointer<Work>.fromAddress(workAddress);
  executeCallback(work);
}

final ReceivePort interactiveCppRequests = ReceivePort()..listen(requestExecuteCallback);

typedef callback_type = Void Function(Pointer<Utf8> cId, Int32 result);
final Map<String, Completer<int>> completerMapping = <String, Completer<int>>{};
final Uuid uuid = Uuid();

Future<int> platformAsync(int value1, int value2) {
  final Completer<int> completer = Completer<int>();
  final String cid = uuid.v1();
  final Pointer<Utf8> cidPtr = cid.toNativeUtf8();
  completerMapping[cid] = completer;
  final int nativePort = interactiveCppRequests.sendPort.nativePort;
  final cMethod = ffiLib.lookupFunction<
      Int32 Function(Pointer<Utf8> cId, Int64 sendPort, Int32 value1, Int32 value2,
          Pointer<NativeFunction<callback_type>> callbackBlock),
      int Function(Pointer<Utf8> cId, int sendPort, int value1, int value2,
          Pointer<NativeFunction<callback_type>> callbackBlock)>('platform_async');

  cMethod(cidPtr, nativePort, value1, value2, Pointer.fromFunction<callback_type>(_callbackBlocking));
  return completer.future;
}

void _callbackBlocking(Pointer<Utf8> cId, int result) {
  final String cidStr = cId.toDartString();
  (() async {
    final Completer<int> c = completerMapping[cidStr];
    if (c == null) {
      return;
    }
    c.complete(result);
    completerMapping.remove(cId);
  })();
}
