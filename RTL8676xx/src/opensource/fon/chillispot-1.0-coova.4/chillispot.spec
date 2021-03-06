Summary:   ChilliSpot is a Wireless LAN Access Point Controller
Name:      chillispot
Version:   1.0
Release:   coova.4 
URL:       http://coova.org/wiki/index.php/CoovaChilli
Source0:   %{name}-%{version}-%{release}.tar.gz
License:   GPL
Group:     System Environment/Daemons
BuildRoot: %{_tmppath}/%{name}-root

%description 

ChilliSpot is an open source captive portal or wireless LAN access point
controller. It supports web based login which is today's standard for
public HotSpots and it supports Wireless Protected Access (WPA) which
is the standard of the future. Authentication, Authorization and 
Accounting (AAA) is handled by your favorite radius server. Read more
on http://www.chillispot.org

For more information on this Coova release of ChilliSpot, visit
http://coova.org/

%prep
%setup -n chillispot-1.0-coova.4 

%build

./configure --prefix=/usr --enable-static-exec

make

%install

make install prefix=$RPM_BUILD_ROOT/usr
strip $RPM_BUILD_ROOT/usr/sbin/chilli
strip $RPM_BUILD_ROOT/usr/sbin/chilli_response
strip $RPM_BUILD_ROOT/usr/sbin/chilli_radconfig
strip $RPM_BUILD_ROOT/usr/sbin/chilli_query
strip $RPM_BUILD_ROOT/usr/sbin/test_radius

#Copy chilli init script in place
mkdir -p $RPM_BUILD_ROOT/etc/rc.d/init.d
install -m755 doc/chilli.init $RPM_BUILD_ROOT/etc/rc.d/init.d/chilli

#Copy chilli.conf in place
mkdir -p $RPM_BUILD_ROOT/etc/chilli
install -m755 conf/chilli.conf $RPM_BUILD_ROOT/etc/chilli.conf
#install -m755 conf/shape.sh $RPM_BUILD_ROOT/etc/chilli/shape.sh
install -m755 conf/firewall $RPM_BUILD_ROOT/etc/chilli/firewall
install -m755 conf/functions $RPM_BUILD_ROOT/etc/chilli/functions
install -m644 conf/defaults $RPM_BUILD_ROOT/etc/chilli/defaults
mkdir -p $RPM_BUILD_ROOT/etc/chilli/www
install -m644 conf/coova.html $RPM_BUILD_ROOT/etc/chilli/www/coova.html
install -m644 conf/coova.jpg $RPM_BUILD_ROOT/etc/chilli/www/coova.jpg

#Clean up unwanted library files
rm -rf $RPM_BUILD_ROOT/usr/include/*
rm -rf $RPM_BUILD_ROOT/usr/lib/*


%clean
rm -rf $RPM_BUILD_ROOT
make clean

%post
/sbin/chkconfig --del chilli
/sbin/chkconfig --add chilli

%files
%defattr(-,root,root)

%attr(755, root, root) /usr/sbin/chilli
%attr(755, root, root) /usr/sbin/chilli_response
%attr(755, root, root) /usr/sbin/chilli_radconfig
%attr(755, root, root) /usr/sbin/chilli_query
%attr(755, root, root) /usr/sbin/test_radius
%attr(755, root, root) /etc/rc.d/init.d/chilli

%doc conf/chilli.conf
%doc doc/chilli.init
%doc conf/firewall
%doc doc/hotspotlogin.cgi
%doc doc/dictionary.chillispot
%doc COPYING

%doc /usr/man/man8/chilli.8.gz

%config /etc/chilli.conf
%config /etc/chilli/firewall
%config /etc/chilli/functions
%config /etc/chilli/defaults
%config /etc/chilli/www/coova.html
%config /etc/chilli/www/coova.jpg

%changelog
* Wed Feb 07 2007  <david@coova.com>
- 1.0-coova.4 release
* Wed Nov 15 2006  <david@coova.com>
- 1.0-coova.3 release
* Thu Mar 25 2004  <support@chillispot.org>
- Initial release.
