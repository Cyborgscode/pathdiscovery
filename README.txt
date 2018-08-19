Description:

This little tool will tell you, if any links are in your path, and it will show you any permission or owner on the way down to your file.

This helps a lot in cases, where you have software running, that denies link usage in pathes. i.e. proftp, apache aso.
It also helps in the more common cause of wrong permissions. You do not need to check any segment by hand ;)

Disclaimer:

Das Kompilieren und der Betrieb erfolgt auf eigene Gefahr.

I'm not responsible for any damage the usage and/or compile of this programm will cause.

Lizenz:

Anyone can modify and distribute it freely, if the documentation/credits will name me.
have fun.

Distributions: 

Your welcome to include it into your repositories. There is no complex compile process involved ;)

Compile: 

gcc -o pathdiscover pathdiscover.c

A working GCC and Linuxenvironment will help, but any Unix should do. 

Usage:

pathdiscover [-a] [-d] [-h] [-n] [-V] <filename>

Options: 

-V  gives out the actual version 

-n --names              disables uid -> username translation 
-N --numbers            enables raw byte output 
-a --alternative        each segment if shown with full path
-d --full-time		add atime, ctime and mtime to the output of each segemnt of your path
-NC 			Does not try to format output in columes

Example:

# pathdiscover /home/wordpress/jetpack/

'/home/wordpress/jetpack/' translates to '/opt/root/home/wordpress/jetpack'

  4096 Bytes  wordpress/wordpress drwxr-xr-x : jetpack   ( directory )
  4096 Bytes  wordpress/services  drwxr-x--- : wordpress ( directory )
  4096 Bytes  root/root           drwxr-xr-x : home      ( directory )
  4096 Bytes  root/root           drwxr-xr-x : root      ( directory )
  4096 Bytes  root/root           drwxr-xr-x : opt       ( directory )

# pathdiscover -d /home/wordpress/jetpack/

'/home/wordpress/jetpack/' translates to '/opt/root/home/wordpress/jetpack'

  4096 Bytes  (2013-10-29 15:24:51 | 2013-09-19 17:54:54 | 2017-12-06 06:30:20) wordpress/wordpress drwxr-xr-x : jetpack   ( directory )
  4096 Bytes  (2017-11-20 16:38:07 | 2017-11-20 16:38:07 | 2017-12-06 06:30:08) wordpress/services  drwxr-x--- : wordpress ( directory )
  4096 Bytes  (2017-11-09 10:34:50 | 2017-11-09 10:34:50 | 2017-12-06 03:58:54) root/root           drwxr-xr-x : home      ( directory )
  4096 Bytes  (2017-09-01 22:09:15 | 2015-02-10 15:27:53 | 2017-12-05 22:45:02) root/root           drwxr-xr-x : root      ( directory )
  4096 Bytes  (2017-09-01 22:16:29 | 2017-09-01 22:16:29 | 2017-12-06 03:58:39) root/root           drwxr-xr-x : opt       ( directory )

