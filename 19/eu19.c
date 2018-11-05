#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

MODULE_LICENSE("GPL");
static const char *id = "BitsCruncher";

unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	struct iphdr *ip_header = NULL;
	unsigned int len = 0;

	if (!skb) {
		return NF_ACCEPT;
	}
	ip_header = ip_hdr(skb);
	
	if (!ip_header) {
		return NF_ACCEPT;
	}
	
	len = htons(ip_header->tot_len);
	if (strnstr((char*) ip_header, id, len)) {
		pr_info("Found %s", id);
	}
	return NF_ACCEPT;
}

static struct nf_hook_ops nfho = 
	{
	.hook = hook_func, 
	.hooknum = NF_INET_LOCAL_IN, 
	.pf = NFPROTO_IPV4, 
	.priority = NF_IP_PRI_FILTER
	};

static int __init mynetfilter_init(void)
{
	return nf_register_net_hook(&init_net, &nfho);
}

static void __exit mynetfilter_exit(void)
{
	nf_unregister_net_hook(&init_net, &nfho);
}

module_init(mynetfilter_init);
module_exit(mynetfilter_exit);                          
