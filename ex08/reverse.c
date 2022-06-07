// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>


// Dont have a license, LOL
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");


static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

char str[PAGE_SIZE + 1];
char *tmp;

static int __init myfd_init(void)
{
	tmp = NULL;

	memset(str, 0, sizeof(str));
	pr_info("Creating reversing module\n");
	return misc_register(&myfd_device);
}

static void __exit myfd_cleanup(void)
{
	kfree(tmp);
	misc_deregister(&myfd_device);
	pr_info("Removing reversing module\n");
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	int	t, i;

	/***************
	 * Malloc like a boss
	 ***************/
	kfree(tmp);
	tmp = kmalloc(sizeof(char) * PAGE_SIZE * 2, GFP_KERNEL);
	pr_info("{%s} {%lu}\n", str, PAGE_SIZE);
	if (!tmp)
		return -ENOMEM;
	for (t = strlen(str) - 1, i = 0; t >= 0; t--, i++)
		tmp[i] = str[t];
	tmp[i] = 0x0;
	return simple_read_from_buffer(user, size, offs, tmp, i);
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;

	pr_info("{%lu} {%lu}\n", size, PAGE_SIZE);
	res = simple_write_to_buffer(str, PAGE_SIZE, offs, user, size);
	if (res)
		str[res] = '\0';
	return size;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
