#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshay Gopal");
MODULE_DESCRIPTION("Automotive Sensor Simulator");

#define DEVICE_NAME  "mydevice"
#define CLASS_NAME   "myclass"
#define BUFFER_SIZE  512

static int    major_number;
static struct class*  mydevice_class  = NULL;
static struct device* mydevice_device = NULL;
static char   message[BUFFER_SIZE]    = {0};
static int    message_size            = 0;
static char   last_command[64]        = "get_all";

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

static void generate_sensor_data(void)
{
    u8 raw1, raw2, raw3;

    get_random_bytes(&raw1, 1);
    get_random_bytes(&raw2, 1);
    get_random_bytes(&raw3, 1);

    int temp    = 20 + (raw1 % 60);
    int speed   = raw2 % 180;
    int battery = 10 + (raw3 % 90);

    const char *door           = (raw1 % 2 == 0) ? "Closed" : "Open";
    const char *temp_status    = (temp > 70)      ? "WARNING" : "Normal";
    const char *speed_status   = (speed > 120)    ? "WARNING" : "Normal";
    const char *battery_status = (battery < 20)   ? "WARNING" : "Normal";
    const char *door_status    = (raw1 % 2 == 0)  ? "Normal"  : "WARNING";

    if (strncmp(last_command, "get_temperature", 15) == 0) {
        message_size = snprintf(message, BUFFER_SIZE,
            "Temperature : %d C [%s]\n",
            temp, temp_status);

    } else if (strncmp(last_command, "get_speed", 9) == 0) {
        message_size = snprintf(message, BUFFER_SIZE,
            "Speed : %d km/h [%s]\n",
            speed, speed_status);

    } else if (strncmp(last_command, "get_battery", 11) == 0) {
        message_size = snprintf(message, BUFFER_SIZE,
            "Battery : %d %% [%s]\n",
            battery, battery_status);

    } else if (strncmp(last_command, "get_door", 8) == 0) {
        message_size = snprintf(message, BUFFER_SIZE,
            "Door : %s [%s]\n",
            door, door_status);

    } else {
        message_size = snprintf(message, BUFFER_SIZE,
            "Temperature : %d C    [%s]\n"
            "Speed       : %d km/h [%s]\n"
            "Battery     : %d %%   [%s]\n"
            "Door        : %s     [%s]\n",
            temp,    temp_status,
            speed,   speed_status,
            battery, battery_status,
            door,    door_status);
    }
}

static int __init mydevice_init(void)
{
    printk(KERN_INFO "mydevice: Initializing driver\n");

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "mydevice: Failed to register\n");
        return major_number;
    }

    mydevice_class = class_create(CLASS_NAME);
    if (IS_ERR(mydevice_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(mydevice_class);
    }

    mydevice_device = device_create(mydevice_class, NULL,
                      MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mydevice_device)) {
        class_destroy(mydevice_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(mydevice_device);
    }

    printk(KERN_INFO "mydevice: Device ready at /dev/%s\n", DEVICE_NAME);
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
    generate_sensor_data();
    printk(KERN_INFO "mydevice: Opened — command: %s\n", last_command);
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
    size_t to_send;
    unsigned long not_copied;

    if (*offset >= message_size)
        return 0;

    to_send = min(len, (size_t)(message_size - *offset));

    not_copied = copy_to_user(buffer, message, to_send);
    if (not_copied != 0) {
        printk(KERN_ALERT "mydevice: copy_to_user failed\n");
        return -EFAULT;
    }

    *offset = message_size;
    printk(KERN_INFO "mydevice: Sent %zu bytes\n", to_send);
    return to_send;
}

static ssize_t mydevice_write(struct file *filep, const char *buffer,
                               size_t len, loff_t *offset)
{
    size_t cmd_len;
    unsigned long not_copied;

    cmd_len = min(len, (size_t)63);

    not_copied = copy_from_user(last_command, buffer, cmd_len);
    if (not_copied != 0) {
        printk(KERN_ALERT "mydevice: copy_from_user failed\n");
        return -EFAULT;
    }

    last_command[cmd_len] = '\0';

    if (last_command[cmd_len - 1] == '\n')
        last_command[cmd_len - 1] = '\0';

    printk(KERN_INFO "mydevice: Received command: %s\n", last_command);
    return len;
}

module_init(mydevice_init);
module_exit(mydevice_exit);
