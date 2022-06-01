cmd_/root/little_penguin/ex01/Module.symvers := sed 's/\.ko$$/\.o/' /root/little_penguin/ex01/modules.order | scripts/mod/modpost    -o /root/little_penguin/ex01/Module.symvers -e    -w -T -
