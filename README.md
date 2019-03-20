# Port of unrar for MorphOS and AmigaOS 4

<h2>State of work:</h2>

<h3>Basic functions</h3>

All the basic functions like listing (verbose, base, technical), unacking (w/ or w/o path, selected files or all), testing or printing works fine. Note that wherever a path is needed that has to be in amiga format not POSIX ie. //test.rar not ../../test.rar.

<h3>National characters in file names</h3>

RAR stores file names in Unicode format and uses it in console printing or file operation (create, find, ...) in UTF-8. Unfortunately no Amiga operating system support UTF-8 today therefore in order to handle localized names, this unrar port converts names to local 8-bit encoding set in locale prefs in OS. Unfortunately, Amiga locale library does not report selected code page. Fortunately, but both MorphOS and AmigaOS4 sets appropriate environment variable:
* on Morphos: CODEPAGE
* on AmigaOS4: ???

Obviously, in order to see national characters in shell or when browsing files in Ambient/Workbench you need to have fonts with the right encoding installed in OS and set as system font and/or in Ambient.

<h3>File modification dates</h3>

When unpacking, created files get modification date set as stored in RAR archive. The data is set in the local time zone. As there is no special attribute for creation data in AmigaOS, this value is always ignored.

<h3>File owner and group</h3>

When unpacking from RAR archive with option -ow, unrar will try to set owner name and group. It will only success if user with given name and group with given name is present in local system. Otherwise an error will be printed. When -ow is not used, owner/group is ignored.

<h3>Symbolic links in an archive</h3>

RAR can store symbolic links and this unrar port supports that. When unpacking, unix symbolic links will be changed into Amiga links (Makelink function) provided that filesystem into which unpack is done supports them (like SFS). When the filesystem does not support symbolic links, an error will be printed and the sympolic link will be ignored.
As RAR stores symbolic link target in the form that is right on OS where an archive was created, before making links in Amiga, the path need to be converted (for instance ../test.txt need to be changed to /test.txt). This unrar port does such conversion.

Note: unrar check if a link is safe to create and will skip links to outside of enpacking directory. In order to make it do them, you need to use -ola switch when unpacking.

<h3>Hard links in an archive</h3>

Hard links are supported in a similar way as symbilic links. Again, they will only work if target file system supports symbolic links. Otherwise (like in case of SFS that does not support hard links) an error will be printed and hardlink will be ignored. There is no option in urar that would allow you to unpack hardlinks and regular files or symbolic links - so if target file system does not support them it is not possible to get them unpacked. 

<h3>Reference files in an archive</h3>

RAR can pack identical files as referenced inside of an archive - that reduces size of targer file. In order to make such archive -oi switch should be used. When unpacking, duplicates are re-created as separate files. That is fully supported in this port.

<h2>Notes about porting</h2>

This port is based on unrar source code version 5.7.3 (unrarsrc-5.7.3.tar.gz) from rarlab.com:
https://www.rarlab.com/rar/unrarsrc-5.7.3.tar.gz

For normalizing UTF, I used utf8proc. I did not need to port it for Amiga - it compiles without any change:
https://juliastrings.github.io/utf8proc/

For vfwprintf based functions of libc, that are not present on Amiga at all, I ported the code I took from newlib 3.1.0:
http://sourceware.org/newlib/
ftp://sourceware.org/pub/newlib/newlib-3.1.0.tar.gz

MorphOS version of unrar has been compiled using gcc 4.4.5 (part of SDK 3.12).
AmigaOS4 version of unrar has been compiled using gcc 4.x.x (part of SDK x.xx).

