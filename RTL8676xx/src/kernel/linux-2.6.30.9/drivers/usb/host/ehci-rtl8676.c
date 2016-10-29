
#include <linux/platform_device.h>

#ifdef CONFIG_USB_PATCH_RTL8676
#ifdef RTL8676_USB_HOST_TX_SHOULD_CHECK
static int ehci_rtl8676_out_64_check(struct urb *urb)
{
	int retval = 0;
	unsigned char aligned;
	unsigned int rest = ((unsigned int)(urb->transfer_buffer_length)) & 0x3f;
	
	if(!urb->temp_data_buf)
		aligned = ((unsigned int)(urb->transfer_dma)) & 0x3;
	else
		aligned = (((unsigned int)(urb->temp_data_buf))+urb->temp_data_offset) & 0x3;

	if(!aligned)
		return retval;
	
	switch(aligned) {
		case 1:	//starting address: 4N+1
			if((rest>=4) && (rest<=6))
				retval = 1;
			break;
		case 2:	//starting address: 4N+2
			if((rest>=3) && (rest<=5))
				retval = 2;
			break;
		case 3:	//starting address: 4N+3
			if((rest>=2) && (rest<=4))
				retval = 3;
			break;
		default:
			printk("Wrong case! aligned = %u\n", aligned);
			return -EPERM;
	}

	if(retval) {
		if(!urb->temp_data_buf) {
			urb->temp_data_buf = kmalloc(urb->transfer_buffer_length+RESERVED_SIZE, GFP_ATOMIC);
			if(!urb->temp_data_buf)
				return -ENOMEM;

			urb->backup_data_dma = urb->transfer_dma;
		}

		if((((unsigned int)(urb->temp_data_buf)) + urb->temp_data_offset) & 0x3)
			urb->temp_data_offset += 4 - ((((unsigned int)(urb->temp_data_buf))+urb->temp_data_offset) & 0x3);
	}

	return retval;
}

static int ehci_rtl8676_out_1K_boundary_check(struct urb *urb, bool setup_pkt)
{
	int retval = 0;
	unsigned int startaddr, len, rest;

	if(setup_pkt) {
		if(!urb->temp_setup_buf)
			startaddr = (unsigned int)(urb->setup_dma);
		else
			startaddr = ((unsigned int)(urb->temp_setup_buf)) + urb->temp_setup_offset;
		len = sizeof(struct usb_ctrlrequest);
	}
	else {
		if(!urb->temp_data_buf)
			startaddr = (unsigned int)(urb->transfer_dma);
		else
			startaddr = ((unsigned int)(urb->temp_data_buf)) + urb->temp_data_offset;
		len = urb->transfer_buffer_length;
	}

	if(startaddr & 0x3ff) {
		if (((startaddr+len)>>10) != (startaddr>>10)) {		// over 1k boundary
			rest = (startaddr+len) & 0x3ff;
			if ((rest>=4) && (rest<=7))
				retval=1;
		}

		if((retval==0) && ((startaddr&0x30)==0x30)) {		//starting at 0x35~0x38
			if(((startaddr&0xf)>=5) && ((startaddr&0xf)<=8))
				retval=1;
		}
	}

	if(retval) {
		if(setup_pkt) {
			if(!urb->temp_setup_buf) {
				urb->temp_setup_buf = kmalloc(len+RESERVED_SIZE, GFP_ATOMIC);
				if(!urb->temp_setup_buf)
					return -ENOMEM;

				urb->backup_setup_dma = urb->setup_dma;
			}
			else
				urb->temp_setup_offset += 4;
		}
		else {
			if(!urb->temp_data_buf) {
				urb->temp_data_buf = kmalloc(len+RESERVED_SIZE, GFP_ATOMIC);
				if(!urb->temp_data_buf)
					return -ENOMEM;

				urb->backup_data_dma = urb->transfer_dma;
			}
			else
				urb->temp_data_offset += 4;
		}
	}

	return retval;
}
#endif//RTL8676_USB_HOST_TX_SHOULD_CHECK

static int ehci_rtl8676_in_alignment_check(struct urb *urb)
{
	int retval = 0;
	unsigned char aligned;
	
	aligned = ((unsigned int)(urb->transfer_dma)) & 0x3;
	if(!aligned)
		return retval;

	retval = aligned;	//not 4 byte alignment

	urb->temp_data_buf = kmalloc(urb->transfer_buffer_length+RESERVED_SIZE, GFP_ATOMIC);
	if(!urb->temp_data_buf)
		return -ENOMEM;

	urb->backup_data_dma = urb->transfer_dma;
	if(((unsigned int)(urb->temp_data_buf)) & 0x3)
		urb->temp_data_offset = 4 - (((unsigned int)(urb->temp_data_buf)) & 0x3);

	return retval;
}

static void ehci_rtl8676_dma_check(struct usb_hcd *hcd, struct urb *urb)
{
	#define RETRYCNT	5
	int retval = 0;

	urb->temp_setup_buf = NULL;
	urb->temp_data_buf = NULL;
	urb->backup_setup_dma = 0;
	urb->backup_data_dma = 0;
	urb->temp_setup_offset = 0;
	urb->temp_data_offset = 0;
	
	//parse setup packet
	#ifdef RTL8676_USB_HOST_TX_SHOULD_CHECK
	if(usb_pipecontrol(urb->pipe)) {
		int retry = 0;
		do {
			if(retry > RETRYCNT) {
				printk("%s %d try too many times\n", __func__, __LINE__);
				goto patch_setup_pkt_fail; 
			}
			
			retval = ehci_rtl8676_out_1K_boundary_check(urb, 1);
			if(retval < 0) {
				printk("%s %d fail! retval=%d\n", __func__, __LINE__, retval);
				goto patch_setup_pkt_fail; 
			}
			retry++;
		} while(retval);

		if(unlikely(urb->temp_setup_buf)) {
			if(urb->temp_setup_offset > RESERVED_SIZE) {
				printk("%s %d fail! urb->temp_setup_offset=%d\n", __func__, __LINE__, urb->temp_setup_offset);
				goto patch_setup_pkt_fail;
			}

			urb->setup_dma = dma_map_single(hcd->self.controller,
				urb->temp_setup_buf+urb->temp_setup_offset,
				sizeof(struct usb_ctrlrequest),
				DMA_TO_DEVICE);
			memcpy(urb->temp_setup_buf+urb->temp_setup_offset, bus_to_virt(urb->backup_setup_dma), sizeof(struct usb_ctrlrequest));
			dma_cache_wback_inv((unsigned int)(urb->temp_setup_buf+urb->temp_setup_offset), sizeof(struct usb_ctrlrequest));
		}
	}
	#endif//RTL8676_USB_HOST_TX_SHOULD_CHECK

	goto parse_data_pkt;
#ifdef RTL8676_USB_HOST_TX_SHOULD_CHECK	
patch_setup_pkt_fail:
	if(unlikely(urb->temp_setup_buf)) {
		kfree(urb->temp_setup_buf);
		urb->temp_setup_offset = 0;
	}
#endif
parse_data_pkt:
	//parse data packet
	if(usb_pipeout(urb->pipe)) {
		#ifdef RTL8676_USB_HOST_TX_SHOULD_CHECK
		int retry = 0;
		do {
			if(retry > RETRYCNT) {
				printk("%s %d try too many times\n", __func__, __LINE__);
				goto patch_data_pkt_fail; 
			}

			retval = ehci_rtl8676_out_64_check(urb);
			if(retval < 0) {
				printk("%s %d fail! retval=%d\n", __func__, __LINE__, retval);
				goto patch_data_pkt_fail; 
			}

			retval = ehci_rtl8676_out_1K_boundary_check(urb, 0);
			if(retval < 0) {
				printk("%s %d fail! retval=%d\n", __func__, __LINE__, retval);
				goto patch_data_pkt_fail;
			}
			retry++;
		} while(retval);

		if(unlikely(urb->temp_data_buf)) {
			if(urb->temp_data_offset > RESERVED_SIZE) {
				printk("%s %d fail! urb->temp_data_offset=%d\n", __func__, __LINE__, urb->temp_data_offset);
				goto patch_data_pkt_fail;
			}
			
			urb->transfer_dma = dma_map_single(hcd->self.controller,
				urb->temp_data_buf+urb->temp_data_offset,
				urb->transfer_buffer_length,
				DMA_TO_DEVICE);
			memcpy(urb->temp_data_buf+urb->temp_data_offset,bus_to_virt(urb->backup_data_dma), urb->transfer_buffer_length);
			dma_cache_wback_inv((unsigned int)(urb->temp_data_buf+urb->temp_data_offset), urb->transfer_buffer_length);
		}
		#endif//RTL8676_USB_HOST_TX_SHOULD_CHECK
	}
	else {
		retval = ehci_rtl8676_in_alignment_check(urb);
		
		if(unlikely(urb->temp_data_buf)) {
			urb->transfer_dma = dma_map_single(hcd->self.controller,
				urb->temp_data_buf+urb->temp_data_offset,
				urb->transfer_buffer_length,
				DMA_FROM_DEVICE);
			dma_cache_wback_inv((unsigned int)(urb->temp_data_buf+urb->temp_data_offset), urb->transfer_buffer_length);
		}
	}

	return;
#ifdef RTL8676_USB_HOST_TX_SHOULD_CHECK
patch_data_pkt_fail:
	if(unlikely(urb->temp_data_buf)) {
		kfree(urb->temp_data_buf);
		urb->temp_data_offset = 0;
	}
	return;
#endif
}

#if 0
static void ehci_rtl8676_dma_giveback_abnormal(struct usb_hcd *hcd, struct urb *urb)
{
	if(unlikely(urb->temp_setup_buf)) {
		if(usb_pipecontrol(urb->pipe))
			dma_unmap_single(hcd->self.controller, urb->setup_dma,
				sizeof (struct usb_ctrlrequest),
				DMA_TO_DEVICE);

		urb->setup_dma = urb->backup_setup_dma;
		kfree(urb->temp_setup_buf);
		urb->temp_setup_offset = 0;
	}

	if(unlikely(urb->temp_data_buf)) {
		if((urb->transfer_buffer_length!=0))
			dma_unmap_single (hcd->self.controller, 
					urb->transfer_dma,
					urb->transfer_buffer_length,
					usb_pipein (urb->pipe)
						? DMA_FROM_DEVICE
						: DMA_TO_DEVICE);

		urb->transfer_dma = urb->backup_data_dma;
		kfree(urb->temp_data_buf);
		urb->temp_data_offset = 0;
	}
	
	return;
}
#endif

static void ehci_rtl8676_dma_giveback_normal(struct usb_hcd *hcd, struct urb *urb)
{
	if(unlikely(urb->temp_setup_buf)) {
		if(usb_pipecontrol(urb->pipe) && (urb->transfer_flags & URB_NO_SETUP_DMA_MAP))
			dma_unmap_single(hcd->self.controller, urb->setup_dma,
				sizeof (struct usb_ctrlrequest),
				DMA_TO_DEVICE);

		urb->setup_dma = urb->backup_setup_dma;

		if (usb_pipecontrol(urb->pipe) && !(urb->transfer_flags & URB_NO_SETUP_DMA_MAP))
			dma_unmap_single (hcd->self.controller, urb->setup_dma,
					sizeof (struct usb_ctrlrequest),
					DMA_TO_DEVICE);

		kfree(urb->temp_setup_buf);
		urb->temp_setup_offset = 0;
	}

	if(unlikely(urb->temp_data_buf)) {
		if((urb->transfer_buffer_length!=0) && (urb->transfer_flags & URB_NO_TRANSFER_DMA_MAP))
			dma_unmap_single (hcd->self.controller, 
					urb->transfer_dma,
					urb->transfer_buffer_length,
					usb_pipein (urb->pipe)
						? DMA_FROM_DEVICE
						: DMA_TO_DEVICE);

		if(usb_pipein(urb->pipe))
			memcpy(bus_to_virt(urb->backup_data_dma), urb->temp_data_buf+urb->temp_data_offset, urb->actual_length);

		urb->transfer_dma = urb->backup_data_dma;

		if (urb->transfer_buffer_length != 0 && !(urb->transfer_flags & URB_NO_TRANSFER_DMA_MAP))
			dma_unmap_single (hcd->self.controller, 
					urb->transfer_dma,
					urb->transfer_buffer_length,
					usb_pipein (urb->pipe)
						? DMA_FROM_DEVICE
						: DMA_TO_DEVICE);

		kfree(urb->temp_data_buf);
		urb->temp_data_offset = 0;
	}
	
	return;
}

void ehci_rtl8676_dma_process(struct usb_hcd *hcd, struct urb *urb, unsigned char option)
{
	switch(option) {
		case 1:
			ehci_rtl8676_dma_check(hcd, urb);
			break;
		#if 0
		case 2:
			ehci_rtl8676_dma_giveback_abnormal(hcd, urb);
			break;
		#endif
		case 3:
			ehci_rtl8676_dma_giveback_normal(hcd, urb);
			break;
		default:
			break;
	}

	return;
}
#endif	//CONFIG_USB_PATCH_RTL8676

static void set_usbphy(void)
{
	//andrew, mantis 6066, bug 1
	//if(IS_6028A || IS_6028B)
		REG32(0xb8021094) = 0x80008000; 
	
// For Port-0 
	if(IS_RTL8676) {
		unsigned int ipSel = REG32(BSP_IP_SEL);

		if(ipSel & BSP_EN_USB_PHY0) {
			ehci_phy_write(0, 0xE2, 0x99);
			ehci_phy_write(0, 0xE5, 0x9D);
			if(!(REG32(BSP_MISC_PINSR) & BSP_CLKSEL))
				ehci_phy_write(0, 0xF5, 0xD1);	//clk:35.328MHZ
			else
				ehci_phy_write(0, 0xF5, 0xC1);	//clk:40MHZ
		}
		else {
			printk("USB PHY0 is disabled!\n");
		}
	}
	else if(IS_RLE0315 || IS_6166) {
		ehci_phy_write(0, 0xE0, 0xB8);
		ehci_phy_write(0, 0xE1, 0xA8);
		ehci_phy_write(0, 0xE2, 0x9A);
		ehci_phy_write(0, 0xE3, 0x41);
		ehci_phy_write(0, 0xE5, 0x91);
		ehci_phy_write(0, 0xE6, 0x88);
		ehci_phy_write(0, 0xF4, 0xE3);
	}
	else {
		ehci_phy_write(0, 0xE0, 0x91);
		ehci_phy_write(0, 0xE1, 0xA8);
		ehci_phy_write(0, 0xE2, 0x81);
		ehci_phy_write(0, 0xE5, 0x95);
		ehci_phy_write(0, 0xE6, 0xE8);
		ehci_phy_write(0, 0xE7, 0x38);
		ehci_phy_write(0, 0xF1, 0x8E);
	}
// For Port-1 

	if(!usb_mac_identify(1)) {
		printk("USB phy 1 is routed to OTG, skip PHY 1 setting in EHCI driver.\n");
		return;
	}
	
	if(IS_RTL8676) {
		unsigned int ipSel = REG32(BSP_IP_SEL);
		unsigned char val;

		if(ipSel & BSP_EN_USB_PHY1) {
			ehci_phy_write(1, 0xE2, 0x99);
			ehci_phy_write(1, 0xE5, 0x9D);
			if(!(REG32(BSP_MISC_PINSR) & BSP_CLKSEL))
				val = 0xD1;		//clk:35.328MHZ
			else
				val = 0xC1;		//clk:40MHZ
			if(!(ipSel & BSP_EN_USB_PHY0))
				ehci_phy_write(0, 0xF5, val);
			ehci_phy_write(1, 0xF5, val);
		}
		else {
			printk("USB PHY1 is disabled!\n");
		}
	}
	else if(IS_RLE0315 || IS_6166) {
		printk("disable USB phy 0\n");
		REG32(BSP_IP_SEL) &= ~BSP_EN_USB_PHY0;
		
		ehci_phy_write(1, 0xE0, 0xB8);
		ehci_phy_write(1, 0xE1, 0xA8);
		ehci_phy_write(1, 0xE2, 0x99);
		ehci_phy_write(1, 0xE3, 0x41);
		ehci_phy_write(1, 0xE5, 0x91);
		ehci_phy_write(1, 0xE6, 0x88);
		ehci_phy_write(1, 0xF4, 0xE3);
	}
	else {
		ehci_phy_write(1, 0xE0, 0x91);
		ehci_phy_write(1, 0xE1, 0xA8);
		ehci_phy_write(1, 0xE2, 0x81);
		ehci_phy_write(1, 0xE5, 0x95);
		ehci_phy_write(1, 0xE6, 0xE8);
		ehci_phy_write(1, 0xE7, 0x38);
		ehci_phy_write(1, 0xF1, 0x8E);
	}

	return;
}

static void ehci_rtl8676_check_phy_timeout(unsigned long ptr)
{
	struct ehci_hcd *ehci = (struct ehci_hcd *)ptr;
	int port = ehci->port;
	unsigned int status = readl (&ehci->regs->port_status[port]);
	unsigned char tmp, phy_E6_val;
	static unsigned char reset[2] = {0, 0};

	printk("EHCI: port status=0x%08x\n", status);
	if((status & PORT_CONNECT) || (status & PORT_RESET)) {	//usb device is still connected or port reset is still in progress
		if(!(status&PORT_OWNER) && !(status&PORT_PE)) {		//if port owner is not ohci and port enable fails, increase disconnection threshold
			phy_E6_val = ehci_phy_read(port, 0xE6);
			if((phy_E6_val & 0xF0) != 0xF0) {
				if(!reset[port]) {
					reset[port] = phy_E6_val;		//backup original value of phy 0xE6
				}
				tmp = (phy_E6_val >> 4) + 1;
				phy_E6_val = (tmp << 4) | (phy_E6_val & 0x0F);
				writel(0, &ehci->regs->port_status[port]);	//turn off port power
				ehci_phy_write(port, 0xE6, phy_E6_val);	//increase reference voltage
				mdelay(10);
				writel(PORT_POWER, &ehci->regs->port_status[port]);	//turn on port power
				printk("EHCI: set phy(0xE6) to 0x%02x\n", phy_E6_val);
				mod_timer(&ehci->check_phy_timer, jiffies + 2*HZ);	//check again to avoid dead port reset condition
			}
			else {
				printk("EHCI: already reach highest ref. voltage\n");
			}
		}
	}
	else if(reset[port]) {	//restore the original value of phy 0xE6
		ehci_phy_write(port, 0xE6, reset[port]);
		printk("EHCI: reset phy(0xE6) to 0x%02x\n", reset[port]);
		reset[port] = 0;
	}

	return;
}

/* called during probe() after chip reset completes */
static int ehci_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);
	int			retval = 0;

	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs + HC_LENGTH(readl(&ehci->caps->hc_capbase));
	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = readl(&ehci->caps->hcs_params);
	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->port = 0;
	init_timer(&ehci->check_phy_timer);
	ehci->check_phy_timer.data = (unsigned long)ehci;
	ehci->check_phy_timer.function = ehci_rtl8676_check_phy_timeout;

	if (ehci_is_TDI(ehci))
		ehci_reset(ehci);

#ifdef	CONFIG_USB_SUSPEND
	/* REVISIT: the controller works fine for wakeup iff the root hub
	 * itself is "globally" suspended, but usbcore currently doesn't
	 * understand such things.
	 *
	 * System suspend currently expects to be able to suspend the entire
	 * device tree, device-at-a-time.  If we failed selective suspend
	 * reports, system suspend would fail; so the root hub code must claim
	 * success.  That's lying to usbcore, and it matters for for runtime
	 * PM scenarios with selective suspend and remote wakeup...
	 */
	if (ehci->no_selective_suspend && device_can_wakeup(&pdev->dev))
		ehci_warn(ehci, "selective suspend/wakeup unavailable\n");
#endif

	return retval;
}

static int ehci_usb_hcd_probe_rtl8676(struct hc_driver *driver, struct platform_device *pdev)
{
	struct usb_hcd		*hcd;
	struct resource *res;
	int			retval; 	
	u32 			rtl8676_usb_irq;	//cathy for 8676

	set_usbphy();

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if(res == NULL)	printk("%s: get irq resource failed!\n", __FUNCTION__);
	rtl8676_usb_irq = res->start;
	
	hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if(hcd == NULL) {
		retval = -ENOMEM;
		return retval;
	}
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(res == NULL)	printk("%s: get memory resource failed!\n", __FUNCTION__);
	hcd->regs = (void *) res->start;
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = res->end - res->start;
	
	retval = usb_add_hcd (hcd, rtl8676_usb_irq, IRQF_SHARED);
	return retval;
} 

void ehci_restart(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	/* halt, reset EHCI, and cleanup EHCI memory */
	ehci_port_power(ehci, 0);
	ehci_halt(ehci);
	del_timer_sync(&ehci->watchdog);
	del_timer_sync(&ehci->check_phy_timer);
	spin_lock_irq(&ehci->lock);
	if(HC_IS_RUNNING(hcd->state))
		ehci_quiesce(ehci);
	ehci_reset(ehci);
	writel(0, &ehci->regs->intr_enable);
	remove_debug_files(ehci);
	if (ehci->async)
		ehci_work(ehci);
	spin_unlock_irq(&ehci->lock);
	ehci_mem_cleanup(ehci);

	/* allocate EHCI memory and start EHCI */
	ehci_setup(hcd);
	ehci_run(hcd);
	ehci_port_power (ehci, 1);

	return;
}

struct hc_driver ehci_driver_rtl8676 = {
	.description =		hcd_name,
	.product_desc =		"EHCI Host Controller",
	.hcd_priv_size =	sizeof(struct ehci_hcd),
	/*
	 * generic hardware linkage
	 */
	.irq =			ehci_irq,
	.flags =		HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset =		ehci_setup,
	.start =		ehci_run,
	.stop =			ehci_stop,
	.shutdown =		ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ehci_urb_enqueue,
	.urb_dequeue =		ehci_urb_dequeue,
	.endpoint_disable =	ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ehci_hub_status_data,
	.hub_control =		ehci_hub_control,
	.bus_suspend =		ehci_bus_suspend,
	.bus_resume =		ehci_bus_resume,
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,
	.clear_tt_buffer_complete     = ehci_clear_tt_buffer_complete,
#ifdef CONFIG_USB_PATCH_RTL8676
	.rtl8676_dma_process = ehci_rtl8676_dma_process,
#endif	//CONFIG_USB_PATCH_RTL8676
};

static int ehci_rtl8676_drv_probe (struct platform_device *pdev)
{
	return ehci_usb_hcd_probe_rtl8676 (&ehci_driver_rtl8676, pdev);
}

static int ehci_rtl8676_drv_remove (struct platform_device *pdev)
{
	struct usb_hcd		*hcd;

	hcd = platform_get_drvdata (pdev);
	usb_remove_hcd (hcd);
	if (hcd->driver->flags & HCD_MEMORY) {
		iounmap (hcd->regs);
		release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	} else {
		release_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	usb_put_hcd (hcd);
	return 0;
}

struct platform_driver platform_driver_rtl8676 = {
	.probe = ehci_rtl8676_drv_probe,
	.remove = ehci_rtl8676_drv_remove,
	.shutdown = usb_hcd_platform_shutdown,
	.driver = {
		   .name = "rtl8676-ehci",
		   },
};

static void usb_release(struct device *dev)
{
	/* normally not freed */
}

static struct resource rtl8676_ehci_resources[] = {
	{
		.start	= BSP_EHCI_BASE,
		.end		= BSP_EHCI_BASE + 0x0000EFFF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= BSP_USB_H_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device rtl8676_ehci = {
	.name			= "rtl8676-ehci",
	.id			= -1,
	.dev = {
		.release		= usb_release,
		.dma_mask		= (void *)~0,
		.coherent_dma_mask	= ~0,
	},
	.num_resources	= ARRAY_SIZE(rtl8676_ehci_resources),
	.resource		= rtl8676_ehci_resources,
};

