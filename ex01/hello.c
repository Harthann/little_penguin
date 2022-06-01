// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for  */
#include <linux/init.h>         /* Needed for the macros */

MODULE_LICENSE("GPL");

static int __init hello_start(void)
{
	pr_info("Loading hello module...\n");
	pr_info("Hello world\n");
	return 0;
}

static void __exit hello_end(void)
{
	pr_info("Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
