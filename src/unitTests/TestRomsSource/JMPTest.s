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
    .addr reset
    .addr irq

; "nes" linker config requires a STARTUP section, even if it's empty
.segment "STARTUP"
; chars
.segment "CHARS"
; Main code segment for the program
.segment "CODE"


main: 
  LDA #1
JSR Add
LDX #10
BRK

Add:
ADC #1
BNE L2
RTS
L2:
LDY #1
RTS
nmi:
    rti
reset:
      jmp main
    irq:
        jmp main