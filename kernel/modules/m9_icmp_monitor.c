#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>

#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/if_ether.h>

#include <net/netns/generic.h>

static unsigned int lkm_net_id;

static unsigned long ipv4_echo_req = 0;
static unsigned long ipv4_echo_rep = 0;

static unsigned long ipv6_echo_req = 0;
static unsigned long ipv6_echo_rep = 0;

struct lkm_netns_data {
    struct nf_hook_ops nf_hops_localout;
    struct nf_hook_ops nf_hops_prerouting;
};

static unsigned int nf_callback(void *priv, struct sk_buff *skb,
				const struct nf_hook_state *state)
{
	struct icmphdr *icmph;
	struct icmp6hdr *icmp6h;
       	struct iphdr *iph;
	struct ipv6hdr *ip6h;

/*	if (!skb || !pskb_may_pull(skb, sizeof(*ip6h))) {
		printk("weird skb?! Drop it!\n");
		return NF_DROP;
	}
	        ip6h = ipv6_hdr(skb);
        if (ip6h->nexthdr == IPPROTO_ICMPV6) {
                printk("recevied ICMPv6 packet! Drop it!\n");
                return NF_DROP;
        }

*/
	if (!skb)
        	 return NF_ACCEPT;

	if (skb->protocol == htons(ETH_P_IP)) {

		iph = ip_hdr(skb);

		if (iph->protocol == IPPROTO_ICMP) {

    icmph = icmp_hdr(skb);

    if (icmph->type == ICMP_ECHO &&
        state->hook == NF_INET_LOCAL_OUT) {

        ipv4_echo_req++;
        printk("IPv4 Echo Request count = %lu\n", ipv4_echo_req);
    }

    else if (icmph->type == ICMP_ECHOREPLY &&
             state->hook == NF_INET_PRE_ROUTING) {

        ipv4_echo_rep++;
        printk("IPv4 Echo Reply count = %lu\n", ipv4_echo_rep);
    }
}
	}

	else if (skb->protocol == htons(ETH_P_IPV6)) {
		icmp6h = (struct icmp6hdr *)(skb_transport_header(skb));

   		 if (ip6h->nexthdr == IPPROTO_ICMPV6) {
		        icmp6h = icmp6_hdr(skb);

			if (icmp6h->icmp6_type == ICMPV6_ECHO_REQUEST &&
			    state->hook == NF_INET_LOCAL_OUT) {

			    ipv6_echo_req++;
			    printk("IPv6 Echo Request count = %lu\n", ipv6_echo_req);
			}

			else if (icmp6h->icmp6_type == ICMPV6_ECHO_REPLY &&
			    state->hook == NF_INET_PRE_ROUTING) {

			    ipv6_echo_rep++;
			    printk("IPv6 Echo Reply count = %lu\n", ipv6_echo_rep);
			}
    		}
	}

	return NF_ACCEPT;
}

static const struct nf_hook_ops localout_template = {
    .hook     = nf_callback,
    .hooknum  = NF_INET_LOCAL_OUT,
    .pf       = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

static const struct nf_hook_ops prerouting_template = {
    .hook     = nf_callback,
    .hooknum  = NF_INET_PRE_ROUTING,
    .pf       = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

static struct lkm_netns_data *lkm_netns(struct net *net)
{
    return net_generic(net, lkm_net_id);
}

static int __net_init netns_init(struct net *net)
{
	struct lkm_netns_data *data = lkm_netns(net);

	memcpy(&data->nf_hops_localout,
       		&localout_template,
      		 sizeof(struct nf_hook_ops));

	memcpy(&data->nf_hops_prerouting,
       		&prerouting_template,
       		sizeof(struct nf_hook_ops));

	nf_register_net_hook(net, &data->nf_hops_localout);
	nf_register_net_hook(net, &data->nf_hops_prerouting);

	return 0;
}

static void __net_exit netns_exit(struct net *net)
{
	struct lkm_netns_data *data = lkm_netns(net);

nf_unregister_net_hook(net, &data->nf_hops_localout);
nf_unregister_net_hook(net, &data->nf_hops_prerouting);
}

static struct pernet_operations lkm_netns_ops = {
	.init = netns_init,
	.exit = netns_exit,
	.id = &lkm_net_id,
	.size = sizeof(struct lkm_netns_data),
};

static int __init lkm_init(void)
{
	int rc;

	rc = register_pernet_subsys(&lkm_netns_ops);
	if (rc) {
		printk("cannot register the pernet ops\n");
		return rc;
	}

	printk("lkm netfilter module registered\n");
	return 0;
}

static void __exit lkm_exit(void)
{
	unregister_pernet_subsys(&lkm_netns_ops);

	printk("lkm netfilter module unregistered\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrea Mayer");
MODULE_DESCRIPTION("Simple Linux kernel Netfilter Module for dropping ICMPv6 ingress packets");
MODULE_VERSION("1.0.0");
