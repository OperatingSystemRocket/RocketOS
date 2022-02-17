import os
import struct

ram_disk_file_dir = "./ramdisk_files/"

with open("ramdisk.img", "wb") as ramdisk:
    for filename in os.listdir(ram_disk_file_dir):
        with open(ram_disk_file_dir + filename, "rb") as file:
            data = file.read()
            ramdisk.write(struct.pack('<i', len(data)))
            ramdisk.write(bytes(filename, 'utf-8'))
            zero_buffer = ('\0'*(100-len(filename)))
            ramdisk.write(bytes(zero_buffer, 'utf-8'))
            reserved_space = ('\0'*24)
            ramdisk.write(bytes(reserved_space, 'utf-8'))
            ramdisk.write(data)
        print(filename)
    ramdisk.write(struct.pack('<i', 0)) #add null file
