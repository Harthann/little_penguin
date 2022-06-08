// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for  */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/usb.h>

MODULE_LICENSE("GPL");



/* table of devices that work with this driver */
static struct usb_device_id skel_table[] = {
	{ .driver_info = 42},
	{ }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, skel_table);

static int skel_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	pr_info("Hello usb\n");
	return 0;
}

static struct usb_driver skel_driver = {
	.name        = "usb_driver",
	.probe       = skel_probe,
	.id_table    = skel_table,
};

static int __init usb_skel_init(void)
{
	int result;

	/* register this driver with the USB subsystem */
	result = usb_register(&skel_driver);
	if (result < 0) {
		pr_info("usb_register failed for the "__FILE__ "driver.Error number %d", result);
		return -1;
	}

	return 0;
}

static void __exit usb_skel_exit(void)
{
	/* deregister this driver with the USB subsystem */
	usb_deregister(&skel_driver);
}

module_init(usb_skel_init);
module_exit(usb_skel_exit);
