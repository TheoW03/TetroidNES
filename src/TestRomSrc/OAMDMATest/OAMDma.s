; sekelton if you want to add on to the tests
.define INPUT_REG1 $20
.define INPUT_REG2 $21

.define BUTTON_UP #$08 
.define BUTTON_DOWN #$04


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
    ; jsr init_input
    ; jsr read_controller
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
    vblank_loop:
        bit $2002
        bpl vblank_loop
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
            cpx #$04
            bne load_sprites
            ; lda %00000011
            ; sta $0202
        cli
        lda #%10000000 ; vblank status
        sta $2000
        lda #%00010000 ; vblank status
        sta $2001

    loop:
        
        JMP loop
nmi:

    jsr init_input
    jsr read_controller
    lda #$00
    lda INPUT_REG1 
    and BUTTON_UP
    bne color
    beq set_color
    ; broken
    color:
        lda %00000000
        sta $0202
        jmp end
    set_color:
        lda %00000011
        sta $0202
    end:
    lda #$02
    sta $4014
    rti   
palletes:
	.byte $00, $0F, $00, $10, 	$00, $0A, $15, $01, 	$00, $29, $28, $27, 	$00, $34, $24, $14 	;background palettes
	.byte $31, $0F, $15, $30, 	$00, $0F, $11, $30, 	$00, $0F, $30, $27, 	$00, $28, $2C, $1C 	;sprite palettes

; https://www.youtube.com/watch?v=nAStgQzPrAQ&t=1s&ab_channel=NesHacker
init_input:
    ; inits
    lda #1
    sta INPUT_REG1

    ; resets this is for input 1
    sta $4016
    lda #0
    sta $4016

    
    ; this resets the 2nd controller register
    lda #1
    sta INPUT_REG2

    ; resets this is for input 2
    sta $4017
    lda #0
    sta $4017
    rts
read_controller:

; stores the contents of the input register to $20 to be used. this is for 
; the left paddle
    read_loop1:
        lda $4016
        lsr a
        rol INPUT_REG1

        bcc read_loop1
    lda #0 

; stores the contents of the input register to $21 to be used. this is for 
; the right paddle
    read_loop2: 
        lda $4017
        lsr a
        rol INPUT_REG2
        bcc read_loop2
    rts

sprite_data:
;Y, SPRITE NUM, attributes, X
;76543210
;||||||||
;||||||++- Palette (4 to 7) of sprite
;|||+++--- Unimplemented
;||+------ Priority (0: in front of background; 1: behind background)
;|+------- Flip sprite horizontally
;+-------- Flip sprite vertically
	.byte $50, $00, %00000011, $70 
.segment "CHARS" ; for graphics
.incbin "OAMDma.chr"