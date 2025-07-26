typedef struct recipe_t {
    int ziplevel;
    const char *pathmasks; // "*.jpg;**.png" > "ktx"
    const char *script; // @todo: int errorlevel;
} recipe_t;

recipe_t cookbook[256]; int cookbook_idx;

int recipe_( const char *pathmasks, int ziplevel, const char *script ) {
    assert(cookbook_idx<COUNTOF(cookbook));
    cookbook[cookbook_idx++] = ((recipe_t){ziplevel, strdup(pathmasks), strdup(script)}); // @leaks
    return 1;
}

const char *cook__( const char *source) {
    // find recipe for asset in cookbook. abort if no recipe is found
    const recipe_t *recipe = 0;
    for( int i = 0; i < cookbook_idx && !recipe; ++i ) {
        for each_string(ext, cookbook[i].pathmasks, ";") {
            if( strmatchi(source, ext) ) {
                recipe = cookbook+i; break;
            }
        }
    }
    if( !recipe ) return source;

    //flockfile_(source_fp);

    // extract final extension
    int has_ext = !!strrstr(recipe->script, "OUTPUT.");

    //
    char *ps = path(source);
    char *ns = path_name(source);
    char *target = va("%s.%s", ps, ns + (ns[0] == '.'));

    //
    char *INPUT = 0;
    char *OUTPUT = 0;

    // generate batch/bash script
    char* batch = 0;
    for each_string(token, recipe->script, " \n") { // @todo: multiline support: \n>& \n>;
        stradd(&batch, " ");
        /**/ if( strstri(token, ".EXE") ) stradd(&batch, path_norm( va("v3/%.*s%s", (int)(strlen(token)-4), token, ifdef(V3_WINDOWS,".exe", ifdef(V3_LINUX,".linux",".osx")) )));
        else if( strstri(token, "ext/") || strstri(token, "ext\\") ) stradd(&batch, path_norm( va("v3/%s", token) ));
        else if( !strcmp(token, "REN") ) stradd(&batch, ifdef(V3_WINDOWS, "move /y", "mv -f"));
        else if( !strcmp(token, "DEL") ) stradd(&batch, ifdef(V3_WINDOWS, "del", "rm"));
        else if( !strcmp(token, "NUL") ) stradd(&batch, ifdef(V3_WINDOWS, "nul", "/dev/null"));
        else if( !strncmp(token, "INPUT", 5) ) stradd(&batch, path_norm(INPUT = va("%s%s", source, token+5))), token = 0;
        else if( !strncmp(token, "OUTPUT", 6) ) stradd(&batch, path_norm(OUTPUT = va("%s%s", target, token+6))), token = 0;
        else stradd(&batch, token);
    }

    // debug run cook script
    //lputs(va("Cooking %s > %s ... (%s)", INPUT, OUTPUT, recipe->script/*batch*/));
    //puts(batch);

    // run it
    int rc = system(batch);
    resize(batch, 0);

    //funlockfile_(source_fp);

    // abort on errors
    if( rc ) return lputs("\2error while cooking:"), lputs(source), source; // //if( rc != recipe.errorlevel )

    // find recipe for asset in cookbook
    const recipe_t *found = 0;
    for( int i = 0; i < cookbook_idx && !found; ++i ) {
        for each_string(ext, cookbook[i].pathmasks, ";") {
            if( strmatchi(target, ext) ) {
                found = cookbook+i; break;
            }
        }
    }

    // try another round. this creates a chain with all recipes until
    // cookbook is exhausted... eg: mod->flac, then flac->wav, then wav->ogg
    return cook__(OUTPUT);
}

const char *cook_( const char *source ) {
    // return early if previously cooked
    char target[FILENAME_MAX];
    snprintf(target, FILENAME_MAX, "%s.%s", path(source), path_name(source));
    if( path_stat(target) == 1 ) return va("%s", target);
    // copy cooked file and return its path
    const char *cooked = cook__(source);
    for( FILE *fp = fopen(cooked, "rb"); fp; fclose(fp), fp = 0 ) {
        for( FILE *out = fopen(target, "wb"); out; fclose(out), out = 0 ) {
            char buf[4096];
            int len;
            do {
                len = fread(buf, 1, sizeof(buf), fp);
                fwrite(buf, 1, len, out);
            } while(len > 0);
        }
    }
    return va("%s", target);
}
