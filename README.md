# Port of unrar for MorphOS, AmigaOS, and AROS

Note: Automatic test package for this unrar port is available at https://github.com/mplpl/unrar_test

<h2>Features</h2>

* support all the basic unrar commands and switches, including listing, 
unpacking (all or selected items), printing and testing archive files

* support national characters in archive name, names of files and directories
in the archive, passwords, comments and list files

* support restoring information about file modification date, owner and group

* support unpacking symbolic links and hard links stored in archives

* support unpacking multi-volume and SFX archives

* can read switches from configuration file s:rar.conf or RAR environment 
variable

* large files support (>4GiB) on MorphOS and AmigsOS4

* support AmigaOS localization via locale.library

<h2>Requirements</h2>

There are no special requirements for MorphOS and AmigaOS4. It should work on any system provided that it has a relatively fresh version of the operating system (I test on MorphOS 3.12 and AmigaOS 4.1 FE Update 1).

AROS version requires i386 and ABI-v0.

Classic AmigaOS version requires a system with:
* CPU Motorola 68000 - no FPU is needed
* Kickstart 2.04 or newer
* ixemul V48 (http://aminet.net/package/util/libs/ixemul-48.0)
* 4MB RAM (unrar needs 2.6MB on its own)

Exact memory utilization depends on the size of compressed files. For instance, my test archive with one 2.0GB file in it needs 32MB of RAM to unpack.

<h2>Details</h2>

<h3>Basic functions</h3>

All the basic functions like listing (verbose, base, technical), unpacking (w/ or w/o path, selected files, or all), testing or printing works fine. Note, that wherever a path is needed it has to be in amiga format, not POSIX i.e. //test.rar not ../../test.rar.

<h3>National characters support</h3>

RAR stores data in UTF-8 in an archive file, then reads it into Unicode (wchar_t) and finally it uses it (print, create files, find files, ...) in UTF-8. Unfortunately, no Amiga operating system supports UTF-8 today, therefore to handle national characters in file names, comments, etc. it needs to be converted to OS local encoding set in OS prefs. Unrar reads what was set using environment variables:
* on MorphOS: CODEPAGE
* on AmigaOS4: Charset
* on AROS: CHARSET
* on AmigaOS3 and AmigaOS2: there is no (read below)

Characters that cannot be converted, because are not present in selected codepage, are replaced with '?'. That means that national characters are fully supported but only for the selected locale, i.e. if I have Polish locale with ISO-8859-2 encoding and have a rar archive with Spanish characters, they will not appear and will be replaced by '?'. That in turns mean that some files may have the same names even if they are different files. In that case, only one will be unpacked. To deal with this, you can use special environment variable RAR_CODEPAGE that allows overriding codepage (for both MorphOS and AmigaOS4) without changing system prefs. For the example above, if you set RAR_CODEPAGE, you will still not see Spanish characters (unless you have the right font) but at least you should get all the files unpacked.

On AmigaOS3 and AmigaOS2, since there is no built-in variable to determine system character encoding, you have to always use RAR_CODEPAGE. To make it slightly simpler, if RAR_CODEPAGE is not set, unrar tries to set encoding based on 'Language' environment variable.

In addition to above, after each text conversion unrar checks if there were any unconvertable characters (that is, characters not present in the target encoding). If so, it will print a warning message, asking the user to set RAR_CODEPAGE.


<h4>National characters in file names</h4>

National characters in filenames are handled as described above - this covers:
* name of files, directories, and links in an archive
* targets for symbolic and hard links in an archive
* archive names that were given to unrar command
* names in list files (if -sc option is not used)

To see national characters in the shell or when browsing unpacked files in Ambient/Workbench you need to have fonts with the right encoding installed in OS and set as system font and/or as Ambient fonts.

<h4>National characters in password</h4>

National characters are supported in passwords. A password can be given in the following forms:
* from the shell when asked
* in command invocation using -p switch 
* from RAR environment variable
* from rar.conf configuration file

In each of the cases above, only passwords containing national characters of the currently selected locale are supported and should be given using locale encoding (as for file names described above). If the password is not using the current locale charset, RAR_CODEPAGE needs to be used to select it.

<h4>-sc switch</h4>

Unrar has -sc<chr>[obj] switch described as "Specify the character set" that is not very useful and not very well documented. From the source code I read that <chr> can be one of: 
* "a" for ANSI, 
* "o" for OEM, 
* "u" for Unicode and 
* "f" for UTF-8. 

For non-Windows systems, only "u" and "f" matter (BTW: "u" is UTF-16 not UTF-32).

[obj] is one of:
* 'c' for comment - does not apply unrar, used only in 'full' rar utility
* 'r' for redirect - applies to Windows only
* 'l' for file lists - applies to file list to unpack (specified after "@"), file list to exclude (specified after -x@) and filter file list (specified after -n@).
  
When -sc switch is not used, unrar will try to autodetect UTF-16 and UTF-8 and when it fails it will assume local encoding. The latter will also happen when "a" or "o" was used.

All above is standard unrar behavior - I'm only documenting it here.

<h3>Wildcards</h3>

Unrar supports wildcards in Windows style:
* "?" = any character
* "*" = any character sequence

Amiga style wildcards are not supported at this moment.

<h3>File modification dates</h3>

When unpacking, created files get modification date set as stored in the RAR archive. The date is set in the local time zone. As there is no special attribute for the creation and access dates in AmigaOS, these values are always ignored.

<h3>File owner and group</h3>

When unpacking from RAR archive with option -ow, unrar will try to set owner name and group. It will only succeed if a user with a given name and a group with a given name is present in the local system. Otherwise, an error will be printed. When -ow is not used, the owner/group is ignored.

<h3>Symbolic links in an archive</h3>

RAR can store symbolic links and this unrar port supports that. When unpacking, targets for Unix symbolic links will be changed into Amiga links (Makelink function) provided that filesystem into which unpack is done supports that (SFS does). When the filesystem does not support symbolic links, an error will be printed and the symbolic link will be ignored.

As RAR stores symbolic link target in the form that is right on OS where an archive was created, before making links in Amiga, the path needs to be converted (for instance ../test.txt need to be changed to /test.txt). This unrar port does such conversion.

Note: unrar checks if a link is safe to create and will skip links targeting outside of the unpacking directory. To make it create them, you need to use -ola switch when unpacking.

<h3>Hard links in an archive</h3>

Hard links are supported in similarly as symbolic links. Again, they will only work if the target filesystem supports hard links (SFS dos not). Otherwise, an error will be printed and hard links will be ignored. No option in unrar would allow the user to unpack hard links as regular files or symbolic links - so if the target filesystem does not support hard links it is not possible to get them unpacked. 

<h3>Reference files in an archive</h3>

RAR can pack identical files as references inside of an archive - that reduces the size of the archive file. To make such an archive -oi switch should be used when packing. When unpacking, duplicates are re-created as separate files. That is fully supported in this unrar port.

<h3>NTFS Junction Points in an archive</h3>

NTFS Junction Points are like symbolic links, but their target is always an absolute path with a drive letter in front. In Unrar version 5.7 for Unix-like systems (including Amiga), these kinds of links are always skipped silently (!). While it is different behavior from how it was in 5.0, I decided to keep it unchanged for Amiga. So, in practice, junction points are always skipped when unpacking.

<h3>Configuration file</h3>

Unrar has a config file in which you can store switches that will always be used when unrar command is called. To do that, "switches=" should be present in the file with all the switches to add (for instance "switches=-ad -ola"). This port of unrar looks for the configuration file in s:rar.conf.

There is also "RAR" environment variable that can have switches to use (like a value of "switches=" in the configuration file). The priority of resolving switches to use is: directly given in the command, RAR environment variable, the configuration file.

<h3>Support for localization</h3>

This unrar port complies with localization rules of MorphOS and AmigaOS. You can find cs/cd file for making additional translations on GitHub.

<h3>Large files support</h3>

On MorphOS and AmigaOS4 large files are supported in the following way:
* a rar file can have size >4GiB
* a file expanded from a rar file may be bigger than 4GiB

The above is true provided that the file system used for unpacking supports files with size >4GiB. 
OFS, FFS, PFS3, SFS, and FAT32 do not support such files.
NTFS, exFAT, and ext2/3/4 (on MorphOS) and SFS2 (on AmigaOS4) support large files.

Support for files >4GiB is currently not available on AROS, AmigaOS3, and AmigaOS2.

<h2>Notes about porting</h2>

This port is based directly on unrar source code version 5.9.2 (unrarsrc-5.9.2.tar.gz) from rarlab.com:
https://www.rarlab.com/rar/unrarsrc-5.9.2.tar.gz

For normalizing UTF, I used utf8proc. I did not need to port it to Amiga - it compiles without any change:
https://juliastrings.github.io/utf8proc/

For vfwprintf-like functions and multiple other wide characters related functions (wc*, mb*, bt*, wmem*) from libc, that are not implemented on Amiga at all, I ported the code I took from newlib 3.1.0:
http://sourceware.org/newlib/

ftp://sourceware.org/pub/newlib/newlib-3.1.0.tar.gz

For getpass() function missing in AROS I took the code from libnix 3.0 by Diego Casorran:
https://github.com/diegocr/libnix

As libiconv is missing in AmigaOS3/AmigaOS2, I use one from Aminet ported by Bruno Haible & Diego Casorran (LGPL):
http://aminet.net/package/dev/gg/libiconv-1.8

MorphOS version of unrar has been compiled using gcc 4.4.5 (part of SDK 3.14).

AmigaOS4 version of unrar has been compiled using gcc 4.2.4 (part of SDK 53.20).

AROS version of unrar has been compiled using gcc 4.6.4 (part of Icaros 2.2).

AmigaOS3/AmigaOS2 version of unrar has been compiled using gcc 3.3 (part of Cubic IDE).

All AmigaOS versions can also be created by cross-compiling using the same compile versions as above from AmiDevCpp package.

