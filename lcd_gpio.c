#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include "lcd_driver.h"

#define DEVICE_NAME "lcd"
#define CLASS_NAME  "lcd_class"

static int major;
static struct class* lcd_class = NULL;
static struct device* lcd_device = NULL;
static struct cdev lcd_cdev;

static int lcd_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "[LCD] Device opened\n");
    return 0;
}

static ssize_t lcd_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    char kbuf[128];
    int i;

    if (len > sizeof(kbuf) - 1)
        len = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    for (i = 0; i < len && i < LCD_NUM_COLS * LCD_NUM_ROWS; i++) {
        lcd_send_data(kbuf[i]);
    }

    return len;
}

static int lcd_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "[LCD] Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = lcd_open,
    .write = lcd_write,
    .release = lcd_release,
};

static int __init lcd_module_init(void) {
    dev_t dev;

    printk(KERN_INFO "[LCD] Initializing driver...\n");

    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "[LCD] Failed to allocate major number\n");
        return -1;
    }

    major = MAJOR(dev);
    printk(KERN_INFO "[LCD] Registered with major number %d\n", major);

    lcd_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(lcd_class)) {
        unregister_chrdev_region(MKDEV(major, 0), 1);
        return PTR_ERR(lcd_class);
    }

    lcd_device = device_create(lcd_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(lcd_device)) {
        class_destroy(lcd_class);
        unregister_chrdev_region(MKDEV(major, 0), 1);
        return PTR_ERR(lcd_device);
    }

    cdev_init(&lcd_cdev, &fops);
    if (cdev_add(&lcd_cdev, MKDEV(major, 0), 1) == -1) {
        device_destroy(lcd_class, MKDEV(major, 0));
        class_destroy(lcd_class);
        unregister_chrdev_region(MKDEV(major, 0), 1);
        return -1;
    }

    lcd_init();

    printk(KERN_INFO "[LCD] Driver loaded\n");
    return 0;
}

static void __exit lcd_module_exit(void) {
    lcd_clear();

    cdev_del(&lcd_cdev);
    device_destroy(lcd_class, MKDEV(major, 0));
    class_destroy(lcd_class);
    unregister_chrdev_region(MKDEV(major, 0), 1);

    printk(KERN_INFO "[LCD] Driver unloaded\n");
}

module_init(lcd_module_init);
module_exit(lcd_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Character device driver for 16x2 LCD via GPIO");
