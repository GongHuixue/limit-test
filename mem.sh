#! /bin/sh 

echo "occupy memory" 
if [ ! -d "/data/memory"]; then 
	mkdir /data/memory
fi

mount -t tmpfs -o size=1024M tmpfs /data/memory
dd if=/dev/zero of=/data/memory/block
#sleep 3600
if true;then
sleep 3600
rm /data/memory/block
umount /data/memory
rmdir /data/memory
fi

