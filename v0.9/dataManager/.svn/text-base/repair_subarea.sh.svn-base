#!/bin/sh


mkfs.ext3 -c -L mmc3 /dev/mmcblk0p3
value=$?
if [ $value -ne 0 ]; then
	exit 1
else
	mount /dev/mmcblk0p3 /nand3
	value=$?
	if [ $value -ne 0 ]; then
		exit 1
	else
		cp -arf /application/data_backup/* /nand3
		value=$?
		if [ $value -ne 0 ]; then
			exit 1
		else
			sync
		fi
	fi
fi

