#include "uefi/uefi.h"

efi_system_table_t *ST;

void PrintNum(uint64_t num)
{
    wchar_t numstr[32];
    wchar_t ch[2];
    uint32_t strlen = 0;
    if (num == 0)
    {
        ST->ConOut->OutputString(ST->ConOut, L"0");
        return;
    }

    while (num != 0)
    {
        numstr[strlen++] = L'0' + (num % 10);
        num /= 10;
    }

    ch[1] = 0;
    do
    {
        ch[0] = numstr[--strlen];
        ST->ConOut->OutputString(ST->ConOut, ch);
    } while (strlen > 0);
}

efi_status_t EfiMain(efi_handle_t ImageHandle, efi_system_table_t *SystemTable)
{
    (void)ImageHandle;
    ST = SystemTable;
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI\r\n");

    uint8_t memmap[8192];
    uint32_t memver = 0;
    uintn_t len = sizeof(memmap), key = 0, size = 0;
    efi_status_t sta = SystemTable->BootServices->GetMemoryMap(&len, (efi_memory_descriptor_t *)memmap, &key, &size, &memver);
    if (sta != EFI_SUCCESS)
        ST->ConOut->OutputString(ST->ConOut, L"GetMemoryMap failed\r\n");
    for (uintn_t i = 0; i < len / size; ++i)
    {
        efi_memory_descriptor_t *entry = (efi_memory_descriptor_t *)(memmap + i * size);
        if (entry->Type != 7)
            continue;
        ST->ConOut->OutputString(ST->ConOut, L"Physical Start at ");
        PrintNum(entry->PhysicalStart);
        ST->ConOut->OutputString(ST->ConOut, L", Page Number ");
        PrintNum(entry->NumberOfPages);
        ST->ConOut->OutputString(ST->ConOut, L"\r\n");
    }

    while (1);
    return EFI_SUCCESS;
}
