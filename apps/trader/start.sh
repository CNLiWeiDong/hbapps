#!/bin/bash

systemctl stop trader.service
#提示“请输入密码”并等待30秒，把用户的输入保存入变量pwd中，输入内容隐藏
read -t 30 -s -p "请输入私钥解密密码:" pwd
echo -e "\n"
systemctl start trader.service 
pwd=''