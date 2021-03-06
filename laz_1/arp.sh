#!/usr/bin/expect -f
set timeout 10
set user "admin"
set password "admin"
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
        send "show\n"
        exp_continue
    }
    "(show)> " {
        send "ip hotspot\n"
        send "exit\n"
        send "exit\n"
        interact
        exit 0;
    }
}
exit 1
