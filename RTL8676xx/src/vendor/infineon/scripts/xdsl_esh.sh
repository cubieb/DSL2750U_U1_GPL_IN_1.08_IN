[WaitForLinkActivate]={
echo Processing Pilot tones in Extended script section...
cr optn 9 0 $0
mov $1 0x100
or $0 $1
cw optn 9 0 $0
}
