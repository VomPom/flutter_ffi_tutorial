import 'dart:ffi';
import 'dart:io';

final DynamicLibrary ffiLib = Platform.isAndroid ? DynamicLibrary.open('libffi_invoke.so') : DynamicLibrary.process();

void cWithoutReturnValue(int value) {
  final cMethod =
      ffiLib.lookupFunction<Void Function(Int32 value), void Function(int value)>('c_with_out_return_value');
  cMethod(value);
}

int cWitReturnValue(int value1, int value2) {
  final cMethod =
      ffiLib.lookupFunction<Int32 Function(Int32 value1, Int32 value2), int Function(int value1, int value2)>(
          'c_with_return_value');
  return cMethod(value1, value2);
}

int platformSync(int value1, int value2) {
  final cMethod =
      ffiLib.lookupFunction<Int32 Function(Int32 value1, Int32 value2), int Function(int value1, int value2)>(
          'platform_sync');
  return cMethod(value1, value2);
}
