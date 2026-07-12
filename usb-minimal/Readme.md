# the usb-minimal program

This program uses the functions from the usb-hw program to create a minimal USB device. It gets enumerated, so that it will show up in the output of lsusb, but that is all it does. The numerical
codes used in the program can be found in the USB specification:

1. table 9-5 gives the descriptor types, among which device, configuration and string are important to our programs

2. table 9-3 gives the values 0x0 and 0x80 for set/get descriptor

3. table 9-4 gives values like SET_ADDRESS = 5

4. Note that the device address is set in handle\_ep0\_in, which is called after the zero length packet confirming the reception of a SET\_ADDRESS has been sent, and not in handle\_set\_request as
one might expect. Citing from 'USB in a nutshell':
"Set Address is used during enumeration to assign a unique address to the USB device. The address is specified in wValue and can only be a maximum of 127. This request is unique in that the
device does not set its address until after the completion of the status stage. All other requests must complete before the status stage."

5. In the USB specification there is a lot of talk about (very complicated) 'state machines'. In the present simple program the value of the global variable ctrl\_request,
together with the peripheral register values, contain enough information, making it unnecessary to maintain further state.

6. The program uses three descriptors: device, configuration and interface. They are described in section 9.6 of the USB specification.

7. The program never needs to transmit more than 64 bytes at once. The transmit buffer is large enough for this, so the code for handle\_get\_request can be very simple.

8. The only interrupts enabled are RESET and CTR. No error handling, suspend/resume etc.

I will not try to explain the USB control flow in this program in detail, because that is not the purpose of the present repository. But after reading 'USB in a nutshell' it should be not difficult
to understand.
