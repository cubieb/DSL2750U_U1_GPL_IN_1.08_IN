/* libipt_id.c */
/* Shared library add-on to iptables to add ID target support. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_LED.h>

/* Function which prints out usage message. */
static void help(void)
{
    printf(
    "LED target v%s options:\n"
    "  --set-led led_id     Set led to flash when datas forward\n",
    IPTABLES_VERSION);
}
 
// opts结构第一个参数为选项名称，
// 第二个参数为1表示选项名称后还带参数，为0表示选项名称后不带参数
// 第3个参数是标志，表示返回数据的格式，一般都设为0
// 第4个参数表示该选项的索引值
static struct option opts[] = {
                                  { "set-led", 1, 0, '1' },
                                  { 0 }
                              };

/* Initialize the target. */
static void init(struct ipt_entry_target *t, unsigned int *nfcache)
{
// 空函数，不需要预处理
}
 
static void parse_id(const unsigned char *s, struct ipt_led_target_info *info)
{
    unsigned int id;

    printf("led id option %s\n", s);
    
    if (string_to_number(s, 0, 10, &id) != -1) 
    {
        // 注意要将主机序数据转换为网络序
        info->led_id = id;
        printf("led id is %d\n", id);
        return;
    }
    exit_error(PARAMETER_PROBLEM, "Bad LED_ID value %s", s);
}
 
/* Function which parses command options; returns true if it ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
                 const struct ipt_entry *entry,
                 struct ipt_entry_target **target)
{
    struct ipt_led_target_info *ledinfo = (struct ipt_led_target_info *)(*target)->data;
    switch (c) 
    {
        case '1':
            if (*flags)
                exit_error(PARAMETER_PROBLEM, "LED target: Cant specify --set-led twice");
            parse_id(optarg, ledinfo);

            printf("get led id is %d\n", ledinfo->led_id);
            *flags = 1;
            break;
        default:
            return 0;
    }
    return 1;
}
 
static void final_check(unsigned int flags)
{
    if (!flags)
    exit_error(PARAMETER_PROBLEM,
             "LED target: Parameter --set-led is required");
}
 
/* Prints out the targinfo. */
static void print(const struct ipt_ip *ip,
                  const struct ipt_entry_target *target,
                  int numeric)
{
    const struct ipt_led_target_info *ledinfo =
    (const struct ipt_led_target_info *)target->data;
    printf("LED set %d\n", ledinfo->led_id);
}
 
/* Saves the union ipt_targinfo in parsable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
    const struct ipt_led_target_info *ledinfo =
    (const struct ipt_led_target_info *)target->data;
    printf("--set-led 0x%x ", ledinfo->led_id);
}

static
struct iptables_target led
= {
    .name          = "LED",
    .version       = IPTABLES_VERSION,
    .size          = IPT_ALIGN(sizeof(struct ipt_led_target_info)),
    .userspacesize = IPT_ALIGN(sizeof(struct ipt_led_target_info)),
    .help          = &help,
    .init          = &init,
    .parse         = &parse,
    .final_check   = &final_check,
    .print         = &print,
    .save          = &save,
    .extra_opts    = opts
};
 
void _init(void)
{
    register_target(&led);
}

