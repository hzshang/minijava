#! /bin/sh
#
# build.sh
# Copyright (C) 2018 hzshang <hzshang15@gmail.com>
#
# Distributed under terms of the MIT license.
#


./parse_out $1 | gotree draw cyjs --with-node-labels > abs.html
