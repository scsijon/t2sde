# --- T2-COPYRIGHT-NOTE-BEGIN ---
# T2 SDE: package/*/palo/stone_mod_palo.sh
# Copyright (C) 2004 - 2022 The T2 SDE Project
# 
# This Copyright note is generated by scripts/Create-CopyPatch,
# more information can be found in the files COPYING and README.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2.
# --- T2-COPYRIGHT-NOTE-END ---
#
# [MAIN] 70 palo PALO Setup
# [SETUP] 90 palo

get_realdev() {
	local dev="$1"
	dev=$(readlink $dev)
	[ "$dev" ] && echo /dev/${dev##*/} || echo $1
}

install_palo() {
	local dev="$(get_realdev "$1")"
	dev="${dev%[0-9]*}"
	palo -U "$dev" || palo -I "$dev"
}

create_kernel_list() {
	first=1
	echo "--format-as=3"
	local part="${bootdev%[0-9]*}"
	part="${bootdev#$part}"
	for x in `(cd /boot/; ls vmlinuz-*) | sort -r`; do
		[ $first = 0 ] && continue
 		ver=${x/vmlinuz-}
		echo "--commandline=$part/$x initrd=$part/initrd-${ver} root=$rootdev"
		first=0
	done
}

create_palo_conf() {
	create_kernel_list > $rootpath/etc/palo.conf

	gui_message "This is the new $rootpath/etc/palo.conf file:

$(< $rootpath/etc/palo.conf)"
}

device4() {
	local dev="`sed -n "s,\([^ ]*\) $1 .*,\1,p" /etc/fstab | tail -n 1`"
	if [ ! "$dev" ]; then # try the higher dentry
		local try="`dirname $1`"
		dev="`grep \" $try \" /etc/fstab | tail -n 1 | \
		      cut -d ' ' -f 1`"
	fi
	if [ -h "$dev" ]; then
	  echo "/dev/`readlink $dev`"
	else
	  echo $dev
	fi
}

realpath() {
	dir="`dirname $1`"
	file="`basename $1`"
	dir="`dirname $dir`/`readlink $dir`"
	dir="`echo $dir | sed 's,[^/]*/\.\./,,g'`"
	echo $dir/$file
}

main() {
	rootdev="`device4 /`"
	bootdev="`device4 /boot`"

	[ "$rootdev" = "$bootdev" ] && bootpath="/boot"

	if [ ! -f $rootpath/etc/palo.conf ]; then
	  if gui_yesno "PALO does not appear to be configured.
Automatically configure PALO now?"; then
	    create_palo_conf && install_palo
	  fi
	fi

	while

	gui_menu palo 'PALO Setup' \
		"Following settings only for expert use: (default)" "" \
		"Root Device ........... $rootdev" "" \
		"Boot Device ........... $bootdev" "" \
		'' '' \
		"(Re-)Create default $rootpath/etc/palo.conf" 'create_palo_conf' \
		'(Re-)Install PALO into the IPL' 'install_palo' \
		'' '' \
		"Edit $bootpath/palo.conf (Config file)" \
		"gui_edit 'PALO Configuration' $rootpath/etc/palo.conf"
    do : ; done
}
