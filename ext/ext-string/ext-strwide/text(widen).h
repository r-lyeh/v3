#ifdef _WIN32

// utf8 (used everywhere) to wstring (used by win32)
wchar_t* strwide_( const char* input ) {
    if( !input ) {
        return NULL;
    }
    int inlen = strlen(input);
    int outlen = MultiByteToWideChar(CP_UTF8, 0, input, inlen, 0, 0);
    wchar_t *output = (wchar_t*)va("%*.s", (int)((outlen+1) * sizeof(wchar_t)), "");
    if( output ) {
        MultiByteToWideChar(CP_UTF8, 0, input, inlen, output, outlen);
        output[outlen] = L'\0';
    }
    return output;
}

// wstring (used by win32) to utf8 (used everywhere)
char* strshort_( const wchar_t* input ) {
    if( !input ) {
        return NULL;
    }
    int inlen = wcslen(input);
    int outlen = WideCharToMultiByte(CP_UTF8, 0, input, inlen, 0, 0, NULL, NULL);
    char *output = va("%*.s", (int)((outlen+1) * sizeof(char)), "");
    if( output ) {
        WideCharToMultiByte(CP_UTF8, 0, input, inlen, output, outlen, NULL, NULL);
        output[outlen] = '\0';
    }
    return output;
}

#else

wchar_t* strwide_( const char* input ) {
    return NULL;
}

char* strshort_( const wchar_t* input ) {
    return NULL;
}

#endif
