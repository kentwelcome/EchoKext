#!/bin/sh

#  verifykext.sh
#  Echo
#
#  Created by Kent Huang on 2015/5/1.
#  Copyright (c) 2015年 KentHuang. All rights reserved.

show_usage () {
    echo "Usage: sh $0 kext_path";
    exit;
}

change_owner_to_root () {
    echo "[\033[0;32mO\033[0m] Change onwner of $1 to root:wheel";
    sudo chown -R root:wheel $1;
}

change_owner_back () {
    echo "[\033[0;32mO\033[0m] Change onwner of $1 to kent:staff";
    sudo chown -R kent:staff $1;
}

verify_kext () {
    echo "[\033[0;32mO\033[0m] Verifiy kext $1";
    kextutil -nt $1;
}

sh_name=$0;
kext_name=$1;

if [ -d "$kext_name" ]
then
    change_owner_to_root $kext_name;
    verify_kext $kext_name;
    change_owner_back $kext_name;
else
    show_usage $0;
fi

