#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static uint8_t *buffer1;
static uint8_t *buffer2;
static uint8_t *dest_buffer;
static uint8_t *src_buffer;
static uint32_t uncomp_len;
static uint32_t pre_copy_len;
static uint32_t UINT_70000018;
static uint32_t UINT_7000002c;
static int32_t INT_70000004;
static uint32_t UINT_70000008;
static uint32_t UINT_70000010;
static uint32_t UINT_7000000c;
static uint8_t temp_buffer[0x4000];

uint32_t decompress(uint8_t *comp_buf, uint8_t *uncomp_buf) {
    uint8_t bVar1;
    uint8_t bVar2;
    uint8_t bVar3;
    uint8_t bVar4;
    uint8_t bVar5;
    uint32_t uVar6;
    uint16_t uVar7;
    int iVar8;
    uint8_t *pbVar9;
    int iVar10;
    uint8_t *pbVar11;
    uint8_t *pbVar12;
    uint8_t *pbVar13;
    int iVar14;
    
    uncomp_len = *(uint32_t *)comp_buf;
    UINT_70000018 = *(uint32_t *)(comp_buf + 4);
    pre_copy_len = *(uint32_t *)(comp_buf + 8);
    dest_buffer = temp_buffer + pre_copy_len;
    src_buffer = comp_buf + 0x10 + pre_copy_len;
    UINT_7000002c = (uint32_t)comp_buf[0xc];
    iVar14 = 0;
    buffer1 = temp_buffer;
    buffer2 = temp_buffer;
    memcpy(temp_buffer, &comp_buf[16], pre_copy_len);
    iVar8 = uncomp_len - UINT_70000018;
    pbVar13 = uncomp_buf + iVar8;
    for (;;) {
        bVar1 = *src_buffer;
        if (iVar14 == 0) {
            pbVar11 = buffer2 + 1;
            bVar2 = *buffer2;
            pbVar9 = src_buffer + 1;
            iVar14 = 0x1e;
            buffer2 = pbVar11;
            if (dest_buffer == pbVar11) {
                buffer2 = buffer1;
            }
            bVar3 = *buffer2;
            pbVar11 = src_buffer + 2;
            buffer2 += 1;
            if (dest_buffer == buffer2) {
                buffer2 = buffer1;
            }
            bVar4 = *buffer2;
            pbVar12 = src_buffer + 3;
            buffer2 += 1;
            if (dest_buffer == buffer2) {
                buffer2 = buffer1;
            }
            bVar5 = *buffer2;
            src_buffer = src_buffer + 4;
            buffer2 += 1;
            if (dest_buffer == buffer2) {
                buffer2 = buffer1;
            }
            INT_70000004 = *pbVar12 ^ bVar5;
            INT_70000004 |= (*pbVar11 ^ bVar4) << 8;
            INT_70000004 |= (*pbVar9 ^ bVar3) << 16;
            INT_70000004 |= (bVar1 ^ bVar2) << 24;
            UINT_70000008 = (*pbVar12 ^ bVar5) & 3;
            UINT_70000010 = 0x3fff >> UINT_70000008;
            UINT_7000000c = 0xe - UINT_70000008;
            bVar1 = *src_buffer;
        }
        pbVar11 = buffer2 + 1;
        bVar2 = *buffer2;
        pbVar9 = src_buffer + 1;
        buffer2 = pbVar11;
        if (dest_buffer == pbVar11) {
            buffer2 = buffer1;
        }
        if (INT_70000004 < 0) {
            bVar3 = *buffer2;
            src_buffer = src_buffer + 2;
            buffer2 = buffer2 + 1;
            if (dest_buffer == buffer2) {
                buffer2 = buffer1;
            }
            uVar7 = (*pbVar9 ^ bVar3) | ((bVar1 ^ bVar2) << 8);
            uVar6 = UINT_7000000c & 0x1f;
            pbVar9 = pbVar13 - ((uVar7 & UINT_70000010) + 1);
            *pbVar13 = *pbVar9;
            for (iVar10 = (uVar7 >> uVar6) + 2; pbVar13 += 1, iVar10 != 0; --iVar10) {
                pbVar9 += 1;
                *pbVar13 = *pbVar9;
            }
        } else {
            src_buffer = pbVar9;
            *pbVar13 = bVar1 ^ bVar2;
            pbVar13 += 1;
        }
        iVar14 -= 1;
        if (UINT_70000018 <= (uint32_t)((uintptr_t)pbVar13 - (uintptr_t)(uncomp_buf + iVar8))) break;
        INT_70000004 = INT_70000004 << 1;
    }
    pbVar13 = uncomp_buf + uncomp_len;
    if ((UINT_70000018 < uncomp_len) && (pbVar9 = pbVar13 - UINT_70000018, pbVar9 < pbVar13)) {
        bVar1 = *pbVar9;
        for (;;) {
            if (bVar1 == UINT_7000002c) {
                bVar2 = pbVar9[1];
                if ((uint32_t)bVar2 == (uint32_t)bVar1) {
                    *uncomp_buf = bVar2;
                    pbVar9 = pbVar9 + 2;
                    uncomp_buf += 1;
                } else {
                    pbVar11 = uncomp_buf + (pbVar9[2] | (bVar2 << 8));
                    pbVar12 = pbVar9 + 3;
                    if (uncomp_buf < pbVar11) {
                        pbVar9 = pbVar9 + 4;
                        do {
                            *uncomp_buf = *pbVar12;
                            uncomp_buf += 1;
                        } while (uncomp_buf < pbVar11);
                    } else {
                        pbVar9 = pbVar9 + 4;
                    }
                }
            } else {
                *uncomp_buf = bVar1;
                pbVar9 = pbVar9 + 1;
                uncomp_buf += 1;
            }
            if (pbVar13 <= pbVar9) break;
            bVar1 = *pbVar9;
        }
    }
    return uncomp_len;
}

int main(int argc, char **argv) {
    if (argc > 3) {
        uint8_t *data, *uncomp_data;
        long len, out_len;
        long off = atol(argv[2]);
        FILE *fp_in, *fp_out;
        fp_in = fopen(argv[1], "rb");
        if (!fp_in) {
            puts("Failed to open input file.");
            return 1;
        }
        fseek(fp_in, 0, SEEK_END);
        len = ftell(fp_in);
        fseek(fp_in, 0, SEEK_SET);
        data = (uint8_t *)malloc(len);
        if (!data) {
            puts("Failed to allocate file buffer.");
            fclose(fp_in);
            return 1;
        }
        fread(data, 1, len, fp_in);
        fclose(fp_in);
        uncomp_data = calloc(1, *(uint32_t *)(&data[off]));
        if (!uncomp_data) {
            puts("Failed to allocate destination buffer.");
            free(data);
            return 1;
        }
        out_len = decompress(&data[off], uncomp_data);
        fp_out = fopen(argv[3], "wb");
        if (!fp_in) {
            puts("Failed to open output file.");
            free(uncomp_data);
            free(data);
            return 1;
        }
        if (fwrite(uncomp_data, 1, out_len, fp_out) != out_len) {
            puts("Failed to write output file.");
            fclose(fp_out);
            free(uncomp_data);
            free(data);
            return 1;
        }
        fclose(fp_out);
        free(uncomp_data);
        free(data);
        puts("Successfully decompressed.");
        return 0;
    }
    puts("Usage:");
    printf("%s <in_path> <offset> <out_path>\n", argv[0]);
    return 1;
}
