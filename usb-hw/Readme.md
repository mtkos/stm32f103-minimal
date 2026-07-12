# the usb-hw program

The USB peripheral is less easy to explain than for instance GPIO or UART. The reason is that USB is a protocol. Communication is done by sending packets back and forth in a very specific way.
A concise introduction to USB can be found in <https://beyondlogic.org/usbnutshell/usb1.shtml/> The USB specification itself in <https://www.usb.org/sites/default/files/usb_20_20250603.zip/>.
I have chosen to write three programs using the USB peripheral, of increasing complexity. The present one requires almost no knowledge of the USB protocol. The peripheral
(chapter 23 of the reference manual) takes care of low level tasks like creating token and data packets, which simplifies programming a little. A few things are worth noting:

1. Data which have been received or need to be sent are stored in a 512-byte piece of SRAM called the packet memory area. It is described in section 23.4.2 and 23.5.3 of the reference manual.
The organization of this PMA is a bit complicated. It consists of half-words (16 bit), located at addresses 0x40006000, 0x40006004, ..., 0x400063fc (reference manual 3.3, table 3). The two most
significant bytes at these addresses can be considered to be 0. Furthermore, the USB protocol is byte-oriented (not half-word-oriented). For these reasons the function pma\_to\_buffer
is more complicated than one might expect.

2. When a correct transfer has taken place, the CTR bit of the ISTR is set. Whether it was a receive or a send can be seen by inspecting the relevant endpoint register (CTR\_RX or CTR\_TX bit).
Clearing  CTR\_RX or CTR\_TX also clears CTR.

3. After a correct TX transfer the STAT\_TX bits in the relevant endpoint register are set to NAK, giving the application time for further processing. When the application has finished doing that,
these bits should be set to VALID again. The reference manual (23.5.2) discourages doing this via a read-modify-write cycle. Fortunately, this is not necessary because we know the STAT\_TX bits.
The same holds for RX.

4. So, for instance, the last line of handle\_ep0\_setup means: clear the CTR\_RX bit, keep the CTR\_TX bit, toggle the STAT\_TX bits from NAK to VALID and keep the STAT\_RX bits.

5. The program receives only 8-byte control packets. It sends back zero-length packets. For SET requests this is what you often want. For GET requests it makes sense because sending nothing
causes the host controller to keep sending GET requests. It now tries 4 times to GET a device descriptor, after which it gives up and tries 2 times to SET the device address. I do not know if this
is standard or documented behaviour, so on your computer things may work differently. The utility test.py reads the requests from the serial interface and shows them in hex.

To use the program you will have to physically disconnect and reconnect the USB cable (keeping the BOOT0 pin low, of course). Just hitting the reset button will produce no output.
