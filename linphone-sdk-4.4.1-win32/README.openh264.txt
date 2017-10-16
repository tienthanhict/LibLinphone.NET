H264 codec
----------
Mediastreamer 2 uses Cisco's OpenH264 library as H264 codec. OpenH264 is an
open source library but H264 is a coding format protected by a patent from
MPEG-LA. That's why that bundle does not embed any OpenH264 library and so you
will not able to use H264 out-the-box.

Fortunatly, Cisco Systems Inc. distribute OpenH264 in its binary form that you
can use in Mediastreamer without paying much royalties to MPEG-LA. However,
that binary file as a restrictive licence. See http://www.openh264.org/BINARY_LICENSE.txt form more information.


Installation procedure
----------------------
The Cisco's provided OpenH264 library is available at this address:
http://ciscobinary.openh264.org/libopenh264-1.5.0-osx64.dylib.bz2 for MacOS
http://ciscobinary.openh264.org/openh264-1.5.0-win32msvc.dll.bz2 for Windows (32 bit)

Once you get the library, uncompress it into <SDK_location>/lib/libopenh264.1.dylib
On Windows, the library must be installed into <SDK_location>\bin\openh264.dll
