# Android Binder 跨进程通信简单实例

这个示例演示了一个最小可理解的 Android Binder IPC（AIDL）流程：

- `MainActivity`（主进程）通过 `bindService` 绑定远程服务
- `RemoteCalculatorService` 运行在 `:remote` 进程
- 通过 AIDL 接口 `IRemoteCalculator` 跨进程调用 `add`、`greet`
- 使用 `CalculatorCore` 封装纯业务逻辑，便于脱离 Android 环境做本地自测

---

## 目录结构

```text
app/src/main/aidl/com/example/binderipc/IRemoteCalculator.aidl
app/src/main/java/com/example/binderipc/CalculatorCore.java
app/src/main/java/com/example/binderipc/RemoteCalculatorService.java
app/src/main/java/com/example/binderipc/MainActivity.java
app/src/main/res/layout/activity_main.xml
app/src/main/AndroidManifest.xml
selftest/test_binder_demo.py
selftest/java/com/example/binderipc/CalculatorCoreSelfTest.java
```

---

## 核心点说明

1. **AIDL 接口定义**
   - `add(int left, int right)`
   - `greet(String name)`

2. **远程进程 Service**
   - Manifest 中将 Service 声明为：
     - `android:process=":remote"`（与 Activity 不同进程）
     - `android:exported="false"`（仅供本应用内部调用）

3. **客户端绑定与调用**
   - `MainActivity` 使用 `IRemoteCalculator.Stub.asInterface(service)` 拿到代理
   - 调用远程方法后，将 clientPid 与返回内容展示在 UI

4. **边界处理**
   - 名字为空/null 时回退为 `Guest`
   - `add` 做整型溢出校验，溢出时抛 `ArithmeticException`

---

## 如何在 Android Studio 运行

1. 用 Android Studio 打开当前目录
2. 安装/配置 Android SDK 与 Gradle（Android Studio 会引导）
3. 连接真机或启动模拟器
4. 运行 `app`
5. 在页面中：
   - 点击“绑定远程服务”
   - 输入名字后点击“调用远程方法”
   - 观察结果中的 `clientPid` 与 `serverPid`（通常不同，说明跨进程调用成功）

---

## 本地自动自测（当前仓库可直接执行）

```bash
python3 -m unittest discover -s selftest -p "test_*.py"
```

自测包含：

- Manifest 远程进程配置检查
- AIDL 接口合同检查
- Activity 绑定与调用关键路径检查
- `javac/java` 编译并执行 `CalculatorCoreSelfTest`（功能 + 边界）

