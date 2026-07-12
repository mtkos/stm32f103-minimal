# the usb-kbd program

This program adds some code to the usb-minimal program to create something which looks a little bit more like a real USB device. Because I wanted to avoid writing code for the host computer
(with libusb) I chose to program a simple HID keyboard-like device, which requires no further driver or client software. After loading the program, the STM32F103 presents itself as a
USB keyboard which sends, at regular intervals, the character 'b'. The program is a straightforward extension of the usb-minimal program, adding an extra endpoint for sending 'keystrokes'
and extra functions for handling HID requests. The necessary extra information for understanding this program can be found in the USB HID specification:
<https://www.usb.org/document-library/device-class-definition-hid-111/>. Specifically, section 7.1: class descriptors 0x21 and 0x22, 7.1.1: HID Class descriptor get request 0x81/0x06. It is also
useful to read section 7.2. Of course, it is also necessary to add the relevant descriptors. They are described in Appendices E.1 through E.5 of the specification, from which they have been
taken without much modification. The report descriptor has been taken from appendix E.6, but has been heavily trimmed down. No modifier keys, no reporting of simultaneously pressed keys, and no
LEDs. To make things a little more interesting some string descriptors have been added. You will see them if you use the -v option of lsusb. For informational purposes, the file usbmon.txt shows
the USB traffic (recorded using usbmon) taking place when the device is connected.
