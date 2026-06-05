#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshay Gopal");
MODULE_DESCRIPTION("Automotive Sensor Simulator");

#define DEVICE_NAME "mydevice"
#define CLASS_NAME  "myclass"

static int    major_number;
static struct class*  mydevice_class  = NULL;
static struct device* mydevice_device = NULL;

static int     mydevice_open(struct inode*, struct file*);
static int     mydevice_release(struct inode*, struct file*);
static ssize_t mydevice_read(struct file*, char*, size_t, loff_t*);
static ssize_t mydevice_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = mydevice_open,
    .release = mydevice_release,
    .read    = mydevice_read,
    .write   = mydevice_write,
};

static int __init mydevice_init(void)
{
    printk(KERN_INFO "mydevice: Initializing driver\n");

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "mydevice: Failed to register\n");
        return major_number;
    }
    printk(KERN_INFO "mydevice: Registered with major number %d\n", major_number);

    mydevice_class = class_create(CLASS_NAME);
    if (IS_ERR(mydevice_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "mydevice: Failed to create class\n");
        return PTR_ERR(mydevice_class);
    }

    mydevice_device = device_create(mydevice_class, NULL,
                      MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mydevice_device)) {
        class_destroy(mydevice_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "mydevice: Failed to create device\n");
        return PTR_ERR(mydevice_device);
    }

    printk(KERN_INFO "mydevice: Device created at /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit mydevice_exit(void)
{
    device_destroy(mydevice_class, MKDEV(major_number, 0));
    class_destroy(mydevice_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "mydevice: Driver unloaded cleanly\n");
}

static int mydevice_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "mydevice: Device opened\n");
    return 0;
}

static int mydevice_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "mydevice: Device closed\n");
    return 0;
}

static ssize_t mydevice_read(struct file *filep, char *buffer,
                              size_t len, loff_t *offset)
{
    printk(KERN_INFO "mydevice: Someone tried to read\n");
    return 0;
}

static ssize_t mydevice_write(struct file *filep, const char *buffer,
                               size_t len, loff_t *offset)
{
    printk(KERN_INFO "mydevice: Someone tried to write\n");
    return len;
}

module_init(mydevice_init);
module_exit(mydevice_exit);
