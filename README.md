# Hall Light Control
I used an Arduino Diecimila, a 
[Sparkfun WiFly shield](http://www.sparkfun.com/products/9954), 
and a [Sparkfun LED strip](http://www.sparkfun.com/products/10260) to light up my hallway.

The circuit uses three MOSFETs with very low Rdson, connected to pins 3 (red), 5 (green), and 6 (blue).

led_control.sch/.brd are the Eagle files, led_control_schematic.png is the schematic exported for those
who don't own eagle, and led_control.pdf is an image of the board suitable for toner transfer.

# Usage
You'll need to edit Credentials.h with the appropriate definitions:

* passphrase

  The passphrase with which to associate to your wireless lan, if needed.

  * ssid

    Self explanatory

Once you've done that, plug your Arduino into an external power supply 
	  (the regulator won't drive the LED strip).  
