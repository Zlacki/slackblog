slackblog
=========

CGI Blogging using C

Beta level software, licensed under CC0 public domain license.
Written by Zach Knight
C99 backend to blog software using CGI.

See it in action at: http://libslack.so/

This should work on any UNIX-like system, but YMMV.  Any bug reports are helpful.

Install with these gcc flags:
gcc -static -std=c99 -Wall -pedantic -D_GNU_SOURCE -O3 -o blog.bin src/blog.c

Maybe once I expand featureset a bit more I will make a Makefile.

Sample style included, derived from Tanami Browne's http://9ch.in/ website.

Any merge requests would be helpful.

 - Zach Knight <zach@libslack.so>
