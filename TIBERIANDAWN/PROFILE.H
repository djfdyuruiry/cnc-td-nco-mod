#pragma once

int WWGetPrivateProfileInt(char const* section, char const* entry, int def, char* profile);
int WWGetPrivateProfileInt(char const* section, char const* entry, char* profile, bool* valueFound);
int WWGetPrivateProfileInt(char const* section, char const* entry, int def, char* profile, bool* valueFound);
bool WWWritePrivateProfileInt(char const* section, char const* entry, int value, char* profile);
bool WWWritePrivateProfileString(char const* section, char const* entry, char const* string, char* profile);
char* WWGetPrivateProfileString(char const* section, char const* entry, char const* def, char* retbuffer, int retlen, char* profile);
char* WWGetPrivateProfileString(char const* section, char const* entry, char* retbuffer, int retlen, char* profile, bool* valueFound);
char* WWGetPrivateProfileString(char const* section, char const* entry, char const* def, char* retbuffer, int retlen, char* profile, bool* valueFound);
unsigned WWGetPrivateProfileHex(char const* section, char const* entry, char* profile);
