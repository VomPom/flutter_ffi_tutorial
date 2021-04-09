import 'dart:ffi';
import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter_ffi_tutorial_example/ffi_async_invoke.dart';
import 'package:flutter_ffi_tutorial_example/ffi_invoke.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _result = '';

  @override
  void initState() {
    initializeApi(NativeApi.initializeApiDLData);
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: Text('Flutter FFI Tutorial')),
        body: Container(
          width: double.infinity,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              _buttonItem('Invoke C without return value', () => _syncMethodMultiType('c_without_return_value')),
              _buttonItem('Invoke C with int value return', () => _syncMethodMultiType('c_with_int_value_return')),
              _buttonItem('Invoke sync Java/Kotlin method', () => _syncMethodMultiType('platform_sync')),
              _buttonItem('Invoke async Java/Kotlin method', () => _syncMethodMultiType('platform_async')),
              SizedBox(height: 80),
              consolePanel(),
            ],
          ),
        ),
      ),
    );
  }

  get randomValue => Random().nextInt(10000);

  void _syncMethodMultiType(String method) async {
    int value1 = randomValue;
    int value2 = randomValue;

    if (method == 'c_without_return_value') {
      cWithoutReturnValue(value1);
      _result = 'Had send the message to c with value:$value1, please check result in develop kit console log';
    } else if (method == 'c_with_int_value_return') {
      int addResult = cWitReturnValue(value1, value2);
      _result = 'Result from C:$value1+$value2=$addResult';
    } else if (method == 'platform_sync') {
      int addResult = platformSync(value1, value2);
      _result = 'Result from Java/Kotlin sync invoke:$value1+$value2=$addResult';
    } else if (method == 'platform_async') {
      _result = 'Start to invoke for result...';
      setState(() {});
      platformAsync(value1, value2).then((value) {
        _result = 'Got result from Java/Kotlin async invoke:$value1+$value2=$value';
        setState(() {});
      });
      _result = 'Waiting for result... ';
    } else {
      _result = 'No implement... ';
    }
    setState(() {});
  }

  Widget _buttonItem(String title, VoidCallback _onPressed) {
    return Container(
      constraints: BoxConstraints(
        minWidth: 250,
        minHeight: 40,
      ),
      margin: EdgeInsets.only(right: 10),
      child: ElevatedButton(child: Text(title), onPressed: _onPressed),
    );
  }

  Widget consolePanel() {
    return Card(
      color: Colors.white70,
      elevation: 1.5,
      child: Container(
        margin: EdgeInsets.symmetric(horizontal: 10),
        constraints: BoxConstraints(minHeight: 200),
        width: double.infinity,
        padding: EdgeInsets.symmetric(horizontal: 10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            SizedBox(height: 10),
            Text("Console:", softWrap: true, style: TextStyle(color: Colors.grey, fontSize: 13)),
            Text(_result, softWrap: true)
          ],
        ),
      ),
    );
  }
}
