#!/usr/bin/perl

# This perl script generates the samba.spec file based on the version
# information in the version.h file in the source tree

open (VER,'../../source/version.h') || die "Unable to open version.h\n";
($_ = <VER>) =~ s/"//g;
close (VER);
@foo = split(' ');
splice(@foo,0,2);
$_ = $foo[0];

# create the package name
$vername = "    id \"Samba Version ".$_."\"\n";

$patch = 0;
#create the subsystem version numbers
if (/alpha/) {
  $_ =~ s/alpha/.00./;
}
elsif (/p/) {
  $_ =~ s/p/./;
  $_ .= '.00';
  $patch = 1;
}
else {
 $_ .='.01.00';
}

($v1,$v2,$v3,$v4,$v5) = split('\.');
$v4 = $v4 + $patch;
$vernum = sprintf("        version %02d%02d%02d%02d%02d\n",$v1,$v2,$v3,$v4,$v5);

# generate the samba.spec file
open(SPEC,">samba.spec") || die "Unable to open samba.spec for output\n";
print SPEC "product samba\n";
print SPEC $vername;
print SPEC "    image sw\n";
print SPEC "        id \"Samba Execution Environment\"\n";
print SPEC $vernum;
print SPEC "        order 0\n";
print SPEC "        subsys base default\n";
print SPEC "            id \"Samba Execution Environment\"\n";
print SPEC "            replaces fw_samba.sw.base 0 9999999999\n";
print SPEC "            replaces fw_samba.sw.samba 0 9999999999\n";
print SPEC "            exp samba.sw.base\n";
print SPEC "        endsubsys\n";
print SPEC "    endimage\n";
print SPEC "    image man\n";
print SPEC "        id \"Samba Online Documentation\"\n";
print SPEC $vernum;
print SPEC "        order 1\n";
print SPEC "        subsys manpages default\n";
print SPEC "            id \"Samba Man Page\"\n";
print SPEC "            replaces fw_samba.man.manpages 0 9999999999\n";
print SPEC "            replaces fw_samba.man.samba 0 9999999999\n";
print SPEC "            exp samba.man.manpages\n";
print SPEC "        endsubsys\n";
print SPEC "        subsys doc default\n";
print SPEC "            id \"Samba Documentation\"\n";
print SPEC "            replaces fw_samba.man.doc 0 9999999999\n";
print SPEC "            exp samba.man.doc\n";
print SPEC "        endsubsys\n";
print SPEC "        subsys relnotes default\n";
print SPEC "            id \"Samba Release Notes\"\n";
print SPEC "            replaces fw_samba.man.relnotes 0 9999999999\n";
print SPEC "            exp samba.man.relnotes\n";
print SPEC "        endsubsys\n";
print SPEC "    endimage\n";
print SPEC "    image src\n";
print SPEC "        id \"Samba Source Code\"\n";
print SPEC $vernum;
print SPEC "        order 2\n";
print SPEC "        subsys samba\n";
print SPEC "            id \"Samba Source Code\"\n";
print SPEC "            replaces fw_samba.src.samba 0 9999999999\n";
print SPEC "            exp samba.src.samba\n";
print SPEC "        endsubsys\n";
print SPEC "    endimage\n";
print SPEC "endproduct\n";
close SPEC || die "Error on close of samba.spec\n";

print "\nsamba.spec file has been created\n\n";
