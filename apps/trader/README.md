# 1、 更新系统时间，和docker mysql的系统时间一致。
tzselect
cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
ntpdate us.pool.ntp.org

# 2、配置systemd
文件：trader.service
内容：
[Unit]
Description=trader

[Service]
ExecStart=/root/trader/trader --secret-key-password $pwd
WorkingDirectory=/root/trader/
Restart=always
User=root
#ExecReload=/bin/kill -SIGHUP $MAINPID
#ExecStop=/bin/kill -SIGINT $MAINPID

[Install]
WantedBy=multi-user.target

#  执行启动脚步
start.sh