/*
 * DigitalWriteFast for ChipKit
 * October 2011 by Bill Westfield (WestfW)
 *
 *
 * The Arduino environment provides functions digitalWrite(Pin, Val),
 * pinMode(Pin, Dir), and digitalRead(Pin) that are much slower than
 * they need to be if the arguments are constants.  Numerous discussions
 * and examples have taken place on how to implement faster functions,
 * by detecting the use of constants and producing more efficient inline
 * code (often, a single instruction on AVR.)
 *
 * This file does the same thing for the ChipKit board, which uses a PIC32
 * microcontroller (implementing the MIPS M4K architecture.)  Since this
 * environment also uses gcc, the code is very similar to several of the
 * examples created for the AVR Arduinos.  The major difference is that
 * this file uses keyboard macros (for EMACS) to derive the complex "if"
 * statement used to convert from "pin" to IOPort and bitmask operating
 * on the source code for pinToPort and pinToMask from the cpu32 "core"
 * pins_arduino_*.cxx files.  This should be less prone to error, though
 * it does produce rather long and nested macro definitions.
 *
 * Also, the PIC32 implements SET and CLR registers for each IO port that
 * prevent the need for read/modify/rewrite and consequent atomicity issues
 * that occur on the AVR.
 */

/*
 * EMACS keyboard macro for converting digital_pin_to_port_PGM table
 * in pins_arduino_pic32_uno.cxx into a macro definition.
 * Copy the table, edit it manually to remove blank lines, set
 * register "p" to 0 ( C-U 0 C-X r n p ) and then execute this macro
 * once for each pin.  Then add the #define at the top and enough
 * close parens at the end to make it work.  (If you don't understand
 * this, then you probably shouldn't try to modify these macros !)
 
Command: last-kbd-macro
Key: none

Macro:

2*SPC			;; self-insert-command
C-q (			;; 
C-q (			;;  "  ((P)=="
P			;; 
C-q )			;; 
==			;; 
C-x r g p		;; insert-register p
C-u 1 C-x r + p		;; increment-register p
M-f			;; forward-word past number we just added
C-q )			;; quoted-insert
C-q SPC			;;   "n) ? "
C-q ?			;; 
C-d			;; delete tab (hope its a tab!)
C-q SPC			;; quoted-insert
ESC f			;; forward-word over existing reg name
C-q SPC			;; " : ( \ "
C-q :			;; 
C-q SPC			;; 
C-q (			;; 
C-k			;; clear rest of line
C-q SPC			;; quoted-insert
C-q \			;; quoted-insert
C-n			;; next-line
C-a			;; move-beginning-of-line

*/


/* Macro to change lines containg "PB" to ones containin "LATBSET"
 * make a couple copies of the new macro, so that they can be modified
 * to implement the necessary functions (set, clr, tris)

C-s			;; isearch-forward
?			;; self-insert-command
SPC			;; self-insert-command
P			;; self-insert-command
NUL			;; set-mark-command
DEL			;; c-electric-backspace
LAT			;; self-insert-command * 3
C-f			;; forward-char
SET			;; self-insert-command * 3
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#define _dpin_to_set_macro(P) \
  ((P)==0) ? &LATFSET : ( \
  ((P)==1) ? &LATFSET : ( \
  ((P)==2) ? &LATDSET : ( \
  ((P)==3) ? &LATDSET : ( \
  ((P)==4) ? &LATFSET : ( \
  ((P)==5) ? &LATDSET : ( \
  ((P)==6) ? &LATDSET : ( \
  ((P)==7) ? &LATDSET : ( \
  ((P)==8) ? &LATDSET : ( \
  ((P)==9) ? &LATDSET : ( \
  ((P)==10) ? &LATDSET : ( \
  ((P)==11) ? &LATGSET : ( \
  ((P)==12) ? &LATGSET : ( \
  ((P)==13) ? &LATGSET : ( \
  ((P)==14) ? &LATBSET : ( \
  ((P)==15) ? &LATBSET : ( \
  ((P)==16) ? &LATBSET : ( \
  ((P)==17) ? &LATBSET : ( \
  ((P)==18) ? &LATBSET : ( \
  ((P)==19) ? &LATBSET : ( \
  ((P)==20) ? &LATBSET : ( \
  ((P)==21) ? &LATBSET : ( \
  ((P)==22) ? &LATBSET : ( \
  ((P)==23) ? &LATBSET : ( \
  ((P)==24) ? &LATBSET : ( \
  ((P)==25) ? &LATBSET : ( \
  ((P)==26) ? &LATESET : ( \
  ((P)==27) ? &LATESET : ( \
  ((P)==28) ? &LATESET : ( \
  ((P)==29) ? &LATESET : ( \
  ((P)==30) ? &LATESET : ( \
  ((P)==31) ? &LATESET : ( \
  ((P)==32) ? &LATESET : ( \
  ((P)==33) ? &LATESET : ( \
  ((P)==34) ? &LATDSET : ( \
  ((P)==35) ? &LATDSET : ( \
  ((P)==36) ? &LATDSET : ( \
  ((P)==37) ? &LATDSET : ( \
  ((P)==38) ? &LATFSET : ( \
  ((P)==39) ? &LATFSET : ( \
  ((P)==40) ? &LATFSET : ( \
  ((P)==41) ? &LATBSET : ( \
  ((P)==42) ? &LATBSET : ( \
  ((P)==43) ? &LATFSET : 0 )))))))))))))))))))))))))))))))))))))))))))

/* Macro to change lines containg "PB" to ones containin "LATBCLR"

C-s			;; isearch-forward
?			;; self-insert-command
SPC			;; self-insert-command
P			;; self-insert-command
NUL			;; set-mark-command
DEL			;; c-electric-backspace
LAT			;; self-insert-command * 3
C-f			;; forward-char
CLR			;; self-insert-command * 3
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#define _dpin_to_clr_macro(P)			\
  ((P)==0) ? &LATFCLR : ( \
  ((P)==1) ? &LATFCLR : ( \
  ((P)==2) ? &LATDCLR : ( \
  ((P)==3) ? &LATDCLR : ( \
  ((P)==4) ? &LATFCLR : ( \
  ((P)==5) ? &LATDCLR : ( \
  ((P)==6) ? &LATDCLR : ( \
  ((P)==7) ? &LATDCLR : ( \
  ((P)==8) ? &LATDCLR : ( \
  ((P)==9) ? &LATDCLR : ( \
  ((P)==10) ? &LATDCLR : ( \
  ((P)==11) ? &LATGCLR : ( \
  ((P)==12) ? &LATGCLR : ( \
  ((P)==13) ? &LATGCLR : ( \
  ((P)==14) ? &LATBCLR : ( \
  ((P)==15) ? &LATBCLR : ( \
  ((P)==16) ? &LATBCLR : ( \
  ((P)==17) ? &LATBCLR : ( \
  ((P)==18) ? &LATBCLR : ( \
  ((P)==19) ? &LATBCLR : ( \
  ((P)==20) ? &LATBCLR : ( \
  ((P)==21) ? &LATBCLR : ( \
  ((P)==22) ? &LATBCLR : ( \
  ((P)==23) ? &LATBCLR : ( \
  ((P)==24) ? &LATBCLR : ( \
  ((P)==25) ? &LATBCLR : ( \
  ((P)==26) ? &LATECLR : ( \
  ((P)==27) ? &LATECLR : ( \
  ((P)==28) ? &LATECLR : ( \
  ((P)==29) ? &LATECLR : ( \
  ((P)==30) ? &LATECLR : ( \
  ((P)==31) ? &LATECLR : ( \
  ((P)==32) ? &LATECLR : ( \
  ((P)==33) ? &LATECLR : ( \
  ((P)==34) ? &LATDCLR : ( \
  ((P)==35) ? &LATDCLR : ( \
  ((P)==36) ? &LATDCLR : ( \
  ((P)==37) ? &LATDCLR : ( \
  ((P)==38) ? &LATFCLR : ( \
  ((P)==39) ? &LATFCLR : ( \
  ((P)==40) ? &LATFCLR : ( \
  ((P)==41) ? &LATBCLR : ( \
  ((P)==42) ? &LATBCLR : ( \
  ((P)==43) ? &LATFCLR : 0 )))))))))))))))))))))))))))))))))))))))))))


//************************************************************************
/*  Slightly different macro for the bitmasks in digital_pin_to_bit_mask_PGM

2*SPC			;; self-insert-command
C-q (			;; quoted-insert
C-q (			;; quoted-insert
P			;; self-insert-command
C-q )			;; quoted-insert
==			;; self-insert-command * 2
C-x r g p		;; insert-register p
C-u 1 C-x r + p		;; increment-register p
M-f			;; forward-word
C-q )			;; quoted-insert
C-q SPC			;; quoted-insert
C-q ?			;; quoted-insert
M-\			;; delete whitespace
C-q SPC			;; quoted-insert
C-s ) C-@		;; forward-search
C-q SPC			;; quoted-insert
C-q :			;; quoted-insert
C-q SPC			;; quoted-insert
C-q (			;; quoted-insert
C-k			;; kill-line
C-q SPC			;; quoted-insert
C-q \			;; quoted-insert
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#define _dpin_to_bitmask_macro(P) \
  ((P)==0) ? _BV( 2 ) : ( \
  ((P)==1) ? _BV( 3 ) : ( \
  ((P)==2) ? _BV( 8 ) : ( \
  ((P)==3) ? _BV( 0 ) : ( \
  ((P)==4) ? _BV( 1 ) : ( \
  ((P)==5) ? _BV( 1 ) : ( \
  ((P)==6) ? _BV( 2 ) : ( \
  ((P)==7) ? _BV( 9 ) : ( \
  ((P)==8) ? _BV( 10 ) : ( \
  ((P)==9) ? _BV( 3 ) : ( \
  ((P)==10) ? _BV( 4 ) : ( \
  ((P)==11) ? _BV( 8 ) : ( \
  ((P)==12) ? _BV( 7 ) : ( \
  ((P)==13) ? _BV( 6 ) : ( \
  ((P)==14) ? _BV( 2 ) : ( \
  ((P)==15) ? _BV( 4 ) : ( \
  ((P)==16) ? _BV( 8 ) : ( \
  ((P)==17) ? _BV( 10 ) : ( \
  ((P)==18) ? _BV( 12 ) : ( \
  ((P)==19) ? _BV( 14 ) : ( \
  ((P)==20) ? _BV( 3 ) : ( \
  ((P)==21) ? _BV( 5 ) : ( \
  ((P)==22) ? _BV( 9 ) : ( \
  ((P)==23) ? _BV( 11 ) : ( \
  ((P)==24) ? _BV( 13 ) : ( \
  ((P)==25) ? _BV( 15 ) : ( \
  ((P)==26) ? _BV( 0 ) : ( \
  ((P)==27) ? _BV( 1 ) : ( \
  ((P)==28) ? _BV( 2 ) : ( \
  ((P)==29) ? _BV( 3 ) : ( \
  ((P)==30) ? _BV( 4 ) : ( \
  ((P)==31) ? _BV( 5 ) : ( \
  ((P)==32) ? _BV( 6 ) : ( \
  ((P)==33) ? _BV( 7 ) : ( \
  ((P)==34) ? _BV( 5 ) : ( \
  ((P)==35) ? _BV( 11 ) : ( \
  ((P)==36) ? _BV( 6 ) : ( \
  ((P)==37) ? _BV( 7 ) : ( \
  ((P)==38) ? _BV( 6 ) : ( \
  ((P)==39) ? _BV( 4 ) : ( \
  ((P)==40) ? _BV( 5 ) : ( \
  ((P)==41) ? _BV( 1 ) : ( \
  ((P)==42) ? _BV( 0 ) : ( \
  ((P)==43) ? _BV( 0 ) : 0)))))))))))))))))))))))))))))))))))))))))))

/*
 * Input Macro.  probably should decide about PORTx vs LATx
 * IIRC, these were created using simple query/replace over a copy
 * of the previous tables, rather than a separate keyboard macro.
 */
#define _dpin_to_inp_macro(P)			\
  ((P)==0) ? &PORTF : ( \
  ((P)==1) ? &PORTF : ( \
  ((P)==2) ? &PORTD : ( \
  ((P)==3) ? &PORTD : ( \
  ((P)==4) ? &PORTF : ( \
  ((P)==5) ? &PORTD : ( \
  ((P)==6) ? &PORTD : ( \
  ((P)==7) ? &PORTD : ( \
  ((P)==8) ? &PORTD : ( \
  ((P)==9) ? &PORTD : ( \
  ((P)==10) ? &PORTD : ( \
  ((P)==11) ? &PORTG : ( \
  ((P)==12) ? &PORTG : ( \
  ((P)==13) ? &PORTG : ( \
  ((P)==14) ? &PORTB : ( \
  ((P)==15) ? &PORTB : ( \
  ((P)==16) ? &PORTB : ( \
  ((P)==17) ? &PORTB : ( \
  ((P)==18) ? &PORTB : ( \
  ((P)==19) ? &PORTB : ( \
  ((P)==20) ? &PORTB : ( \
  ((P)==21) ? &PORTB : ( \
  ((P)==22) ? &PORTB : ( \
  ((P)==23) ? &PORTB : ( \
  ((P)==24) ? &PORTB : ( \
  ((P)==25) ? &PORTB : ( \
  ((P)==26) ? &PORTE : ( \
  ((P)==27) ? &PORTE : ( \
  ((P)==28) ? &PORTE : ( \
  ((P)==29) ? &PORTE : ( \
  ((P)==30) ? &PORTE : ( \
  ((P)==31) ? &PORTE : ( \
  ((P)==32) ? &PORTE : ( \
  ((P)==33) ? &PORTE : ( \
  ((P)==34) ? &PORTD : ( \
  ((P)==35) ? &PORTD : ( \
  ((P)==36) ? &PORTD : ( \
  ((P)==37) ? &PORTD : ( \
  ((P)==38) ? &PORTF : ( \
  ((P)==39) ? &PORTF : ( \
  ((P)==40) ? &PORTF : ( \
  ((P)==41) ? &PORTB : ( \
  ((P)==42) ? &PORTB : ( \
  ((P)==43) ? &PORTF : 0 )))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_trisclr_macro(P)			\
  ((P)==0) ? &TRISFCLR : ( \
  ((P)==1) ? &TRISFCLR : ( \
  ((P)==2) ? &TRISDCLR : ( \
  ((P)==3) ? &TRISDCLR : ( \
  ((P)==4) ? &TRISFCLR : ( \
  ((P)==5) ? &TRISDCLR : ( \
  ((P)==6) ? &TRISDCLR : ( \
  ((P)==7) ? &TRISDCLR : ( \
  ((P)==8) ? &TRISDCLR : ( \
  ((P)==9) ? &TRISDCLR : ( \
  ((P)==10) ? &TRISDCLR : ( \
  ((P)==11) ? &TRISGCLR : ( \
  ((P)==12) ? &TRISGCLR : ( \
  ((P)==13) ? &TRISGCLR : ( \
  ((P)==14) ? &TRISBCLR : ( \
  ((P)==15) ? &TRISBCLR : ( \
  ((P)==16) ? &TRISBCLR : ( \
  ((P)==17) ? &TRISBCLR : ( \
  ((P)==18) ? &TRISBCLR : ( \
  ((P)==19) ? &TRISBCLR : ( \
  ((P)==20) ? &TRISBCLR : ( \
  ((P)==21) ? &TRISBCLR : ( \
  ((P)==22) ? &TRISBCLR : ( \
  ((P)==23) ? &TRISBCLR : ( \
  ((P)==24) ? &TRISBCLR : ( \
  ((P)==25) ? &TRISBCLR : ( \
  ((P)==26) ? &TRISECLR : ( \
  ((P)==27) ? &TRISECLR : ( \
  ((P)==28) ? &TRISECLR : ( \
  ((P)==29) ? &TRISECLR : ( \
  ((P)==30) ? &TRISECLR : ( \
  ((P)==31) ? &TRISECLR : ( \
  ((P)==32) ? &TRISECLR : ( \
  ((P)==33) ? &TRISECLR : ( \
  ((P)==34) ? &TRISDCLR : ( \
  ((P)==35) ? &TRISDCLR : ( \
  ((P)==36) ? &TRISDCLR : ( \
  ((P)==37) ? &TRISDCLR : ( \
  ((P)==38) ? &TRISFCLR : ( \
  ((P)==39) ? &TRISFCLR : ( \
  ((P)==40) ? &TRISFCLR : ( \
  ((P)==41) ? &TRISBCLR : ( \
  ((P)==42) ? &TRISBCLR : ( \
  ((P)==43) ? &TRISFCLR : 0 )))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_trisset_macro(P)			\
  ((P)==0) ? &TRISFSET : ( \
  ((P)==1) ? &TRISFSET : ( \
  ((P)==2) ? &TRISDSET : ( \
  ((P)==3) ? &TRISDSET : ( \
  ((P)==4) ? &TRISFSET : ( \
  ((P)==5) ? &TRISDSET : ( \
  ((P)==6) ? &TRISDSET : ( \
  ((P)==7) ? &TRISDSET : ( \
  ((P)==8) ? &TRISDSET : ( \
  ((P)==9) ? &TRISDSET : ( \
  ((P)==10) ? &TRISDSET : ( \
  ((P)==11) ? &TRISGSET : ( \
  ((P)==12) ? &TRISGSET : ( \
  ((P)==13) ? &TRISGSET : ( \
  ((P)==14) ? &TRISBSET : ( \
  ((P)==15) ? &TRISBSET : ( \
  ((P)==16) ? &TRISBSET : ( \
  ((P)==17) ? &TRISBSET : ( \
  ((P)==18) ? &TRISBSET : ( \
  ((P)==19) ? &TRISBSET : ( \
  ((P)==20) ? &TRISBSET : ( \
  ((P)==21) ? &TRISBSET : ( \
  ((P)==22) ? &TRISBSET : ( \
  ((P)==23) ? &TRISBSET : ( \
  ((P)==24) ? &TRISBSET : ( \
  ((P)==25) ? &TRISBSET : ( \
  ((P)==26) ? &TRISESET : ( \
  ((P)==27) ? &TRISESET : ( \
  ((P)==28) ? &TRISESET : ( \
  ((P)==29) ? &TRISESET : ( \
  ((P)==30) ? &TRISESET : ( \
  ((P)==31) ? &TRISESET : ( \
  ((P)==32) ? &TRISESET : ( \
  ((P)==33) ? &TRISESET : ( \
  ((P)==34) ? &TRISDSET : ( \
  ((P)==35) ? &TRISDSET : ( \
  ((P)==36) ? &TRISDSET : ( \
  ((P)==37) ? &TRISDSET : ( \
  ((P)==38) ? &TRISFSET : ( \
  ((P)==39) ? &TRISFSET : ( \
  ((P)==40) ? &TRISFSET : ( \
  ((P)==41) ? &TRISBSET : ( \
  ((P)==42) ? &TRISBSET : ( \
  ((P)==43) ? &TRISFSET : 0 )))))))))))))))))))))))))))))))))))))))))))


#ifndef digitalWriteFast
#define digitalWriteFast(P, V) \
do {								\
    if (__builtin_constant_p(P) && __builtin_constant_p(V)) {		\
	if (V) {							\
	    *(_dpin_to_set_macro(P)) = _dpin_to_bitmask_macro(P);	\
	} else {							\
	    *(_dpin_to_clr_macro(P))  = _dpin_to_bitmask_macro(P);	\
	}								\
    } else  digitalWrite((P), (V));					\
}while (0)
#endif  //#ifndef digitalWriteFast


#define pinModeFast(P, D) \
do {								\
    if (__builtin_constant_p(P) && __builtin_constant_p(D)) {		\
	if (D == OUTPUT) {						\
	    *(_dpin_to_trisclr_macro(P)) = _dpin_to_bitmask_macro(P);	\
	} else {							\
	    *(_dpin_to_trisset_macro(P))  = _dpin_to_bitmask_macro(P);	\
	}								\
    } else pinMode((P), (D));					\
}while (0)

#define _BIT_READ(value, bit) (((value) >> (bit)) & 0x01)

#define digitalReadFast(P)	       \
    (__builtin_constant_p(P) ) ? (					\
	(_BIT_READ(*(_dpin_to_inp_macro(P)), _dpin_to_bitmask_macro(P))) ) : \
    digitalRead((P))
