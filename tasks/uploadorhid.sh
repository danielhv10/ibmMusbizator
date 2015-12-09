#!/bin/bash 
#Before use this script you need dfu-programmer and dfu-util library
#********sudo apt-get install dfu-programmer dfu-util*********#
#this script requires correct user permissions over the physical device

arg1=$1

#
upload_script(){
	echo "step1"
	sudo dfu-programmer atmega16u2 erase
	echo "step2"
	sudo dfu-programmer atmega16u2 flash --debug 1 ./libraries/dfu/Arduino-usbserial-uno.hex
	sudo dfu-programmer atmega16u2 reset
}

hid(){
	echo "step1"
	sudo dfu-programmer atmega16u2 erase
	echo "step2"
	sudo dfu-programmer atmega16u2 flash --debug 1 ./libraries/dfu/Arduino-keyboard-0.3.hex
	echo "step3"
	sudo dfu-programmer atmega16u2 reset
}

read_options(){
	case $arg1 in
		"-u") 		upload_script ;;
		"--upload") upload_script ;;
		"-h")		hid ;;
		"--human")	hid ;;
		*) echo "uploadorhid [argument]  -u --upload -h --human" 
	esac 
}

read_options