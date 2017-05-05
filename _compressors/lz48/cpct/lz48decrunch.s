;
; LZ48 decrunch V006 from Roudou
;
; hl  compressed data adress
; de  output adress of data
;
; Definition:
;  void LZ48_decrunch(const u8 *source, u8* dest) __z88dk_callee;

_LZ48_decrunch::
ldi
ld b,#0

nextsequence:
ld a,(hl)
inc hl
ld (ix),a
and #240
jr z,lzunpack ; no litteral bytes
rrca
rrca
rrca
rrca

ld c,a
cp #15 ; more bytes for length?
jr nz,copyliteral

getadditionallength:
ld a,(hl)
inc hl
inc a
jr nz,lengthnext
inc b
dec bc
jr getadditionallength

lengthnext:
dec a
add a,c
ld c,a
ld a,b
adc a,#0
ld b,a ; bc=length

copyliteral:
ldir

lzunpack:
ld a,(ix)
and #15
add #3
ld c,a
cp #18 ; more bytes for length?
jr nz,readoffset

getadditionallengthbis:
ld a,(hl)
inc hl
inc a
jr nz,lengthnextbis
inc b
dec bc
jr getadditionallengthbis

lengthnextbis:
dec a
add a,c
ld c,a
ld a,b
adc a,#0
ld b,a ; bc=length

readoffset:
; read encoded offset
ld a,(hl)
inc a
ret z ; LZ48 end with zero offset
inc hl
push hl
ld l,a
ld a,e
sub l
ld l,a
ld a,d
sbc a,#0
ld h,a
; source=dest-copyoffset

copykey:
ldir

pop hl
jr nextsequence




