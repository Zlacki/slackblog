slackblog
=========

CGI Blogging using C

Alpha, maybe beta level software, licensed under WTFPLv2(public domain worldwide).
Written by Sasha Volkov
C99 backend to blog software using CGI.

See it in action at: http://libslack.so/

This should work on any UNIX-like system, but YMMV.  Any bug reports are helpful.

Install with these gcc flags:
gcc -static -std=c99 -Wall -pedantic -D_GNU_SOURCE -O3 -o index.bin src/index.c

Maybe once I expand featureset a bit more I will make a Makefile.

Sample style included, derived from Tanami Browne's http://9ch.in/ website.

Any merge requests would be helpful.

