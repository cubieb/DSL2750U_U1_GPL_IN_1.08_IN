//cathy
#include <linux/platform_device.h>


#ifdef CONFIG_USB_PATCH_RTL8676
#ifndef CONFIG_USB_EHCI_HCD
static int ohci_rtl8672_out_64_check(struct urb *urb)
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

static int ohci_rtl8672_out_1K_boundary_check(struct urb *urb, bool setup_pkt)
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

static int ohci_rtl8672_in_alignment_check(struct urb *urb)
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

static void ohci_rtl8672_dma_check(struct usb_hcd *hcd, struct urb *urb)
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
	if(usb_pipecontrol(urb->pipe)) {
		int retry = 0;
		do {
			if(retry > RETRYCNT) {
				printk("%s %d try too many times\n", __func__, __LINE__);
				goto patch_setup_pkt_fail; 
			}
			
			retval = ohci_rtl8672_out_1K_boundary_check(urb, 1);
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

	goto parse_data_pkt;
	
patch_setup_pkt_fail:
	if(unlikely(urb->temp_setup_buf)) {
		kfree(urb->temp_setup_buf);
		urb->temp_setup_offset = 0;
	}

parse_data_pkt:
	//parse data packet
	if(usb_pipeout(urb->pipe)) {
		int retry = 0;
		do {
			if(retry > RETRYCNT) {
				printk("%s %d try too many times\n", __func__, __LINE__);
				goto patch_data_pkt_fail; 
			}

			retval = ohci_rtl8672_out_64_check(urb);
			if(retval < 0) {
				printk("%s %d fail! retval=%d\n", __func__, __LINE__, retval);
				goto patch_data_pkt_fail; 
			}

			retval = ohci_rtl8672_out_1K_boundary_check(urb, 0);
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
	}
	else {
		retval = ohci_rtl8672_in_alignment_check(urb);
		
		if(unlikely(urb->temp_data_buf)) {
			urb->transfer_dma = dma_map_single(hcd->self.controller,
				urb->temp_data_buf+urb->temp_data_offset,
				urb->transfer_buffer_length,
				DMA_FROM_DEVICE);
			dma_cache_wback_inv((unsigned int)(urb->temp_data_buf+urb->temp_data_offset), urb->transfer_buffer_length);
		}
	}

	return;

patch_data_pkt_fail:
	if(unlikely(urb->temp_data_buf)) {
		kfree(urb->temp_data_buf);
		urb->temp_data_offset = 0;
	}
	return;
}

#if 0
static void ohci_rtl8672_dma_giveback_abnormal(struct usb_hcd *hcd, struct urb *urb)
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

static void ohci_rtl8672_dma_giveback_normal(struct usb_hcd *hcd, struct urb *urb)
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

static void ohci_rtl8672_dma_process(struct usb_hcd *hcd, struct urb *urb, unsigned char option)
{
	switch(option) {
		case 1:
			ohci_rtl8672_dma_check(hcd, urb);
			break;
		#if 0
		case 2:
			ohci_rtl8672_dma_giveback_abnormal(hcd, urb);
			break;
		#endif
		case 3:
			ohci_rtl8672_dma_giveback_normal(hcd, urb);
			break;
		default:
			break;
	}

	return;
}
#endif	//CONFIG_USB_EHCI_HCD
#endif	//CONFIG_USB_PATCH_RTL8676



static int
ohci_reset (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);

	ohci_hcd_init (ohci);
	return ohci_init (ohci);
}

static int //__devinit//linux 2.6.19
ohci_start (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
	int		ret;

	/* REVISIT this whole block should move to reset(), which handles
	 * all the other one-time init.
	 */
#if 0	 
	if (hcd->self.controller) {
		struct pci_dev *pdev = to_pci_dev(hcd->self.controller);

		/* AMD 756, for most chips (early revs), corrupts register
		 * values on read ... so enable the vendor workaround.
		 */
		if (pdev->vendor == PCI_VENDOR_ID_AMD
				&& pdev->device == 0x740c) {
			ohci->flags = OHCI_QUIRK_AMD756;
			ohci_dbg (ohci, "AMD756 erratum 4 workaround\n");
			/* also erratum 10 (suspend/resume issues) */
			device_init_wakeup(&hcd->self.root_hub->dev, 0);
		}

		/* FIXME for some of the early AMD 760 southbridges, OHCI
		 * won't work at all.  blacklist them.
		 */

		/* Apple's OHCI driver has a lot of bizarre workarounds
		 * for this chip.  Evidently control and bulk lists
		 * can get confused.  (B&W G3 models, and ...)
		 */
		else if (pdev->vendor == PCI_VENDOR_ID_OPTI
				&& pdev->device == 0xc861) {
			ohci_dbg (ohci,
				"WARNING: OPTi workarounds unavailable\n");
		}

		/* Check for NSC87560. We have to look at the bridge (fn1) to
		 * identify the USB (fn2). This quirk might apply to more or
		 * even all NSC stuff.
		 */
		else if (pdev->vendor == PCI_VENDOR_ID_NS) {
			struct pci_dev	*b;

			b  = pci_get_slot (pdev->bus,
					PCI_DEVFN (PCI_SLOT (pdev->devfn), 1));
			if (b && b->device == PCI_DEVICE_ID_NS_87560_LIO
					&& b->vendor == PCI_VENDOR_ID_NS) {
				ohci->flags |= OHCI_QUIRK_SUPERIO;
				ohci_dbg (ohci, "Using NSC SuperIO setup\n");
			}
			pci_dev_put(b);
		}

		/* Check for Compaq's ZFMicro chipset, which needs short 
		 * delays before control or bulk queues get re-activated
		 * in finish_unlinks()
		 */
		else if (pdev->vendor == PCI_VENDOR_ID_COMPAQ
				&& pdev->device  == 0xa0f8) {
			ohci->flags |= OHCI_QUIRK_ZFMICRO;
			ohci_dbg (ohci,
				"enabled Compaq ZFMicro chipset quirk\n");
		}

		/* RWC may not be set for add-in PCI cards, since boot
		 * firmware probably ignored them.  This transfers PCI
		 * PM wakeup capabilities (once the PCI layer is fixed).
		 */
		if (device_may_wakeup(&pdev->dev))
			ohci->hc_control |= OHCI_CTRL_RWC;
	}
#endif
	/* NOTE: there may have already been a first reset, to
	 * keep bios/smm irqs from making trouble
	 */
	if ((ret = ohci_run (ohci)) < 0) {
		ohci_err (ohci, "can't start\n");
		ohci_stop (hcd);
		return ret;
	}
	return 0;
}
#if 0
#ifdef	CONFIG_PM

static int ohci_pci_suspend (struct usb_hcd *hcd, pm_message_t message)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
	unsigned long	flags;
	int		rc = 0;

	/* Root hub was already suspended. Disable irq emission and
	 * mark HW unaccessible, bail out if RH has been resumed. Use
	 * the spinlock to properly synchronize with possible pending
	 * RH suspend or resume activity.
	 *
	 * This is still racy as hcd->state is manipulated outside of
	 * any locks =P But that will be a different fix.
	 */
	spin_lock_irqsave (&ohci->lock, flags);
	if (hcd->state != HC_STATE_SUSPENDED) {
		rc = -EINVAL;
		goto bail;
	}
	ohci_writel(ohci, OHCI_INTR_MIE, &ohci->regs->intrdisable);
	(void)ohci_readl(ohci, &ohci->regs->intrdisable);

	/* make sure snapshot being resumed re-enumerates everything */
	if (message.event == PM_EVENT_PRETHAW)
		ohci_usb_reset(ohci);

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
 bail:
	spin_unlock_irqrestore (&ohci->lock, flags);

	return rc;
}


static int ohci_pci_resume (struct usb_hcd *hcd)
{
	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	usb_hcd_resume_root_hub(hcd);
	return 0;
}

#endif	/* CONFIG_PM */
#endif

/*-------------------------------------------------------------------------*/

struct hc_driver ohci_hc_driver_rtl8672 = {
	.description =		hcd_name,
	.product_desc =		"OHCI Host Controller",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_MEMORY | HCD_USB11,

	/*
	 * basic lifecycle operations
	 */
	.reset =		ohci_reset,
	.start =		ohci_start,
	.stop =			ohci_stop,
	.shutdown =		ohci_shutdown,
#if 0
#ifdef	CONFIG_PM
	/* these suspend/resume entries are for upstream PCI glue ONLY */
	.suspend =		ohci_pci_suspend,
	.resume =		ohci_pci_resume,
#endif
#endif
	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ohci_urb_enqueue,
	.urb_dequeue =		ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
	// .hub_irq_enable =	ohci_rhsc_enable,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,

#ifdef CONFIG_USB_PATCH_RTL8676
#ifdef CONFIG_USB_EHCI_HCD
	.rtl8672_dma_process = ehci_rtl8672_dma_process,
#else
	.rtl8672_dma_process = ohci_rtl8672_dma_process,
#endif	//CONFIG_USB_EHCI_HCD
#endif	//CONFIG_USB_PATCH_RTL8676

};

/*-------------------------------------------------------------------------*/


int ohci_usb_hcd_probe_rtl8672 (struct hc_driver *driver, struct platform_device *pdev)
{
	struct usb_hcd		*hcd;
	struct resource *res;
	int			retval; 	
	u32 			rtl8672_usb_irq;	//cathy for 8672

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if(res == NULL)	printk("%s: get irq resource failed!\n", __FUNCTION__);
	rtl8672_usb_irq = res->start;
	
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
	
	retval = usb_add_hcd (hcd, rtl8672_usb_irq, IRQF_SHARED);	
	return retval;
}


int ohci_rtl8672_drv_probe (struct platform_device *pdev)
{
	return ohci_usb_hcd_probe_rtl8672 (&ohci_hc_driver_rtl8672, pdev);
}

static int ohci_rtl8672_drv_remove (struct platform_device *pdev)
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
	//the_controller = NULL;
	return 0;
}

 struct platform_driver platform_driver_rtl8672_ohci = {
	.probe = ohci_rtl8672_drv_probe,
	.remove = ohci_rtl8672_drv_remove,
	.shutdown = usb_hcd_platform_shutdown,
	.driver = {
	    .name = "rtl8672-ohci",
	},
};

static void usb_release(struct device *dev)
{
	/* normally not freed */
}
#define RTL8672_OHCI_BASE	0xb8020000

static struct resource rtl8672_ohci_resources[] = {
	{
		.start	= RTL8672_OHCI_BASE,
		.end		= RTL8672_OHCI_BASE + 0x00000FFF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= BSP_USB_H_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};


static struct platform_device rtl8672_ohci = {
	.name			= "rtl8672-ohci",
	.id			= -1,
	.dev = {
		.release		= usb_release,
		.dma_mask		= (void *)~0,
		.coherent_dma_mask	= ~0,
	},
	.num_resources	= ARRAY_SIZE(rtl8672_ohci_resources),
	.resource		= rtl8672_ohci_resources,
};

static int __init ohci_hcd_rtl8672_init (void) 
{
	int retval;

	//printk (KERN_DEBUG "%s: " DRIVER_INFO " (PCI)\n", hcd_name);
	printk (KERN_DEBUG "%s: (PCI)\n", hcd_name);
	if (usb_disabled())
		return -ENODEV;

	if((IS_RLE0315 || IS_6166) && !usb_mac_identify(1)) {
		printk("%s: USB phy 1 is routed to OTG, skip OHCI driver initialization.\n", hcd_name);
		return -ENODEV;
	}

	pr_debug ("%s: block sizes: ed %Zd td %Zd\n", hcd_name,
		sizeof (struct ed), sizeof (struct td));
	//return pci_register_driver (&ohci_pci_driver);

	retval = platform_device_register(&rtl8672_ohci);
	if (retval < 0)
		return retval;
	retval = platform_driver_register(&platform_driver_rtl8672_ohci);
	if (retval < 0) {
		platform_driver_unregister(&platform_driver_rtl8672_ohci);
	}
	return retval;
}
module_init (ohci_hcd_rtl8672_init);

/*-------------------------------------------------------------------------*/

static void __exit ohci_hcd_rtl8672_cleanup (void) 
{	
	//pci_unregister_driver (&ohci_pci_driver);
	platform_driver_unregister(&platform_driver_rtl8672_ohci);
	platform_device_unregister(&rtl8672_ohci);
}
module_exit (ohci_hcd_rtl8672_cleanup);

