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


## 代码ble_uart_task分析

### ble_uart_server_task启动
- 代码
```C
    ble_uart_server_init();
    errcode_t ret = uapi_uart_register_rx_callback(CONFIG_BLE_UART_BUS,
                                                   UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                                   1, ble_uart_read_int_handler);
```
- 日志
```
-----------------ble_uart_server_task Init Start----------------
[GAP service register][device name] handle: 0x0006, value handle: 0x0007
[GAP service register][device appearance] handle: 0x0008, value handle: 0x0009
[GAP service register][prefer conn param] handle: 0x000a, value handle: 0x000b
[ble uart server] beginning add service
ble_uart_server_task Init Stop
[ble uart server] ble_enable status: 0
[ble uart server] add characters_and_descriptors cbk service:1, srv_handle:14, uuid_len:2, status:0, uuid:18 0d 
[ble uart server] TX characters:1 srv_handle:14 
[ble uart server] characters_uuid:2a 37
[ble uart server] ccc_uuid:2a 37
[ble uart server] start service: 1 service_hdl: 1 status: 0
[ble uart server] add character cbk service:1 service_hdl: 14 char_hdl: 15 char_val_hdl: 16 uuid_len: 2 
uuid:2a 37 [ble uart server] status:0 indicate_handle:16
[ble uart server] service:1 service_hdl: 14 desc_hdl: 17 uuid_len: 2 
uuid:29 02 [ble uart server] status:0
[ble uart server] start service: 1 service_hdl: 5 status: 0
[ble uart server] start service cbk , start adv
[ble uart adv] ble_uart_set_adv_data adv_handle 1, len:24, data:?
[ble uart adv] ble_uart_start_adv adv_id 1
[ACore] ble set adv param min_interval:0x30, max interval:0x60, adv_type:0, duration:0

[ACore] gap ble start adv in, adv_id:1
[ble uart server] start service: 1 service_hdl: 14 status: 0
[ble uart server] adv enable cbk adv_id:1 status:1
```



### 蓝牙协议栈启动/liteos启动
```
SSB|Debug uart init succ
SSB|System Power On
SSB|System Reboot cause:0xf0f0, total reboot count:0, exception reboot count:0
SSB|actual mempool size=134464, free size=19856 max_size=154320
SSB|sizeof(conn_mem_stru *)=4 sizeof(conn_mem_stru)=20
SSB|mem manage cost size 32
SSB|g_flash_mode = 0
\sBS25-ssb-codeloader?賊eSSB|code finish codeloader
SSB|mode flag is ffffffff
SSB|sec check.
SSB|jump_to_cimage :2, addr:0x10000
[osal_msg_queue_create:25]:qName:dfx_msg qID=0x0 
[APPS]Read customized nv success!cpu 0 entering scheduler
[1J[11H[31mWelcome to Test Suite
[osal_msg_queue_create:25]:qName:test_suite_queue qID=0x2 
[34m
Enter Command >>> APP|version: BS25 1.10.113
[ACore] ble enable cbk in, event:9
```
