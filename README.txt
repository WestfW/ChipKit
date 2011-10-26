Assorted hacks and bits for ChipKit PIC32-based Arduino-compatible
boards.  In general, these can be used one file or one directory
at a time, rather than requiring a download of the whole project.

FastDigital.h: macros for implementing digitalWriteFast(),
	       digitalReadFast(), and pinModeFast() that compile to a
	       couple of instructions (instead of a function call)
	       when the arguments are constants.
