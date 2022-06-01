cmd_/root/little_penguin/ex01/modules.order := {   echo /root/little_penguin/ex01/hello.ko; :; } | awk '!x[$$0]++' - > /root/little_penguin/ex01/modules.order
