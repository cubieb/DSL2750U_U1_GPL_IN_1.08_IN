#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x89e24b9c, "struct_module" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xba7921dc, "zlib_inflateEnd" },
	{ 0xe1b7029c, "print_tainted" },
	{ 0xe1c442fa, "LzmaDecode" },
	{ 0x1b7d4074, "printk" },
	{ 0xce5ac24f, "zlib_inflate_workspacesize" },
	{ 0x6b60eef6, "zlib_inflate" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x3b8be29d, "zlib_inflateInit2" },
	{ 0x37a0cba, "kfree" },
	{ 0xf56a3962, "zlib_inflateReset" },
	{ 0xe3d7679c, "LzmaDecodeProperties" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=unlzma";


MODULE_INFO(srcversion, "AA8EEA19B66153D6BF9DC81");
