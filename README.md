# Particle-compressFile

A Particle library to compress files on an SDCard using the [Heatshrink](https://github.com/atomicobject/heatshrink) compression technique.
This compression is fast and uses a low amount of RAM.


## Getting started

Connect an SDcard to your Particle device. Setup the right pins and SD card configuration in the example and run it by compiling and flashing the files to a particle device.
Connect to the particle using a terminal/serial monitor and let it do its magic.

See the [examples](examples) folder for more details.

## Documentation

After compressing a file, it can be decompressed on a linux or windows machine using the [Heatshrink](https://github.com/atomicobject/heatshrink) program. Use the Linux version already build in the [bin](bin) folder or clone the Heatshrink repository and run make to create a binary that can be run. On a linux computer, after the binary is created, you can use "heatshrink -d -w 10 -l 4 $compressedname $name" to decompress your file. I recommend placing the heatshrink file in /usr/local/bin so you can use it everywhere on the command line. 

On the [Heatshrink blog](https://spin.atomicobject.com/2013/03/14/heatshrink-embedded-data-compression/) a good description and overview of the different settings in the heatshrink_config.h are given. The HS 1,4 compression that is the standard setting gives a compression of about 50%.


This library is used by the FTP file send library I also created to send the compressed file over FTP to an ftp server. This library will be uploaded as soon as this one is done.


## Contributing
There is no option to uncompress files for now on the particle device. It might be nice to add this functionality as well.
Furthermore, all space allocation is static for now, about 2k of RAM space is used. This can probably be improved by dynamic allocation.

## LICENSE
Copyright 2017 wvdv2002

Licensed under the ISC license
