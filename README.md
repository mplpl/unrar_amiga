# Port of unrar for MorhpOS and Amiga OS4

<h1>State of work:

<h2>Basic functions

All the basic functions like listing (verbose, base, technical), unacking (w/ or w/o path, selected files or all), testing or printing works fine. Note that wherever a path is needed that has to be in amiga format not POSIX ie. //test.rar not ../../test.rar.

<h2>National characters in file names

RAR stores file names in Unicode format and uses it in console printing or file operation (create, find, ...) in UTF-8. Unfortunately no Amiga operating system support UTF-8 today therefore in order to handle localized names, this unrar port converts names to local 8-bit encoding set in locale prefs in OS. Unfortunately, Amiga locale library does not report selected code page. Fortunately, but both MorphOS and AmigaOS4 sets appropriate environment variable:
* on Morphos: CODEPAGE
* on AmigaOS4: ???

Obviously, in order to see national characters in shell or when browsing files in Ambient/Workbench you need to have fonts with the right encoding installed in OS and set as system font and/or in Ambient.

<h2>File modification dates

When unpacking, created files get modification date set as stored in RAR archive. The data is set in the local time zone. As there is no special attribute for creation data in AmigaOS, this value is always ignored.

<h2>File owner and group

When unpacking from RAR archive with option -ow, unrar will try to set owner name and group. It will only success if user with given name and group with given name is present in local system. Otherwise an error will be printed. When -ow is not used, owner/group is ignored.

<h2>Symbolic links in an archive

RAR can store symbolic links and this unrar port supports that. When unpacking, unix symbolic links will be changed into Amiga links (Makelink function) provided that filesystem into which unpack is done supports them (like SFS). When the filesystem does not support symbolic links, an error will be printed and the sympolic link will be ignored.
As RAR stores symbolic link target in the form that is right on OS where an archive was created, before making links in Amiga, the path need to be converted (for instance ../test.txt need to be changed to /test.txt). This unrar port does such conversion.

Note: unrar check if a link is safe to create and will skip links to outside of enpacking directory. In order to make it do them, you need to use -ola switch when unpacking.

<h2>Hard links in an archive

Hard links are supported in a similar way as symbilic links. Again, they will only work if target file system supports symbolic links. Otherwise (like in case of SFS that does not support hard links) an error will be printed and hardlink will be ignored. There is no option in urar that would allow you to unpack hardlinks and regular files or symbolic links - so if target file system does not support them it is not possible to get them unpacked. 

<h2>Reference files in an archive

RAR can pack identical files as referenced inside of an archive - that reduces size of targer file. In order to make such archive -oi switch should be used. When unpacking, duplicates are re-created as separate files. That is fully supported in this port.
