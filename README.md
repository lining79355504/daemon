# daemon
c programe 


此时你查看一下inittab文件：
# cat /etc/inittab
会发现原来daemontools是使用init的方式来保护自己的：


example :

nohup /usr/local/daemon -r "/home/service/php/bin/php /home/work/lab1/logistics/ams/public/script.php PushConsumer >>/dev/null & " -m 4 -c "ps -ef | grep PushConsumer | wc -l"  >>/dev/null 2>&1 &
