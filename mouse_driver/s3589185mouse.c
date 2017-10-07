/*s3589185mouse.c*/

/* Driver for a virtual mouse controlled by a user program */

#include <linux/sysfs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>

MODULE_AUTHOR("JOSHUA HANSEN");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "s3589185mouse"

struct input_dev *mouse_input_dev;
static struct platform_device *mouse_dev;

static ssize_t write_mouse(struct device *dev, struct device_attribute *attr, const char *buffer, size_t count)
{
	int x,y;
	printk("%s: Writing to device\n", DEVICE_NAME);
	sscanf(buffer, "%d%d", &x, &y);

	printk("%s: X = %d", DEVICE_NAME, x);
	printk("%s: y = %d", DEVICE_NAME, y);

	input_report_rel(mouse_input_dev, REL_X, x);
	input_report_rel(mouse_input_dev, REL_Y, y);
	input_sync(mouse_input_dev);

	return count;
}

DEVICE_ATTR(coordinates, 0644, NULL, write_mouse);

static struct attribute *mouse_attrs[] = {
	&dev_attr_coordinates.attr, 
	NULL
};

static struct attribute_group mouse_attr_group = {
	.attrs = mouse_attrs
};

static int __init mouse_init(void)
{
	printk("%s: Starting device initialization\n", DEVICE_NAME);

	mouse_dev = platform_device_register_simple("mouse", -1, NULL, 0);;
	if(IS_ERR(mouse_dev))
	{
		PTR_ERR(mouse_dev);
		printk("%s: mouse_init: error\n", DEVICE_NAME);
	}

	sysfs_create_group(&mouse_dev->dev.kobj, &mouse_attr_group);

	mouse_input_dev = input_allocate_device();
	if(!mouse_input_dev)
	{
		printk("%s: Bad input_alloc_device()\n", DEVICE_NAME);
	}

	__set_bit(EV_REL, mouse_input_dev->evbit);
	__set_bit(REL_X, mouse_input_dev->relbit);
	__set_bit(REL_Y, mouse_input_dev->relbit);

	mouse_input_dev->name = DEVICE_NAME;
	input_register_device(mouse_input_dev);

	printk("%s: Virtual Mouse Driver Initialized.\n", DEVICE_NAME);
	return 0;
}

static void __exit mouse_exit(void)
{
	printk("%s: Unregistering device\n", DEVICE_NAME);

	input_unregister_device(mouse_input_dev);

	sysfs_remove_group(&mouse_dev->dev.kobj, &mouse_attr_group);

	platform_device_unregister(mouse_dev);

	printk("%s: Device removed successfully\n", DEVICE_NAME);

	return;
}

module_init(mouse_init);
module_exit(mouse_exit);
