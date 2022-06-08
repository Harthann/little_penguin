// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for  */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>


MODULE_LICENSE("GPL");


static int __init hello_init(void)
{
	pr_info("Hello device\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Goodbye device\n");
}

module_init(hello_init);
module_exit(hello_exit);
