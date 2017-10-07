#include <linux/init.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("JOSHUA HANSEN");
MODULE_DESCRIPTION("SIMPLE DRIVER WITH OPEN READ WRITE AND CLOSE");
MODULE_LICENSE("GPL");

struct virtual_device {
	char data[100];		//local buffer of the device
	struct semaphore sem;	//allows other processes to use the device
}myDevice;

struct cdev *mcdev;
int major_num;
int rtrn;

dev_t dev_num;

#define DEVICE_NAME	"s3589185Device"

int device_open(struct inode *inode, struct file *filp)
{
	if(down_interruptible(&myDevice.sem) != 0)
	{
		printk("%s: could bot lock device during open\n", DEVICE_NAME);
		return -1;
	}
	printk("%s: Opened Device\n", DEVICE_NAME);
	return 0;
}

ssize_t device_read(struct file *filp, char *bufferData, size_t bufferCount, loff_t *curOffset)
{
	printk("%s: Reading from device\n", DEVICE_NAME);
	rtrn = copy_to_user(bufferData, myDevice.data, bufferCount);
	return rtrn;
}

ssize_t device_write(struct file *filp, const char *bufferData, size_t bufferCount, loff_t *curOffset)
{
	printk("%s: Writing to device\n", DEVICE_NAME);
	rtrn = copy_from_user(myDevice.data, bufferData, bufferCount);
	return rtrn;
}

int device_close(struct inode *inode, struct file *filp)
{
	up(&myDevice.sem);
	printk("%s: Close Device\n", DEVICE_NAME);
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.write = device_write,
	.read = device_read
};

static int s3589185Device_init(void)
{
	rtrn = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if(rtrn < 0)
	{
		printk("%s: Failed to allocate a MAJOR NUMBER\n", DEVICE_NAME);
		return rtrn;
	}
	major_num = MAJOR(dev_num);
	printk("%s: MAJOR NUMBER is %d", DEVICE_NAME, major_num);
	printk("%s: USE \"mkmod /dev/%s c %d 0\" FOR DEVICE FILE\n", DEVICE_NAME, DEVICE_NAME, major_num);
	mcdev = cdev_alloc();
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;

	rtrn = cdev_add(mcdev, dev_num, 1);
	if(rtrn < 0)
	{
		printk("%s: Unable to add cdev to kernel\n", DEVICE_NAME);
		return rtrn;
	}

	sema_init(&myDevice.sem, 1);
	printk("%s: Diver Loaded Successfully\n", DEVICE_NAME);
	return 0;
}

static void s3589185Device_exit(void)
{
	cdev_del(mcdev);

	unregister_chrdev_region(dev_num, 1);

	printk("%s: Driver Exited Successfully\n", DEVICE_NAME);
	return;
}

module_init(s3589185Device_init);
module_exit(s3589185Device_exit);
