import os
import sys
import hashlib
import shutil
import struct

root_path = os.path.abspath(os.path.dirname(__file__))
flash_version = "embed_flash"
fota_bin_load_addr = 0x101000
flash_fota_bin_load_addr = 0x1000
bin_map = {
    "ssb.bin" : 0x00000000,
    "bt.bin" : 0x0002c000,
    "bs25_all_nv.bin" : 0x001fc000,
    "application.bin" : 0x0007c000,
}

fota_map = {
    "ssb_header.bin" : [0x00000000, 0],
    "bt_header.bin" : [0x00000078, 0],
    "bs25_all_nv_header.bin" : [0x000000b4, 0],
    "application_header.bin" : [0x000000f0, 0],
    "ssb.bin" : [0x00001000, 0],
    "bt.bin" : [0x00001000, 0],
    "bs25_all_nv.bin" : [0x00001000, 0],
    "application.bin" : [0x00001000, 0],
}

def create_header_file(source):
    header_path = os.path.join(root_path, source.split('.')[0] + '_header.bin')
    if os.path.exists(header_path):
        os.remove(header_path)
    bin_path = os.path.join(root_path, source)
    with open(header_path, "ab") as header:
        if source == "bs25_all_nv.bin":
            flag = struct.pack("i", 0xff)
        else:
            flag = struct.pack("i", 0x5b)
        value_default = struct.pack("B", 0xff)
        back_addr = struct.pack("i", (fota_map[source][0] + fota_bin_load_addr))
        run_addr = struct.pack("i", bin_map[source])
        bin_size = struct.pack("i", os.path.getsize(bin_path))
        with open(bin_path, "rb") as bin_file:
            sha = hashlib.sha256(bin_file.read())
            bin_file.close()
        if flash_version == "embed_flash":
            version = struct.pack("B", 0x55)
        elif flash_version == "flash":
            version = struct.pack("B", 0xaa)
        header.write(flag)
        header.write(back_addr)
        header.write(run_addr)
        header.write(bin_size)
        header.write(sha.digest())
        for _ in range(4):
            header.write(version)
        for _ in range(8):
            header.write(value_default)
        header.close()

def pack_fota(is_flash_version, bin_root_path=None, out_bin_path=None):
    global root_path
    global flash_version
    global fota_bin_load_addr
    if bin_root_path:
        root_path = bin_root_path
    if is_flash_version:
        flash_version = "flash"
        fota_bin_load_addr = flash_fota_bin_load_addr
        bin_map["bs25_all_nv.bin"] = 0x000fc000
    packet_bin = []
    for i in bin_map.keys():
        bin_path = os.path.join(root_path, i)
        if os.path.exists(bin_path):
            packet_bin.append(i)
    for i in packet_bin:
        bin_path = os.path.join(root_path, i)
        if i == 'ssb.bin':
            fota_map['ssb.bin'][0] = 0x00001000
            fota_map['ssb.bin'][1] = (os.path.getsize(bin_path) // 4096 + 1) * 4096
        elif i == 'bt.bin':
            fota_map['bt.bin'][0] = fota_map['ssb.bin'][0] + fota_map['ssb.bin'][1]
            fota_map['bt.bin'][1] = (os.path.getsize(bin_path) // 4096 + 1) * 4096
        elif i == 'bs25_all_nv.bin':
            fota_map['bs25_all_nv.bin'][0] = fota_map['bt.bin'][0] + fota_map['bt.bin'][1]
            fota_map['bs25_all_nv.bin'][1] = (os.path.getsize(bin_path) // 4096 + 1) * 4096
        elif i == 'application.bin':
            fota_map['application.bin'][0] = fota_map['bs25_all_nv.bin'][0] + fota_map['bs25_all_nv.bin'][1]
            fota_map['application.bin'][1] = (os.path.getsize(bin_path) // 4096 + 1) * 4096
    final_bin = []
    final_offset = []

    for i in packet_bin:
        header = i.split('.')[0] + '_header.bin'
        final_bin.append(i)
        final_bin.append(header)
        create_header_file(i)
        fota_map[header][1] = 0x3c
        final_offset.append(fota_map[i][0])
        final_offset.append(fota_map[header][0])
    if out_bin_path:
        bin_result_path = out_bin_path
    else:
        bin_result_path = os.path.join(root_path, "fota.bin")


    for i in final_bin:
        open(os.path.join(root_path, i), 'rb')
    if os.path.exists((bin_result_path)):
        os.remove(bin_result_path)
    bin_merge = open(bin_result_path, 'ab')
    final_size = 0x1000
    for i in packet_bin:
        final_size += fota_map[i][1]
    offset = 0
    value_default = struct.pack("B", 0xff)
    print("merge")
    print("packet bin list: ", final_bin)
    while offset < final_size:
        if offset in final_offset:
            file_id = final_offset.index(offset)
            open_bin_path = os.path.join(root_path, final_bin[file_id])
            bin_read = open(open_bin_path, 'rb')
            data = bin_read.read()
            bin_merge.write(data)
            offset += os.path.getsize(open_bin_path)
            bin_read.close()
        else:
            bin_merge.write(value_default)
            offset += 1
    bin_merge.close()
    print("success")
    print("fota size: ", final_size)
    for i in packet_bin:
        header = i.split('.')[0] + '_header.bin'
        header_bin_path = os.path.join(root_path, header)
        os.remove(header_bin_path)

if __name__ == '__main__':
    args = len(sys.argv)
    argv = sys.argv
    if(args <= 1):
        print("parma error")
        sys.exit()
    if (argv[1].split('-')[-1] == 'flash'):
        pack_fota(True)
    else:
        pack_fota(False)