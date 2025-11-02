#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0xc083a3d7, "gpiod_set_value" },
	{ 0x9442a1d, "device_destroy" },
	{ 0x73fa698d, "class_destroy" },
	{ 0x55ca8fd0, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc2fa4b73, "cdev_init" },
	{ 0x2f1bda82, "cdev_add" },
	{ 0xec08f28b, "class_create" },
	{ 0x6e192851, "device_create" },
	{ 0x593f9876, "gpio_to_desc" },
	{ 0x7403643e, "gpiod_direction_output" },
	{ 0x2489bfa8, "gpiod_direction_input" },
	{ 0x5dc733aa, "gpiod_get_value" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdcb764ad, "memset" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "021A88E22E9346BE137E6B8");
