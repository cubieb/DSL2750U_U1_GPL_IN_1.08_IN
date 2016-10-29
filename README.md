GPL Code test Procedure

1. Firmware: ME_1.09

2.Contents:
    a) DSL2750U_U1_GPL_ME_1.09_ME.tar.bz2: The source tree directory.
    b) rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2: The toolchains used to build the firmware.
    c) DSL2750U_README.txt: Instructions of building firmware.
    d) Steps_to_install_the_system.docx: how to install the system
    e) how_to_make.docx : how to make GPL file

3.How to install the system?
    a) The compile environment we used is x86 CPU, CentOS-5.4(32)(see Steps_to_install_the_system.docx).

4.how to make?
    a) when the system installation is complete we need to install the compiler tool chain and make GPL file, you can see the "how_to_make.docx" file or follow the steps below.
        1), login system with root user.
		
        2), copy the file “rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2” and “DSL2750U_U1_GPL_ME_1.09_ME.tar.bz2” to  the “root’s home” folder from you external device
		
        3), Open a terminal.Create directory “/opt/tbs_toolchains/realtek”.  
            use command:
                cd /opt/
                mkdir tbs_toolchains
                cd tbs_toolchains/
                mkdir realtek
                cd realtek/
				
        4), copy the “rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2” to the directory “/opt/tbs_toolchains/realtek”.  
            use command:
                cp /root/rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2 /opt/tbs_toolchains/realtek
		
        5), untar “rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2” in the directory “/opt/tbs_toolchains/realtek”.  
            use command:
                tar -jxf rsdk-1.5.6-5281-EB-2.6.30-0.9.30.3-110915.tar.bz2
		
        6), go to directory “/root/” .  
            use command:
                cd /root/
		
        7), untar GPL “DSL2750U_U1_GPL_ME_1.09_ME.tar.bz2”.  
            use command:
                tar -jxf DSL2750U_U1_GPL_ME_1.09_ME.tar.bz2
		
        8), Set execute permissions. 
            use command:
                chmod 777 DSL2750U_U1_GPL_ME_1.09_ME -R
		
        9), go to directory "DSL2750U_U1_GPL_ME_1.09_ME/RTL8676xx/". 
            use command:
                cd DSL2750U_U1_GPL_ME_1.09_ME/RTL8676xx/
		
        10), make GPL.
            use command: 
                make autobuild AUTOPRODUCT=DSL2750U 

    b) The final image is under directory: ./build/romfs/image.img. image.bin include the image.img and boot-loader.

5.How to install?
    a) Upgrade image.img via router's Web UI. (please refer your user manual)

	   *NOTE*, I have to say, if you modified something and cause some errors,
	   the device may not be able to restart again.
	   
	   So, PLEASE MAKE SURE YOU REALLY KNOW WHAT YOU ARE DOING BEFORE YOU
	   UPGRADE ANY UNOFFICIAL FIRMWARE.
	   
6.Some hints
	The plateform we used on this project is mips, yeah, you might have
	known that after unpacking the cross compiler.
	It means, every change you want to make, could be verified in PC
	first. It's important, or you may need to restore the flash image
	much often.
	
7.Modules developed by us
	logic --- it is in the path : DSL2750U_U1_GPL_ME_1.09_ME/customer/apps/logic
	apps --- it is in the path :  DSL2750U_U1_GPL_ME_1.09_ME/RTL8676xx/src/apps
	
