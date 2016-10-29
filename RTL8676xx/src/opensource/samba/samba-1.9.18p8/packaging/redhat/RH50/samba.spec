Summary: SMB client and server
Name: samba
%define	version 1.9.18p8
Version: %{version}
Release: 50.1
Copyright: GPL
Group: Networking
Source: ftp://samba.anu.edu.au/pub/samba/samba-%{version}.tar.gz
Patch: samba-make.patch
Packager: John H Terpstra [Samba-Team] <jht@samba.anu.edu.au>
Requires: pam >= 0.59
BuildRoot: /tmp/samba

%description
Samba provides an SMB server which can be used to provide
network services to SMB (sometimes called "Lan Manager")
clients, including various versions of MS Windows, OS/2,
and other Linux machines. Samba also provides some SMB
clients, which complement the built-in SMB filesystem
in Linux. Samba uses NetBIOS over TCP/IP (NetBT) protocols
and does NOT need NetBEUI (Microsoft Raw NetBIOS frame)
protocol.

This release is known as the "Locking Update" and has full
support for Opportunistic File Locking. In addition this update
includes native support for Microsoft encrypted passwords,
improved browse list and WINS database management.

Please refer to the WHATSNEW.txt document for fixup information.
This binary release includes encrypted password support.
Please read the smb.conf file and ENCRYPTION.txt in the
docs directory for implementation details.

NOTE: Red Hat Linux 5.0 Uses PAM which has integrated support
for Shadow passwords. Do NOT recompile with the SHADOW_PWD option
enabled. Red Hat Linux has built in support for quotas in PAM.

%changelog
* Sat Jun 13 1998 John H Terpstra <jht@samba.anu.edu.au>
  - Added code pages 737 and 861 to files section
  - Added auto-generation of empty /etc/lmhosts file if not exist
  - Always zap and create empty /var/lock/samba/STATUS..LCK file

* Wed Jun 10 1998 John H Terpstra <jht@samba.anu.edu.au>
  - Updated version info for 1.9.18p8 release
  - updated codepage support for pages 737 861

* Sun Apr 26 1998 John H Terpstra <jht@samba.anu.edu.au>
  - minor tidy up in preparation for release of 1.9.18p5
  - added findsmb utility from SGI package

* Wed Mar 18 1998 John H Terpstra <jht@samba.anu.edu.au>
  - Updated version and codepage info.
  - Release to test name resolve order

* Sat Jan 24 1998 John H Terpstra <jht@samba.anu.edu.au>
 - Many optimisations (some suggested by Manoj Kasichainula <manojk@io.com>
  - Use of chkconfig in place of individual symlinks to /etc/rc.d/init/smb
  - Compounded make line
  - Updated smb.init restart mechanism
  - Use compound mkdir -p line instead of individual calls to mkdir
  - Fixed smb.conf file path for log files
  - Fixed smb.conf file path for incoming smb print spool directory
  - Added a number of options to smb.conf file
  - Added smbadduser command (missed from all previous RPMs) - Doooh!
  - Added smbuser file and smb.conf file updates for username map

%prep
%setup
%patch -p1

%build
cd source
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS" all wsmbconf

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/etc/codepages/src
mkdir -p $RPM_BUILD_ROOT/etc/{logrotate.d,pam.d}
mkdir -p $RPM_BUILD_ROOT/etc/rc.d/{init.d,rc0.d,rc1.d,rc2.d,rc3.d,rc5.d,rc6.d}
mkdir -p $RPM_BUILD_ROOT/home/samba
mkdir -p $RPM_BUILD_ROOT/usr/{bin,sbin}
mkdir -p $RPM_BUILD_ROOT/usr/man/{man1,man5,man7,man8}
mkdir -p $RPM_BUILD_ROOT/var/lock/samba
mkdir -p $RPM_BUILD_ROOT/var/log/samba
mkdir -p $RPM_BUILD_ROOT/var/spool/samba

for i in nmblookup smbclient smbpasswd smbrun smbstatus testparm testprns
do
install -m755 -s source/$i $RPM_BUILD_ROOT/usr/bin
done

for i in addtosmbpass mksmbpasswd.sh smbtar make_smbcodepage make_printerdef
do
install -m755 source/$i $RPM_BUILD_ROOT/usr/bin
done

for i in smbd nmbd wsmbconf
do
install -m755 -s source/$i $RPM_BUILD_ROOT/usr/sbin
done

for i in smbclient.1 smbrun.1 smbstatus.1 smbtar.1 testparm.1 testprns.1 make_smbcodepage.1
do
install -m644 docs/$i $RPM_BUILD_ROOT/usr/man/man1
done

for i in 437 737 850 852 861 866 932 949 950 936
do
install -m644 source/codepage_def.$i $RPM_BUILD_ROOT/etc/codepages/src
done

install -m644 docs/smb.conf.5 $RPM_BUILD_ROOT/usr/man/man5
install -m644 docs/samba.7 $RPM_BUILD_ROOT/usr/man/man7
install -m644 docs/smbd.8 $RPM_BUILD_ROOT/usr/man/man8
install -m644 docs/nmbd.8 $RPM_BUILD_ROOT/usr/man/man8
install -m644 docs/smbpasswd.8 $RPM_BUILD_ROOT/usr/man/man8
install -m644 packaging/redhat/RH50/smb.conf $RPM_BUILD_ROOT/etc/smb.conf
install -m644 packaging/redhat/RH50/smbusers $RPM_BUILD_ROOT/etc/smbusers
install -m755 packaging/redhat/RH50/smbprint $RPM_BUILD_ROOT/usr/bin
install -m755 packaging/redhat/RH50/smbadduser $RPM_BUILD_ROOT/usr/bin
install -m755 packaging/redhat/RH50/findsmb $RPM_BUILD_ROOT/usr/bin
install -m755 packaging/redhat/RH50/smb.init $RPM_BUILD_ROOT/etc/rc.d/init.d/smb
install -m755 packaging/redhat/RH50/smb.init $RPM_BUILD_ROOT/usr/sbin/samba
install -m644 packaging/redhat/RH50/samba.pamd $RPM_BUILD_ROOT/etc/pam.d/samba
install -m644 packaging/redhat/RH50/samba.log $RPM_BUILD_ROOT/etc/logrotate.d/samba

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/chkconfig --add smb
for i in 437 737 850 852 861 866 932 949 950 936
do
/usr/bin/make_smbcodepage c $i /etc/codepages/src/codepage_def.$i /etc/codepages/codepage.$i
done
if [ ! -e /etc/lmhosts ]; then
  cp /dev/null /etc/lmhosts
fi
if [ -x /var/lock/samba/STATUS..LCK ]; then
  rm -f /var/lock/samba/STATUS..LCK
fi
cp /dev/null /var/lock/samba/STATUS..LCK
chown root.root /var/lock/samba/STATUS..LCK
chmod 644 /var/lock/samba/STATUS..LCK

%preun
/sbin/chkconfig --del smb

%postun
if [ -x /etc/pam.d/samba ]; then
  rm -f /etc/pam.d/samba
fi
if [ -e /etc/codepages ]; then
  rm -rf /etc/codepages
fi
if [ -e /var/log/samba ]; then
  rm -rf /var/log/samba
fi
if [ -e /var/lock/samba ]; then
  rm -rf /var/lock/samba
fi

%files
%attr(-,root,root) %doc README COPYING Manifest Read-Manifest-Now
%attr(-,root,root) %doc WHATSNEW.txt Roadmap
%attr(-,root,root) %doc docs
%attr(-,root,root) %doc examples
%attr(-,root,root) /usr/sbin/smbd
%attr(-,root,root) /usr/sbin/nmbd
%attr(2755,root,root) /usr/sbin/wsmbconf
%attr(-,root,root) /usr/bin/addtosmbpass
%attr(-,root,root) /usr/bin/mksmbpasswd.sh
%attr(-,root,root) /usr/bin/smbclient
%attr(-,root,root) /usr/bin/testparm
%attr(-,root,root) /usr/bin/testprns
%attr(-,root,root) /usr/bin/smbrun
%attr(-,root,root) /usr/bin/findsmb
%attr(-,root,root) /usr/bin/smbstatus
%attr(-,root,root) /usr/bin/nmblookup
%attr(-,root,root) /usr/bin/make_smbcodepage
%attr(-,root,root) /usr/bin/make_printerdef
%attr(-,root,root) /usr/bin/smbpasswd
%attr(-,root,root) /usr/bin/smbtar
%attr(-,root,root) /usr/bin/smbprint
%attr(-,root,root) /usr/bin/smbadduser
%attr(-,root,root) %config /etc/smb.conf
%attr(-,root,root) %config /etc/smbusers
%attr(-,root,root) %config /etc/rc.d/init.d/smb
%attr(755,root,root) %config /usr/sbin/samba
%attr(-,root,root) %config /etc/logrotate.d/samba
%attr(-,root,root) %config /etc/pam.d/samba
%attr(-,root,root) %config /etc/codepages/src/codepage_def.437
%attr(-,root,root) %config /etc/codepages/src/codepage_def.737
%attr(-,root,root) %config /etc/codepages/src/codepage_def.850
%attr(-,root,root) %config /etc/codepages/src/codepage_def.852
%attr(-,root,root) %config /etc/codepages/src/codepage_def.861
%attr(-,root,root) %config /etc/codepages/src/codepage_def.866
%attr(-,root,root) %config /etc/codepages/src/codepage_def.932
%attr(-,root,root) %config /etc/codepages/src/codepage_def.936
%attr(-,root,root) %config /etc/codepages/src/codepage_def.949
%attr(-,root,root) %config /etc/codepages/src/codepage_def.950
%attr(-,root,root) /usr/man/man1/smbstatus.1
%attr(-,root,root) /usr/man/man1/smbclient.1
%attr(-,root,root) /usr/man/man1/make_smbcodepage.1
%attr(-,root,root) /usr/man/man1/smbrun.1
%attr(-,root,root) /usr/man/man1/smbtar.1
%attr(-,root,root) /usr/man/man1/testparm.1
%attr(-,root,root) /usr/man/man1/testprns.1
%attr(-,root,root) /usr/man/man5/smb.conf.5
%attr(-,root,root) /usr/man/man7/samba.7
%attr(-,root,root) /usr/man/man8/smbd.8
%attr(-,root,root) /usr/man/man8/nmbd.8
%attr(-,root,root) /usr/man/man8/smbpasswd.8
%attr(-,root,nobody) %dir /home/samba
%attr(-,root,root) %dir /etc/codepages
%attr(-,root,root) %dir /etc/codepages/src
%attr(-,root,root) %dir /var/lock/samba
%attr(-,root,root) %dir /var/log/samba
%attr(777,root,root) %dir /var/spool/samba
