char* download( const char *url, int *len ); // must free() after use

#ifdef _WIN32
#include <wininet.h>
#pragma comment(lib,"wininet")

char* download( const char *url, int *len ) { // must free() after use
    char *ptr = 0; int cap = 0;

    if( strchr(url, '+') ) {
        char escaped[512], *q = escaped;
        for( const char *p = url; *p ; ) {
            if (*p == '+') {
                *q++ = '%';
                *q++ = '2';
                *q++ = 'B';
                p++;
            } else {
                *q++ = *p++;
            }
        }
        *q = '\0';
        url = va("%s", escaped);
    }

    int ok = 0;
    char buffer[ 4096 ];
    DWORD response_size = 0;

    for( DWORD bufsz = sizeof(buffer); InternetCanonicalizeUrlA(url, buffer, &bufsz, 0) && url && (url = buffer); url = 0 )
    for( HINTERNET session = InternetOpenA("" /*"fwk.download_file"*/, PRE_CONFIG_INTERNET_ACCESS, NULL,NULL/*INTERNET_INVALID_PORT_NUMBER*/, 0); session; InternetCloseHandle(session), session = 0 ) // @fixme: download_file
    for( HINTERNET request = InternetOpenUrlA(session, url, NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE/*|INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_KEEP_CONNECTION*/, 0); request; InternetCloseHandle(request), request = 0 )
    for( ; (ok = !!InternetReadFile(request, buffer, sizeof(buffer), &response_size)) && response_size > 0 ; ) {
        ptr = realloc(ptr, cap += response_size );
        memcpy(ptr + (cap - response_size), buffer, response_size);
    }

    if( !ok ) {
        if( ptr ) free(ptr);
        return NULL;
    }

    if( len ) *len = cap;
    return ptr;
}

#elif 1

char* download( const char *url, int *len ) { // must free() after use
    char *ptr = 0; int cap = 0;

    int ok = 0;
    char buffer[ 4096 ];

    // curl: -b cookies -c cookies -H="Accept: text/html" -H="Connection: keep-alive" -H="User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_0) AppleWebKit/600.1.17 (KHTML, like Gecko) Version/8.0 Safari/600.1.17"

    if( url[0] != '!' ) {
        if( !ok ) sprintf(buffer, "!curl -L '%s' 2>/dev/null", url), ptr = download(buffer, len), ok = !!ptr;
        if( !ok ) sprintf(buffer, "!wget -qO- '%s' 2>/dev/null", url), ptr = download(buffer, len), ok = !!ptr;
        if(  ok ) return ptr;
    }
    else
    for( FILE *fp = popen(url+1, "r"); fp; pclose(fp), fp = 0)
    for(; !feof(fp); ) {
        int count = fread(buffer, 1, sizeof(buffer), fp);
        ok = count > 0;
        if(!ok) break;

        ptr = realloc(ptr, cap += count );
        memcpy(ptr + (cap - count), buffer, count);
    }

    if( !ok ) {
        if( ptr ) free(ptr);
        return NULL;
    }

    if( len ) *len = cap;
    return ptr;
}

#endif
