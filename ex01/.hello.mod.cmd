cmd_/root/little_penguin/ex01/hello.mod := echo /root/little_penguin/ex01/hello.o | awk -v RS='( |\n)' '!x[$$0]++' > /root/little_penguin/ex01/hello.mod
