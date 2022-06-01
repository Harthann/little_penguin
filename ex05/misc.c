// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for  */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/miscdevice.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

static char *message = "nieyraud";
static int message_length = sizeof(message);


static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, message,
			strlen(message));
}

static ssize_t misc_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char specified_msg[count];
	ssize_t retval = -EINVAL;

	if (count != message_length)
		return retval;

	retval = simple_write_to_buffer(specified_msg, count, f_pos, buf, count);
	if (retval < 0)
		return retval;

	pr_info("{%s} {%s}\n", message, specified_msg);
	retval = strncmp(message, specified_msg, count) ? -EINVAL : count;
	return retval;
}

const struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice misc_example = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &misc_fops,
};

static int __init misc_start(void)
{
	pr_info("Loading misc module with minor: {%i}...\n", misc_example.minor);
	return misc_register(&misc_example);
}

static void __exit misc_end(void)
{
	pr_info("Goodbye Mr.\n");
	misc_deregister(&misc_example);
}

module_init(misc_start);
module_exit(misc_end);
