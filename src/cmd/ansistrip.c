#include <u.h>
#include <libc.h>

void
main(int argc, char *atgv[])
{
	char buf[8192], out[8192];
	long n, o;
	char inCSI = 0;
	while ((n=read(0, buf, (long)sizeof buf))>0) {
		o = 0;
		for (int i = 0 ; i < n ; ++i ) {
			char c = buf[i];
			if (inCSI == 0) {
				if (c == 0x1b) {
					i++; // This is buggy in the advent of ESC happening as the last byte of the buffer
					inCSI = 1;
				}
				else
					out[o++] = c;
			} else {
				if (c >= 0x40 && c <= 0x7f) {
					inCSI = 0;
				}
			}
		}
		if(write(1, out, o) != o)
			sysfatal("write error copying: %r");
	}
	if (n < 0)
		sysfatal("error reading <stdin>: %r");
	exits(0);
}