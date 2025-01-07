; sekelton if you want to add on to the tests

; for testing the CRT affect shader
.segment "HEADER"
  ; .byte "NES", $1A      ; iNES header identifier
  .byte $4E, $45, $53, $1A
  .byte 2               ; 2x 16KB PRG code
  .byte 1               ; 1x  8KB CHR data
  .byte $01, $00        ; mapper 0, vertical mirroring
.segment "VECTORS"
  ;; When an NMI happens (once per frame if enabled) the label nmi:
    .addr nmi
    ;; When the processor first turns on or is reset, it will jump to the label reset:
    .addr reset ; reset vector
.segment "STARTUP"
reset:
    sei
    cld

    ; reset sound
    ldx #%10000000
    stx $4017
    ldx #$00
    stx $4010

    ; init stack
    ldx #$FF
    TXS

    ; clear PPU registers
    LDX $00
    STX $2000
    STX $2001

    ;wait for vblank
    ; jsr wait_for_vblank
    ; vblank_loop:
    ;     bit $2002
    ;     bpl vblank_loop
    txa
    clear_mem:
        sta $0000, X
        sta $0100, X
        lda $FF
        sta $0200, X
        lda $00
        sta $0300, X
        sta $0400, X
        sta $0500, X
        sta $0600, X
        sta $0700, X
        inx 
        cpx #$00
        bne clear_mem
        ; jsr wait_for_vblank
        vblank_loop2:
            bit $2002
            bpl vblank_loop2
        
        lda #$02
        sta $4014
        nop

        ; store 3f00 in ppu
        lda #$3f
        sta $2006
        lda #$00
        sta $2006

        load_pallete:
        LDA palletes,X 
        sta $2007
        inx 
        cpx #$20
        bne load_pallete

        ldx #$00
        load_sprites:
            lda sprite_data, X
            sta $0200, X
            inx
            cpx #$10
            bne load_sprites
        cli
        lda #%10010000 ; vblank status
        sta $2000
        lda #%00011110 ; vblank status
        sta $2001

    loop:
        JMP loop
nmi:
    lda #$02
    sta $4014
    rti   
palletes:
	.byte $00, $0F, $00, $10, 	$00, $0A, $15, $01, 	$00, $29, $28, $27, 	$00, $34, $24, $14 	;background palettes
	.byte $20, $0F, $15, $30, 	$20, $0F, $11, $30, 	$20, $0F, $30, $27, 	$20, $3C, $2C, $1C 	;sprite palettes


sprite_data:
;Y, SPRITE NUM, attributes, X
;76543210
;||||||||
;||||||++- Palette (4 to 7) of sprite
;|||+++--- Unimplemented
;||+------ Priority (0: in front of background; 1: behind background)
;|+------- Flip sprite horizontally
;+-------- Flip sprite vertically
	.byte $24, $00, $00, $40
	; .byte $40, $01, $00, $48
	; .byte $48, $10, $00, $40
	; .byte $48, $11, $00, $48
.segment "CHARS" ; for graphics
; .incbin  "rom.chr"