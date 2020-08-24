#ifndef PRINT_HEX_H
#define PRINT_HEX_H

void fpe_print_hex(const fpe_t op);
void fp2e_print_hex(const fp2e_t op);
void fp12e_print_hex(const fp12e_t op);
void fp12e_to_bytearray(unsigned char *rop, const fp12e_t op);
void bytearray_to_fp12e(const fp12e_t op, unsigned char *rop);
#endif
