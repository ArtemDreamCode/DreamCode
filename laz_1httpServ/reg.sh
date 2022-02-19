#!/usr/bin/expect -f
set timeout 10
set user "admin"
set password "admin"

set mod [lindex $argv 0]
set name [lindex $argv 1]
set mac [lindex $argv 2]

#./reg.sh reg NAME MAC (registr dev)
#./reg.sh del MAC (unregistr dev)

#known host NAME MAC
#no known host MAC

spawn telnet 192.168.1.1
expect {
    "?ogin*" {
        send "$user\n"
        exp_continue
    }
    "?assword*" {
        send "$password\n"
        exp_continue
    }
    "(config)> " {
	if { $mod == "reg" } {
	send "know host $name $mac\n"
        } elseif { $mod == "del" } {
        send "no know host $mac\n"
        }
        send "exit\n"
        send "exit\n"
        interact
        exit 0;
    }
}
exit 1
