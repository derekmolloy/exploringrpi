#!/usr/bin/perl
# A small Perl script to set up GPIO4 LED to be turned on or off from 
#  Linux console. Written by Derek Molloy (derekmolloy.ie) for the 
#  book Exploring Raspberry Pi.

# Next line is required to import the usleep() function.
use Time::HiRes qw(usleep);  

$LED4_PATH = "/sys/class/gpio/gpio4/";
$GPIO_SYSFS = "/sys/class/gpio/";
$GPIO_NUMBER = "4";
$command = $ARGV[0];

# Perl Write to Sysfs function, the path  $_[0] is the first argument
#   the filename is the second argument $_[1]  and the value to write 
#   is the third argument $_[2]
sub writeSysfs{
  #  print("Debug: Writing to $_[0]$_[1]\n");
  open(FILE, ">" . $_[0] . $_[1] )
     or die "Could not open the file, $!";
  print FILE $_[2] ;
  close(FILE);
}

print "Starting the LED Perl Script\n";
# 0 means that there is exactly one argument
if ( $#ARGV != 0 ){
  print "There are no arguments. Usage is:\n";
  print " bashLED Command, where command is one of\n";
  print "   setup, on, off, status, or close e.g. bashLED on\n";
  exit 2;
}
print "The LED Command that was passed is: " . $command . "\n";
if ( $command eq "on" ){
  print "Turning the LED on\n";
  writeSysfs ($LED4_PATH, "value", "1");
}
elsif ( $command eq "off" ){
  print "Turning the LED off\n";
  writeSysfs ($LED4_PATH, "value", "0");
}
elsif ( $command eq "setup" ){
  print "Exporting the GPIO entry\n";
  writeSysfs ($GPIO_SYSFS, "export", $GPIO_NUMBER);
  usleep(100000);
  writeSysfs ($LED4_PATH, "direction", "out");
}
elsif ( $command eq "close" ){
  print "Unexporting the GPIO entry\n";
  writeSysfs ($GPIO_SYSFS, "unexport", $GPIO_NUMBER);
}
elsif ( $command eq "status" ){
  open(DATA, "<". $LED4_PATH . "value");
  while(<DATA>){
    print "$_";
  }
  close(DATA);
}
print "End of the LED Perl Script\n";
