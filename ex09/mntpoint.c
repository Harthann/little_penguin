// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>	/* Specifically, a module */
#include <linux/init.h>
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <linux/fs.h>	/* Necessary because we use the proc fs */
#include <linux/fs_struct.h>	/* Necessary because we use the proc fs */

MODULE_LICENSE("GPL");

char *procfs_name = "mymounts";

/**
 ***	This structure hold information about the /proc file
 **/
static struct proc_dir_entry *proc_file;
static char buffer[PATH_MAX];

static char *kstrjoin(char *dst, const char *src)
{
	char *tmp;

	if (!dst) {
		tmp = kmalloc(strlen(src) + 1, GFP_KERNEL);
		strcpy(tmp, src);
		return tmp;
	}
	tmp = kmalloc(strlen(dst) + strlen(src) + 1, GFP_KERNEL);

	if (!tmp)
		return NULL;
	memset(tmp, 0, strlen(dst) + strlen(src) + 1);
	strcat(tmp, dst);
	strcat(tmp, src);

	kfree(dst);
	return tmp;
}

ssize_t procfile_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	int		ret;
	char		*str = NULL;
	struct dentry	*curdentry;

	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child) {
		if (curdentry->d_flags & DCACHE_MOUNTED) {
			str = kstrjoin(str, curdentry->d_name.name);
			str = kstrjoin(str, "\t");
			str = kstrjoin(str, dentry_path_raw(curdentry, buffer, sizeof(buffer)));
			str = kstrjoin(str, "\n");
		}
	}
	ret = simple_read_from_buffer(user, size, offs, str, strlen(str));
	kfree(str);
	return ret;
}

static const struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
};

static int __init module_entry(void)
{
	proc_file = proc_create(procfs_name, 0644, NULL, (const struct proc_ops *)&proc_fops);

	if (proc_file == NULL) {
		proc_remove(proc_file);
		pr_alert("Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	pr_info("/proc/%s created\n", procfs_name);

	return 0;
}

static void __exit module_cleanup(void)
{
	proc_remove(proc_file);
	pr_info("/proc/%s removed\n", procfs_name);
}


module_init(module_entry);
module_exit(module_cleanup);
