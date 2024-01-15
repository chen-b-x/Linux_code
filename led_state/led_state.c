#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/regulator/consumer.h>
/*
led_state{
	compatible = "worldchip,led_state";
	twi_id = <6>;
	pmu_addr = <0x34>;
	pmu_status2_addr = <0x01>;
	led_R = <&pio PE 8 GPIO_ACTIVE_HIGH>;
	led_G = <&pio PE 5 GPIO_ACTIVE_HIGH>;
	led_B = <&pio PE 10 GPIO_ACTIVE_HIGH>;
}
*/

struct led_state {
	struct device *dev;
	struct device_node *np;
	
	int gpio_Red_port;
	int gpio_Green_port;
	int gpio_Blue_port;

	struct regulator *led_power;
	unsigned int led_power_vol;
};
/*
static int LED_set_value(struct device *dev,char *mode,int value)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	switch (*mode) {
    case 'R':
		if (gpio_is_valid(led_state->gpio_Red_port)) {
			gpio_direction_output(led_state->gpio_Red_port,value);
		}
        return 0;

    case 'G':
		if (gpio_is_valid(led_state->gpio_Green_port)) {
			gpio_direction_output(led_state->gpio_Green_port,value);
		}
        return 1;

	case 'B':
		if (gpio_is_valid(led_state->gpio_Blue_port)) {
			gpio_direction_output(led_state->gpio_Blue_port,value);
		}
		return 2;
    default:
        return -1;
	}
}

static int LED_get_value(struct device *dev,char *mode)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	switch (*mode) {
    case 'R':
		if (gpio_is_valid(led_state->gpio_Red_port)) {
			return gpio_get_value(led_state->gpio_Red_port);
		}
        return 0;

    case 'G':
		if (gpio_is_valid(led_state->gpio_Green_port)) {
			return gpio_get_value(led_state->gpio_Green_port);
		}

	case 'B':
		if (gpio_is_valid(led_state->gpio_Blue_port)) {
			return gpio_get_value(led_state->gpio_Blue_port);
		}
    default:
        return -1;
	}
}
*/
static ssize_t LED_Red_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	return snprintf(buf, PAGE_SIZE, "%d\n", gpio_is_valid(led_state->gpio_Red_port) ? gpio_get_value(led_state->gpio_Red_port) : -1);
}


static ssize_t LED_Red_store(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	gpio_direction_output(led_state->gpio_Red_port,simple_strtol(buf, NULL, 10));
	return size;
}

static ssize_t LED_Green_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	return gpio_is_valid(led_state->gpio_Green_port) ? gpio_get_value(led_state->gpio_Green_port) : -1;
}


static ssize_t LED_Green_store(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	gpio_direction_output(led_state->gpio_Green_port,simple_strtol(buf, NULL, 10));
	return size;
}

static ssize_t LED_Blue_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	return gpio_is_valid(led_state->gpio_Blue_port) ? gpio_get_value(led_state->gpio_Blue_port) : -1;
}


static ssize_t LED_Blue_store(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
	struct platform_device *pdev = container_of(dev,struct platform_device, dev);
	struct led_state *led_state = platform_get_drvdata(pdev);
	gpio_direction_output(led_state->gpio_Blue_port,simple_strtol(buf, NULL, 10));
	return size;
}

static DEVICE_ATTR(LED_Red, S_IRUGO | S_IWUSR, LED_Red_show, LED_Red_store);
static DEVICE_ATTR(LED_Green, S_IRUGO | S_IWUSR, LED_Green_show, LED_Green_store);
static DEVICE_ATTR(LED_Blue, S_IRUGO | S_IWUSR, LED_Blue_show, LED_Blue_store);


static int led_get_gpio(struct led_state *led_state)
{
	int ret = -1;

	led_state->gpio_Red_port = of_get_named_gpio(led_state->np, "led_Red", 0);
	led_state->gpio_Green_port = of_get_named_gpio(led_state->np, "led_Green", 0);
	led_state->gpio_Blue_port = of_get_named_gpio(led_state->np, "led_Blue", 0);
	ret = gpio_request(led_state->gpio_Red_port, "gpio_Red_port");
	if (ret < 0) {
		dev_err(led_state->dev,"get gpio_Red_port fail\n");
		goto err_gpio_Red_port;
	}
	dev_dbg(led_state->dev,"gpio_Red_port:%d\n",led_state->gpio_Red_port);
	gpio_direction_output(led_state->gpio_Red_port, 0);

	ret = gpio_request(led_state->gpio_Green_port, "gpio_Green_port");
	if (ret < 0) {
		dev_err(led_state->dev,"get gpio_Green_port fail\n");
		goto err_gpio_Green_port;
	}
	dev_dbg(led_state->dev,"gpio_Green_port:%d\n",led_state->gpio_Green_port);
	gpio_direction_output(led_state->gpio_Green_port, 0);

	ret = gpio_request(led_state->gpio_Blue_port, "gpio_Blue_port");
	if (ret < 0) {
		dev_err(led_state->dev,"get gpio_Blue_port fail\n");
		goto err_gpio_Blue_port;
	}
	dev_dbg(led_state->dev,"gpio_Blue_port:%d\n",led_state->gpio_Blue_port);
	gpio_direction_output(led_state->gpio_Blue_port, 0);

	return ret;

err_gpio_Blue_port:
	if (gpio_is_valid(led_state->gpio_Blue_port))
		gpio_free(led_state->gpio_Blue_port);
err_gpio_Green_port:
	if (gpio_is_valid(led_state->gpio_Green_port))
		gpio_free(led_state->gpio_Green_port);
err_gpio_Red_port:
	if (gpio_is_valid(led_state->gpio_Red_port))
		gpio_free(led_state->gpio_Red_port);
	return ret;
}


static int led_get_regulator(struct led_state *led_state)
{
	int ret = -1;
	led_state->led_power = regulator_get(led_state->dev, "led_state");
	if (IS_ERR_OR_NULL(led_state->led_power)) {
		dev_err(led_state->dev,"led_state->led_power get fail!\n");
		return ret;
	}
	ret = of_property_read_u32(led_state->np, "led_power_vol",&led_state->led_power_vol);
	if (ret < 0){
		dev_err(led_state->dev,"led_state->led_power_vol get fail!\n");
		return ret;
	}
	if (!IS_ERR_OR_NULL(led_state->led_power))
		regulator_set_voltage(led_state->led_power,(int)(led_state->led_power_vol)*1000,(int)(led_state->led_power_vol)*1000);
	return 0;
}

static int led_state_probe(struct platform_device *pdev)
{
	struct led_state *led_state;
	struct device *dev = &pdev->dev;
	int ret;

	dev_dbg(&pdev->dev,"led_state driver probe\n");

	led_state = devm_kzalloc(dev, sizeof(struct led_state), GFP_KERNEL);
	if (!led_state)
		return -ENOMEM;

	led_state->np = dev->of_node;
	platform_set_drvdata(pdev, led_state);
	led_state->dev = dev;

	ret = led_get_gpio(led_state);
	if (ret < 0){
		dev_err(led_state->dev,"led_get_gpio fail check dts gpio\n");
		goto probe_fail;
	}

	ret = led_get_regulator(led_state);
	if (ret < 0){
		dev_err(led_state->dev,"led_get_regulator fail check dts gpio\n");
		goto put_gpio_port;
	} else {
		if (0 != regulator_enable(led_state->led_power)){
			dev_err(led_state->dev,"regulator_enable error!\n");
			goto put_regulator;
		}
	}
	dev_dbg(led_state->dev,"led get regulator success!\n");

	device_create_file(&pdev->dev, &dev_attr_LED_Red);
	device_create_file(&pdev->dev, &dev_attr_LED_Green);
	device_create_file(&pdev->dev, &dev_attr_LED_Blue);

	return 0;

put_regulator:
	regulator_put(led_state->led_power);
put_gpio_port:
	if (gpio_is_valid(led_state->gpio_Blue_port))
		gpio_free(led_state->gpio_Blue_port);
	if (gpio_is_valid(led_state->gpio_Green_port))
		gpio_free(led_state->gpio_Green_port);
	if (gpio_is_valid(led_state->gpio_Red_port))
		gpio_free(led_state->gpio_Red_port);
probe_fail:
	return -1;
}

static int led_state_remove(struct platform_device *pdev)
{
	struct led_state *led_state = platform_get_drvdata(pdev);

	device_remove_file(&pdev->dev, &dev_attr_LED_Red);
	device_remove_file(&pdev->dev, &dev_attr_LED_Green);
	device_remove_file(&pdev->dev, &dev_attr_LED_Blue);

	regulator_disable(led_state->led_power);
	regulator_put(led_state->led_power);

	if (gpio_is_valid(led_state->gpio_Blue_port))
		gpio_free(led_state->gpio_Blue_port);

	if (gpio_is_valid(led_state->gpio_Green_port))
		gpio_free(led_state->gpio_Green_port);

	if (gpio_is_valid(led_state->gpio_Red_port))
		gpio_free(led_state->gpio_Red_port);

	return 0;
}

static const struct of_device_id led_state_dt_ids[] = {
	{.compatible = "worldchip,led_state"},
	{},
};

static struct platform_driver led_state_driver = {
	.probe		= led_state_probe,
	.remove		= led_state_remove,
	.driver		= {
		.name	= "led_state",
		.owner	= THIS_MODULE,
/*
#if IS_ENABLED(CONFIG_PM)
		.pm	= SUNXI_LED_PM_OPS,
#endif
*/
		.of_match_table = led_state_dt_ids,
	},
};

module_platform_driver(led_state_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Chenbx <de-07@worldchip.cn>");
MODULE_DESCRIPTION("worldchip led_state driver");
