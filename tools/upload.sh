#!/bin/bash

# 目标主机的 IP 地址
TARGET_IP="192.168.137.171"
# 目标主机的用户名
TARGET_USER="root"
# 目标主机的密码
PASSWORD="CherryYoudao"
# 源文件路径
SOURCE_FILE="/home/ubuntu/dsProject/ds"
# 目标文件路径
TARGET_DIR="/userdisk/Favorite"

spawn scp -r /home/ubuntu/dsProject/ds "$TARGET_USER@$TARGET_IP":"$TARGET_DIR"
expect "password:"
send "$PASSWORD"
