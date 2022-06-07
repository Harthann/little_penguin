// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for  */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/miscdevice.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");


static char *message = "nieyraud";
static char message_length = sizeof(message);

/* IDFILE
 * io read/write for id file
 */
static ssize_t id_file_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, message, strlen(message));
}

static ssize_t id_file_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char specified_msg[count];
	ssize_t retval = -EINVAL;

	if (count != message_length)
		return retval;

	retval = simple_write_to_buffer(specified_msg, count, f_pos, buf, count);
	if (retval < 0)
		return retval;

	retval = strncmp(message, specified_msg, count) ? -EINVAL : count;
	return retval;
}

const struct file_operations id_file_fops = {
	.owner = THIS_MODULE,
	.read = id_file_read,
	.write = id_file_write,
};

/* jiffies file io */
static ssize_t jiffies_file_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char buffer[64];

	sprintf(buffer, "%lu\n", jiffies);
	return simple_read_from_buffer(buf, count, f_pos, buffer, strlen(buffer));
}

const struct file_operations jiffies_file_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_file_read,
};


/* foo file io */

static char foo_buffer[PAGE_SIZE];
static ssize_t foo_len;

static ssize_t foo_file_read(struct file *flip, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, foo_buffer, foo_len);
}


static ssize_t foo_file_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	if (count > PAGE_SIZE)
		return -EMSGSIZE;
	memcpy(foo_buffer, buf, count);
	foo_len = count;
	return count;
}

const struct file_operations foo_file_fops = {
	.owner = THIS_MODULE,
	.read = foo_file_read,
	.write = foo_file_write,
};


static struct dentry *dir_fortytwo;


static int __init debug_start(void)
{
	int permission = 0666;

	pr_info("Loading debug module\n");

	dir_fortytwo = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(dir_fortytwo))
		pr_err("Error occured during directory creation\n");

	if (IS_ERR(debugfs_create_file("id", permission, dir_fortytwo, NULL, &id_file_fops)))
		pr_err("Error occured during id creation\n");

	permission = 0444;
	if (IS_ERR(debugfs_create_file("jiffies", permission, dir_fortytwo, NULL, &jiffies_file_fops)))
		pr_err("Error occured during jiffies creation\n");

	permission = 0655;
	if (IS_ERR(debugfs_create_file("foo", permission, dir_fortytwo, NULL, &foo_file_fops)))
		pr_err("Error occured during foo creation\n");
	foo_len = 0;
	return 0;
}

static void __exit debug_end(void)
{
	debugfs_remove_recursive(dir_fortytwo);
	pr_info("Goodbye Mr.\n");
}

module_init(debug_start);
module_exit(debug_end);
