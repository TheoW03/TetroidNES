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
  .addr main ; reset vector
  .addr irq
  ;; External interrupt IRQ (unused)

; "nes" linker config requires a STARTUP section, even if it's empty
.segment "STARTUP"

; Main code segment for the program
.segment "CODE"

main:
    ; put code in here

irq:
    jmp main ; goes here if brk instrcution
nmi:
    rti ;goes here if NMI interrupt
    
.segment "CHARS" ; for graphics