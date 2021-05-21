
ata_lba_read:
               pushfd
               and eax, 0x0FFFFFFF
               push eax
               push ebx
               push ecx
               push edx
               push edi

               mov ebx, eax         ; Save LBA in RBX

               mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
               shr eax, 24          ; Get bit 24 - 27 in al
               or al, 11100000b     ; Set bit 6 in al for LBA mode
               out dx, al

               mov edx, 0x01F2      ; Port to send number of sectors
               mov al, cl           ; Get number of sectors from CL
               out dx, al

               mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
               mov eax, ebx         ; Get LBA from EBX
               out dx, al

               mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 8           ; Get bit 8 - 15 in AL
               out dx, al


               mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 16          ; Get bit 16 - 23 in AL
               out dx, al

               mov edx, 0x1F7       ; Command port
               mov al, 0x20         ; Read with retry.
               out dx, al

.still_going:  in al, dx
               test al, 8           ; the sector buffer requires servicing.
               jz .still_going      ; until the sector buffer is ready.

               mov eax, 256         ; to read 256 words = 1 sector
               xor bx, bx
               mov bl, cl           ; read CL sectors
               mul bx
               mov ecx, eax         ; RCX is counter for INSW
               mov edx, 0x1F0       ; Data port, in and out
               rep insw             ; in to [RDI]

               pop edi
               pop edx
               pop ecx
               pop ebx
               pop eax
               popfd
               ret


