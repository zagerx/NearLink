## 目的
- 标准的UUID  [官方链接](https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf?v=1723363990696)
- 自定义的服务
- 客户端和服务器之间的交互
- 



## GAP通用文件配置
    
- 设置/获取设备本地名称 `gap_ble_set_local_name()`
- 设置/获取本地地址 `gap_ble_set_local_addr()`
- 设置广播数据，广播参数，设置扫描参数，设置物理地址
- 设置物理参数`gap_ble_set_phy()`,设置配对相关`gap_ble_pair_remote_device()`

## GATT
- `关键词`：`服务`

## ATT
- `关键词`：`属性`






## 实现蓝牙心率计步骤

- 两个`默认服务`
    - Generic Access Service (GAP)
        - `0x1800`GAP服务
            - `0x2A00` 特征值  设备名称(Device Name的)UUID
            - `0x2A01` 特征值  设备外观(Appearance)的UUID
            - `0x2A04` 特征值  PPCP特性(Peripheral Preferred Connection Parameters)的UUID
                - PPCP特性允许BLE外设（如传感器、健康监测设备等）指定其首选的连接参数，如连接间隔、从属延迟、监控超时等。这些参数对于优化BLE连接的功耗和性能至关重要。通过读取PPCP特性，BLE中心设备（如智能手机、平板电脑等）可以了解外设的连接需求，并据此调整其连接参数，以实现更高效的通信
            - `0x2AAB` (CGM Session Run Time)<span style="color: red;">待确认</span>
    - Generic Attribute Service (GATT)
        - `0x1801`GATT服务
            - `0x2A05` 特征值  服务变化(Service Changed)的UUID
            ```在BLE的交互过程中，如果客户端设备在连接过程中订阅了“Service Changed”特性的通知或指示，那么当服务发生变化时，它将接收到一个包含更改信息的通知或指示。这使得客户端设备能够动态地更新其内部的服务和特性列表，而无需重新执行完整的服务发现过程```
            - `0x2902` 描述符 （Client Characteristic Configuration）的UUID
- 心率服务的标准UUID:`0x180D`
- 心率特征的标准UUID:`0x2A37`

### 具体操作

- 设置心率服务的读写特征




