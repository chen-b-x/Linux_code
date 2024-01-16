#include "virtual_gpio_dev.h"

static void virtual_gpio_dev_release(struct device *dev)
{

}


static struct virt_gpio_plat_data_s virtual_gpio_data =
{
	.base_gpio = 256,
	.gpio_num = 64 
};

static struct platform_device virtual_gpio_dev = {
	.name = "virtual_gpio_dev",
	.id = -1,
	.dev = {
		.platform_data = &virtual_gpio_data,
		.release = virtual_gpio_dev_release,
	}
};

static int __init virturl_gpio_dev_init(void)
{
    int ret = 0;

    ret = platform_device_register(&virtual_gpio_dev);

    printk("%s\n", __FUNCTION__);   
    return ret;
}

static void __exit virturl_gpio_dev_exit(void)
{
    printk("%s\n", __FUNCTION__);
    platform_device_unregister(&virtual_gpio_dev);
}

module_init(virturl_gpio_dev_init);
module_exit(virturl_gpio_dev_exit);
MODULE_DESCRIPTION("Virtual GPIO Platform Device");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chenbx");