#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include "mux33xx.h"
#include "mux.h"

#define DRV_CLASS    "ht_device"

#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))
#define GPIO_1 GPIO_TO_PIN(0, 15)

void __iomem *base = NULL;
static dev_t ht_devno  = 0;
static struct cdev *ht_cdev  = NULL;
static struct class *ht_class = NULL;

static void set_pin_mux(const int pin_reg_offset, const int mode)
{
    __raw_writew(mode, base + pin_reg_offset);
}

static void ht_device_release(struct device *dev)
{
}

static struct platform_device ht_device = {
    .name        = "ht_dev",
    .id          = -1,
    .dev.release = ht_device_release,
};

static void ht_cdev_teardown(void)
{
    if (ht_class) {
        device_destroy(ht_class, ht_devno);
        class_destroy(ht_class);
    }
    
    if (ht_devno) {
        unregister_chrdev_region(ht_devno, 1);
        if (ht_cdev) {
            cdev_del(ht_cdev);
        }
    }

    return;
}

static int ht_device_setup(void)
{
    int status;

    status = alloc_chrdev_region(&ht_devno, 0, 1, ht_device.name);
    if (status) {
        goto err;
    }

    /* create a new char device */
    ht_cdev = cdev_alloc();
    if (ht_cdev == NULL) {
        status = -ENOMEM;
        goto err;
    }
    ht_cdev->owner = THIS_MODULE;
    status = cdev_add(ht_cdev, ht_devno, 1);
    if (status) {
        goto err;
    }

    /* register the class */
    ht_class = class_create(THIS_MODULE, DRV_CLASS);
    if (IS_ERR(ht_class)) {
        status = -EFAULT;
        goto err;
    }
    if (IS_ERR(ht_class)) {
        status = -EFAULT;
        goto err;
    }
    device_create(ht_class, NULL, ht_devno,
                  NULL, ht_device.name);
    printk(KERN_INFO "Major: %u; minor: %u\n",
           MAJOR(ht_devno),
           MINOR(ht_devno));

    return 0;
err:
    ht_cdev_teardown();
    return status;
}

static int gpio_pluse(const int gpio_num)
{
    char gpio_label[32] = { '\0' };

    set_pin_mux(AM33XX_CONTROL_PADCONF_UART1_TXD_OFFSET, OMAP_MUX_MODE7);

    sprintf(gpio_label, "gpio_%d", gpio_num);
    if (gpio_request(gpio_num, gpio_label) < 0) {
        printk(KERN_ERR "******* Failed to request %s\n", gpio_label);
        return -1;
    }

    gpio_direction_output(gpio_num, 1);
    mdelay(400);
    gpio_direction_output(gpio_num, 0);
    mdelay(25);
    gpio_direction_output(gpio_num, 1);
    mdelay(24);

    gpio_free(gpio_num);
    set_pin_mux(AM33XX_CONTROL_PADCONF_UART1_TXD_OFFSET, OMAP_MUX_MODE0 | AM33XX_PULL_ENBL); /* uart1_txd.uart1_txd */
    
    return 0;
}

static ssize_t ht_device_pluse(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf, size_t count)
{
    gpio_pluse(GPIO_1);
    
    return count;
}

static DEVICE_ATTR(pluse, 0666, NULL, ht_device_pluse);

static const struct attribute *ht_dev_attrs[] = {
    &dev_attr_pluse.attr,
    NULL,
};

static const struct attribute_group ht_dev_attr_group = {
    .attrs = (struct attribute **)ht_dev_attrs,
};

static int __init gpio_plush_init(void)
{
    int status;
    
    base = ioremap(AM33XX_CONTROL_PADCONF_MUX_PBASE,
                   AM33XX_CONTROL_PADCONF_MUX_SIZE);

    status = platform_device_register(&ht_device);
    if (status) {
        goto fail_no_dev;
    }
    
    status = sysfs_create_group(&(ht_device.dev.kobj),
                                &ht_dev_attr_group);
    if (status) {
        goto fail_no_sysfs;
    }
    
    ht_device_setup();

    return 0;
    
fail_no_sysfs:
    platform_device_unregister(&ht_device);
fail_no_dev:
    iounmap(base);

    return status;
}

static void __exit gpio_plush_exit(void)
{
    ht_cdev_teardown();
    
    if (base)
        iounmap(base);

    platform_device_unregister(&ht_device);
    sysfs_remove_group(&(ht_device.dev.kobj), &ht_dev_attr_group);
}

module_init(gpio_plush_init);
module_exit(gpio_plush_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jakebo Huang");
