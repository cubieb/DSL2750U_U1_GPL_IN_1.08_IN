#!/usr/bin/perl

# only works on LINUX (not for Windows)
# this perl script does the following:
# 1. copy input file to output file.
# 2. pads output file to filesize of multiple of four bytes. 

use strict;

use File::stat;


#********************************************************************************************

$ARGV[0] || go_die();
$ARGV[1] || go_die();

my $infile = $ARGV[0];
my $outfile = $ARGV[1];

my $stat;

my $OUTPUTFILE;

#********************************************************************************************


if ($stat = stat $infile) {
    print "$infile is ", $stat->size, " bytes\n";
} else {
    go_die("Can't open $infile for read", 1);
}

system ("cp $infile $outfile");

open ($OUTPUTFILE, ">>$outfile") || go_die("Can't open $outfile for write",1);

binmode $OUTPUTFILE;

my $pad_cnt = $stat->size % 4;

if ($pad_cnt != 0)
{
    $pad_cnt = 4 - $pad_cnt;
}

while ($pad_cnt > 0)
{
    print $OUTPUTFILE 0;
    $pad_cnt--;
}

close ($OUTPUTFILE);

if ($stat = stat $outfile) {
    print "$outfile is ", $stat->size, " bytes\n";
} else {
    go_die("Can't open $outfile for read", 1);
}

#********************************************************************************************

sub go_die
{
    my($message, $supressformat) = @_;

    if ($message)
    {
        print "$message";
    }

    close (INFILE);
    close (OUTFILE);

    unless ($supressformat)
    {
        print "format is: \n";
        print " perl padfour.pl u-boot.ifx ubootifx.bin\n";
    }
    else
    {
       print "----------------------------------------------------------\n";
    }

    die ("\n");
}

