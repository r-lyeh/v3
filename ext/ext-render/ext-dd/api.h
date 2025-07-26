#ifndef DD_H
#define DD_H

// public api

typedef struct dd_flags {
    //@todo: add numframes, inccolor, quality
    union { vec3 position, pos, p, center, c; };
    union { vec3 normal, n, updir, up, u; };
    union { vec3 direction, dir, d, forward, fwd, f; };
    union { vec3 step, incpos; };
    union { vec3 stepn, incn; };
    union { float radius, r, scale, s, length, l; };
    union {
        struct {
        uint64_t top:1;     // always on top? (default: no)
        uint64_t thin:2;    // line/point thin size [0..3] (default:0) \ @todo: apply to both line/points
        uint64_t dashed:1;  // dashed style or solid (default: solid)  / @todo: rename dashed as shape: =dot/dashed for lines, =symbol for points
        uint64_t type:1;    // enum { lines=0, dots=1 }
        uint64_t aa:1;      // use antialias (default: yes)
        uint64_t count:10;  // amount of elements to draw (default: 1)
        uint64_t color:32;  // RGB color (default: auto)
        };
        uint64_t bits;
        uint64_t key;
    };
} dd_flags, dd_flags_t;

void dd_point(vec3 pos, dd_flags options);
void dd_line(vec3 from, vec3 to, dd_flags options);
void dd_camera(vec3 pos, dd_flags options);
void dd_frustum(mat44 proj, mat44 view, dd_flags options);
void dd_grid(dd_flags options);
void dd_axis(dd_flags options);
void dd_sphere(vec3 center, dd_flags options);
void dd_plane(vec3 p, vec3 n, dd_flags options);

void dd_triangle(vec3 pa, vec3 pb, vec3 pc, dd_flags options);
void dd_arrow(vec3 begin, vec3 end, dd_flags options);
void dd_circle(vec3 pos, vec3 n, float r, dd_flags options);
void dd_normal(vec3 pos, vec3 n, dd_flags options);
void dd_prism(vec3 center, float radius, float height, vec3 normal, int segments, dd_flags options);
void dd_text(vec3 pos, float scale, const char *text, dd_flags options);
/*
void dd_aabb(vec3 minbb, vec3 maxbb);
void dd_boid(vec3 position, vec3 dir);
void dd_bone(vec3 center, vec3 end);
void dd_box(vec3 c, vec3 extents);
void dd_camera_deprecated(vec3 pos, vec3 fwd, vec3 up, mat44 proj, mat44 view);
void dd_capsule(vec3 from, vec3 to, float r);
void dd_cone(vec3 center, vec3 top, float radius);
void dd_cone_lowres(vec3 center, vec3 top, float radius);
void dd_cube(vec3 center, float radius);
void dd_cube33(vec3 center, vec3 radius, mat33 M);
void dd_cylinder(vec3 center, float height, int segments);
void dd_diamond(vec3 from, vec3 dir, float len);
void dd_hexagon(vec3 pos, float r);
void dd_pentagon(vec3 pos, float r);
void dd_position( vec3 position, float radius );
void dd_position_dir( vec3 position, vec3 direction, float radius );
void dd_pyramid(vec3 center, float height, int segments);
void dd_ring(vec3 pos, vec3 n, float r);
void dd_square(vec3 pos, float r);
void dd_text2d(vec2 pos, const char *text);
void dd_triangle(vec3 pa, vec3 pb, vec3 pc, dd_flags st);

void dd_bounds(const vec3 points[8]);
void dd_bounds_corners(const vec3 points[8]);
*/
void dd_demo();

#endif

#ifndef DD_DEFAULTS_H
#define DD_DEFAULTS_H

#define dd_hash3(s) ((0[s] ^ ((1[s] ^ ((2[s] ^ 0) * 131ull)) * 131ull)) * 131ull)
#define dd_flags(...) (CAST(dd_flags) { \
    .aa = 1, .count = 1, .n = vec3(0, 1, 0),\
    .color = 5 + (dd_hash3(__func__+sizeof(__func__)-3) % 11), ##__VA_ARGS__ })

#define dd_line(from,to,...)    dd_line(from,to,dd_flags(__VA_ARGS__))
#define dd_point(pos,...)       dd_point(pos,dd_flags(.type = 1, ##__VA_ARGS__))
#define dd_camera(pos,...)      dd_camera(pos,dd_flags(.up=vec3(0,1,0), .fwd=vec3(0,0,1), __VA_ARGS__))
#define dd_grid(...)            dd_grid(dd_flags(__VA_ARGS__))
#define dd_axis(...)            dd_axis(dd_flags(.scale = 1, ##__VA_ARGS__))
#define dd_sphere(pos,...)      dd_sphere(pos,dd_flags(.r = 1, ##__VA_ARGS__))
#define dd_plane(p,n,...)       dd_plane(p,n,dd_flags(.s = 1, ##__VA_ARGS__))
#define dd_frustum(pj, vw, ...) dd_frustum(pj,vw,dd_flags(__VA_ARGS__))

#define dd_triangle(pa,pb,pc,...) dd_triangle(pa,pb,pc,dd_flags(__VA_ARGS__))
#define dd_arrow(from,to,...) dd_arrow(from,to,dd_flags(__VA_ARGS__))
#define dd_normal(pos,n,...) dd_normal(pos,n,dd_flags(__VA_ARGS__))
#define dd_circle(pos,n,r,...) dd_circle(pos,n,r,dd_flags(__VA_ARGS__))
#define dd_prism(pos,r,h,n,segments,...) dd_prism(pos,r,h,n,segments,dd_flags(__VA_ARGS__))
#define dd_text(pos,scale,text,...) dd_text(pos,scale,text,dd_flags(.color = V3_WHITE, __VA_ARGS__))
#define dd_circle__with_orientation(pos,dir,r,...) dd_circle__with_orientation(pos,dir,r,dd_flags(__VA_ARGS__))
#define dd_aabb(minbb,maxbb,...) dd_aabb(minbb,maxbb,dd_flags(__VA_ARGS__))

#elif CODE
#pragma once
#include v3_dd_backend
#include "dd(common).h"

// Helper to transform vec3 to vec4, apply matrix, and convert back
#define transform_point(p3) vec4_xyz(mat44_mul_vec4(transform,vec4(p3.x,p3.y,p3.z,1)))

mat44 dd_make_transform(vec3 position, vec3 direction, vec3 upvector) { // transformation matrix (rotation+translation)
    // remember that object vertices that will be multiplied by
    // this transformation matrix must be located at (0,0,0).

    mat44 m;
    vec3 z_axis = norm3(direction); // Forward (local z)
    vec3 x_axis = norm3(cross3(direction, upvector)); // Right (local x)
    vec3 y_axis = norm3(cross3(x_axis, z_axis)); // Up (local y)

    m.x = vec4(x_axis.x, y_axis.x, z_axis.x, position.x);
    m.y = vec4(x_axis.y, y_axis.y, z_axis.y, position.y);
    m.z = vec4(x_axis.z, y_axis.z, z_axis.z, position.z);
    m.w = vec4(0,0,0, 1.0f);

    return m;
}

#define DD_PI VECMATH_PI
#define DD_DEG2RAD (DD_PI / 180.0)

#include "dd_camera.h"
#include "dd_frustum.h"
#include "dd_grid.h"
#include "dd_axis.h"
#include "dd_sphere.h"
#include "dd_plane.h"

void (dd_triangle)(vec3 pa, vec3 pb, vec3 pc, dd_flags st) {
    (dd_line)(pa, pb, st);
    (dd_line)(pa, pc, st);
    (dd_line)(pb, pc, st);
}

void (dd_text)(vec3 pos, float scale, const char *text, dd_flags st) {
    // [ref] http://paulbourke.net/dataformats/hershey/ (PD)
    // [ref] https://sol.gfxile.net/hershey/fontprev.html (WTFPL2)
    static const char *hershey[] = { /* simplex font */
    "AQ","IKFVFH@@FCEBFAGBFC","FQEVEO@@MVMO","LVLZE:@@RZK:@@EMSM@@DGRG","[UIZI=@@MZ"
    "M=@@RSPUMVIVFUDSDQEOFNHMNKPJQIRGRDPBMAIAFBDD","`YVVDA@@IVKTKRJPHOFODQDSEUGVIVK"
    "UNTQTTUVV@@RHPGOEOCQASAUBVDVFTHRH","c[XMXNWOVOUNTLRGPDNBLAHAFBECDEDGEIFJMNNOOQ"
    "OSNULVJUISIQJNLKQDSBUAWAXBXC","HKFTEUFVGUGSFQEP","KOLZJXHUFQELEHFCH?J<L:","KOD"
    "ZFXHUJQKLKHJCH?F<D:","IQIVIJ@@DSNM@@NSDM","F[NSNA@@EJWJ","IKGBFAEBFCGBG@F>E=",\
    "C[EJWJ","FKFCEBFAGBFC","CWUZC:","RUJVGUERDMDJEEGBJALAOBQERJRMQROULVJV","EUGRIS"
    "LVLA","OUEQERFTGUIVMVOUPTQRQPPNNKDARA","PUFVQVKNNNPMQLRIRGQDOBLAIAFBECDE","GUN"
    "VDHSH@@NVNA","RUPVFVEMFNIOLOONQLRIRGQDOBLAIAFBECDE","XUQSPUMVKVHUFREMEHFDHBKAL"
    "AOBQDRGRHQKOMLNKNHMFKEH","FURVHA@@DVRV","^UIVFUESEQFOHNLMOLQJRHREQCPBMAIAFBECD"
    "EDHEJGLJMNNPOQQQSPUMVIV","XUQOPLNJKIJIGJELDODPESGUJVKVNUPSQOQJPENBKAIAFBED","L"
    "KFOENFMGNFO@@FCEBFAGBFC","OKFOENFMGNFO@@GBFAEBFCGBG@F>E=","DYUSEJUA","F[EMWM@@"
    "EGWG","DYESUJEA","USDQDRETFUHVLVNUOTPRPPONNMJKJH@@JCIBJAKBJC","x\\SNRPPQMQKPJO"
    "ILIIJGLFOFQGRI@@MQKOJLJIKGLF@@SQRIRGTFVFXHYKYMXPWRUTSUPVMVJUHTFREPDMDJEGFEHCJB"
    "MAPASBUCVD@@TQSISGTF","ISJVBA@@JVRA@@EHOH","XVEVEA@@EVNVQURTSRSPRNQMNL@@ELNLQK"
    "RJSHSERCQBNAEA","SVSQRSPUNVJVHUFSEQDNDIEFFDHBJANAPBRDSF","PVEVEA@@EVLVOUQSRQSN"
    "SIRFQDOBLAEA","LTEVEA@@EVRV@@ELML@@EARA","ISEVEA@@EVRV@@ELML","WVSQRSPUNVJVHUF"
    "SEQDNDIEFFDHBJANAPBRDSFSI@@NISI","IWEVEA@@SVSA@@ELSL","CIEVEA","KQMVMFLCKBIAGA"
    "EBDCCFCH","IVEVEA@@SVEH@@JMSA","FREVEA@@EAQA","LYEVEA@@EVMA@@UVMA@@UVUA","IWEV"
    "EA@@EVSA@@SVSA","VWJVHUFSEQDNDIEFFDHBJANAPBRDSFTITNSQRSPUNVJV","NVEVEA@@EVNVQU"
    "RTSRSORMQLNKEK","YWJVHUFSEQDNDIEFFDHBJANAPBRDSFTITNSQRSPUNVJV@@MES?","QVEVEA@@"
    "EVNVQURTSRSPRNQMNLEL@@LLSA","UURSPUMVIVFUDSDQEOFNHMNKPJQIRGRDPBMAIAFBDD","FQIV"
    "IA@@BVPV","KWEVEGFDHBKAMAPBRDSGSV","FSBVJA@@RVJA","LYCVHA@@MVHA@@MVRA@@WVRA",""
    "FUDVRA@@RVDA","GSBVJLJA@@RVJL","IURVDA@@DVRV@@DARA","LOEZE:@@FZF:@@EZLZ@@E:L:",
    "COAVO>","LOJZJ:@@KZK:@@DZKZ@@D:K:","KQGPISKP@@DMIRNM@@IRIA","CQA?Q?","HKGVFUES"
    "EQFPGQFR","RTPOPA@@PLNNLOIOGNELDIDGEDGBIALANBPD","RTEVEA@@ELGNIOLONNPLQIQGPDNB"
    "LAIAGBED","OSPLNNLOIOGNELDIDGEDGBIALANBPD","RTPVPA@@PLNNLOIOGNELDIDGEDGBIALANB"
    "PD","RSDIPIPKOMNNLOIOGNELDIDGEDGBIALANBPD","IMKVIVGUFRFA@@COJO","WTPOP?O<N;L:I"
    ":G;@@PLNNLOIOGNELDIDGEDGBIALANBPD","KTEVEA@@EKHNJOMOONPKPA","IIDVEUFVEWDV@@EOE"
    "A","LKFVGUHVGWFV@@GOG>F;D:B:","IREVEA@@OOEE@@IIPA","CIEVEA","S_EOEA@@EKHNJOMOO"
    "NPKPA@@PKSNUOXOZN[K[A","KTEOEA@@EKHNJOMOONPKPA","RTIOGNELDIDGEDGBIALANBPDQGQIP"
    "LNNLOIO","RTEOE:@@ELGNIOLONNPLQIQGPDNBLAIAGBED","RTPOP:@@PLNNLOIOGNELDIDGEDGBI"
    "ALANBPD","INEOEA@@EIFLHNJOMO","RROLNNKOHOENDLEJGILHNGOEODNBKAHAEBDD","IMFVFEGB"
    "IAKA@@COJO","KTEOEEFBHAKAMBPE@@POPA","FQCOIA@@OOIA","LWDOHA@@LOHA@@LOPA@@TOPA",
    "FRDOOA@@OODA","JQCOIA@@OOIAG=E;C:B:","IROODA@@DOOO@@DAOA","hOJZHYGXFVFTGRHQIOI"
    "MGK@@HYGWGUHSIRJPJNILEJIHJFJDIBHAG?G=H;@@GIIGIEHCGBF@F>G<H;J:","CIEZE:","hOFZH"
    "YIXJVJTIRHQGOGMIK@@HYIWIUHSGRFPFNGLKJGHFFFDGBHAI?I=H;@@IIGGGEHCIBJ@J>I<H;F:",""
    "XYDGDIELGMIMKLOIQHSHUIVK@@DIEKGLILKKOHQGSGUHVKVM" };
    vec3 src = pos, old = {0}; float abs_scale = fabs(scale);
    for( signed char c; (c = *text++, c > 0 && c < 127); ) {
        if( c == '\n' || c == '\r' ) {
            pos.x = src.x, pos.y -= scale * ((signed char)hershey['W'-32][1] - 65) * 1.25f; // spacing @1
        } else {
            const char *glyph = (const char*)hershey[c - 32];
            if( c > 32 ) for( int pen = 0, i = 0; i < (glyph[0] - 65); i++ ) { // verts @0
                int x = glyph[2 + i*2 + 0] - 65, y = glyph[2 + i*2 + 1] - 65;
                if( x == -1 && y == -1 ) pen = 0; else {
                    vec3 next = add3(pos, vec3(abs_scale*x, scale*y, 0));
                    if( !pen ) pen = 1; else (dd_line)(old, next, st);
                    old = next;
                }
            }
            pos.x += abs_scale * (glyph[1] - 65); // spacing @1
        }
    }
}


void dd_cube(vec3 center, float radius) { // draw_prism(center, 1, -1, vec3(0,1,0), 4);
    float half = radius * 0.5f;
    vec3 l = vec3(center.x-half,center.y+half,center.z-half); // left-top-far
    vec3 r = vec3(center.x+half,center.y-half,center.z+half); // right-bottom-near

    dd_line(l, vec3(r.x,l.y,l.z));
    dd_line(vec3(r.x,l.y,l.z), vec3(r.x,l.y,r.z));
    dd_line(vec3(r.x,l.y,r.z), vec3(l.x,l.y,r.z));
    dd_line(vec3(l.x,l.y,r.z), l);
    dd_line(l, vec3(l.x,r.y,l.z));

    dd_line(r, vec3(l.x,r.y,r.z));
    dd_line(vec3(l.x,r.y,r.z), vec3(l.x,r.y,l.z));
    dd_line(vec3(l.x,r.y,l.z), vec3(r.x,r.y,l.z));
    dd_line(vec3(r.x,r.y,l.z), r);
    dd_line(r, vec3(r.x,l.y,r.z));

    dd_line(vec3(l.x,l.y,r.z), vec3(l.x,r.y,r.z));
    dd_line(vec3(r.x,l.y,l.z), vec3(r.x,r.y,l.z));
}

void (dd_aabb)(vec3 minbb, vec3 maxbb, dd_flags opt) {
    vec3 points[8], bb[2] = { minbb, maxbb };
    for (int i = 0; i < 8; ++i) {
        points[i].x = bb[(i ^ (i >> 1)) & 1].x;
        points[i].y = bb[     (i >> 1)  & 1].y;
        points[i].z = bb[     (i >> 2)  & 1].z;
    }
    (dd_bounds/*_corners*/)(points, opt);
}

void (dd_normal)(vec3 pos, vec3 n, dd_flags st) {
    (dd_line)(pos, add3(pos, norm3(n)), st);
}

void dd_boid(vec3 position, vec3 dir) {
    dir = norm3(dir);

    // if n is too similar to up vector, use right. else use up vector
    vec3 v1 = cross3(dir, dot3(dir, vec3(0,1,0)) > 0.8f ? vec3(1,0,0) : vec3(0,1,0));
    vec3 v2 = cross3(dir, v1);
    v1 = cross3(dir, v2);

    unsigned col = position.y < 0 ? V3_ORANGE : V3_CYAN;

        vec3 front = add3(position, scale3(dir, 1));
        vec3 back = add3(position, scale3(dir, -0.25f));
        vec3 right = add3(back, scale3(v1, 0.5f));
        vec3 left = add3(back, scale3(v1, -0.5f));
        dd_line( front, left, .color = col );
        dd_line( left, position, .color = col );
        dd_line( position, right, .color = col );
        dd_line( right, front, .color = col );
}

void (dd_prism)(vec3 center, float radius, float height, vec3 normal, int segments, dd_flags st) {
    vec3 left, up;
    basis3(&left, &up, normal);

    vec3 point, lastPoint;
    up = scale3(up, radius);
    left = scale3(left, radius);
    lastPoint = add3(center, up);
    vec3 pivot = add3(center, scale3(normal, height));

    for (int i = 1; i <= segments; ++i) {
        const float radians = (DD_PI * 2) * i / segments;

        vec3 vs = scale3(left, sinf(radians));
        vec3 vc = scale3(up,   cosf(radians));

        point = add3(center, vs);
        point = add3(point,  vc);

        (dd_line)(lastPoint, point, st);
        if( height > 0 ) dd_line(point, pivot);
        else if(height < 0) {
            (dd_line)(point, add3(point,scale3(normal, -height)), st);
        }
        lastPoint = point;
    }

    if(height < 0) (dd_prism)(add3(center, scale3(normal, -height)), radius, 0, normal, segments, st);
}

void (dd_circle)(vec3 pos, vec3 n, float r, dd_flags st) { (dd_prism)(pos, r, 0, n, 32, st); }
void dd_ring(vec3 pos, vec3 n, float r) { dd_circle(pos,n,r);dd_circle(pos,n,r*0.90); }
void dd_hexagon(vec3 pos, float r) { dd_prism(pos, r, 0, vec3(0,1,0), 6); }
void dd_pentagon(vec3 pos, float r) { dd_prism(pos, r, 0, vec3(0,1,0), 5); }
void dd_square(vec3 pos, float r) { dd_prism(pos, r, 0, vec3(0,1,0), 4); }
//void dd_triangle(vec3 pos, float r) { dd_prism(pos, r, 0, vec3(0,1,0), 3); }
void dd_capsule(vec3 from, vec3 to, float r) {
    /* calculate axis */
    vec3 up, right, forward;
    forward = sub3(to, from);
    forward = norm3(forward);
    basis3(&right, &up, forward);

    /* calculate first two cone verts (buttom + top) */
    vec3 lastf, lastt;
    lastf = scale3(up,r);
    lastt = add3(to,lastf);
    lastf = add3(from,lastf);

    /* step along circle outline and draw lines */
    enum { step_size = 20 };
    for (int i = step_size; i <= 360; i += step_size) {
        /* calculate current rotation */
        vec3 ax = scale3(right, sinf(i*DD_DEG2RAD));
        vec3 ay = scale3(up, cosf(i*DD_DEG2RAD));

        /* calculate current vertices on cone */
        vec3 tmp = add3(ax, ay);
        vec3 pf = scale3(tmp, r);
        vec3 pt = scale3(tmp, r);

        pf = add3(pf, from);
        pt = add3(pt, to);

        /* draw cone vertices */
        dd_line(lastf, pf);
        dd_line(lastt, pt);
        dd_line(pf, pt);

        lastf = pf;
        lastt = pt;

        /* calculate first top sphere vert */
        vec3 prevt = scale3(tmp, r);
        vec3 prevf = add3(prevt, from);
        prevt = add3(prevt, to);

        /* sphere (two half spheres )*/
        for (int j = 1; j < 180/step_size; j++) {
            /* angles */
            float ta = j*step_size;
            float fa = 360-(j*step_size);

            /* top half-sphere */
            ax = scale3(forward, sinf(ta*DD_DEG2RAD));
            ay = scale3(tmp, cosf(ta*DD_DEG2RAD));

            vec3 t = add3(ax, ay);
            pf = scale3(t, r);
            pf = add3(pf, to);
            dd_line(pf, prevt);
            prevt = pf;

            /* bottom half-sphere */
            ax = scale3(forward, sinf(fa*DD_DEG2RAD));
            ay = scale3(tmp, cosf(fa*DD_DEG2RAD));

            t = add3(ax, ay);
            pf = scale3(t, r);
            pf = add3(pf, from);
            dd_line(pf, prevf);
            prevf = pf;
        }
    }
}
void dd_pyramid(vec3 center, float height, int segments) {
    dd_prism(center, 1, height, vec3(0,1,0), segments);
}
void dd_cylinder(vec3 center, float height, int segments) {
    dd_prism(center, 1, -height, vec3(0,1,0), segments);
}
void dd_cone(vec3 center, vec3 top, float radius) {
    vec3 diff3 = sub3(top, center);
    dd_prism(center, radius ? radius : 1, len3(diff3), norm3(diff3), 24);
}
void dd_cone_lowres(vec3 center, vec3 top, float radius) {
    vec3 diff3 = sub3(top, center);
    dd_prism(center, radius ? radius : 1, len3(diff3), norm3(diff3), 3);
}
void dd_bone(vec3 center, vec3 end) {
    vec3 diff3 = sub3(end, center);
    float len = len3(diff3), len10 = len / 10;
    dd_prism(center, len10, 0, vec3(1,0,0), 24);
    dd_prism(center, len10, 0, vec3(0,1,0), 24);
    dd_prism(center, len10, 0, vec3(0,0,1), 24);
    dd_line(end, add3(center, vec3(0,+len10,0)));
    dd_line(end, add3(center, vec3(0,-len10,0)));
}

void (dd_arrow)(vec3 begin, vec3 end, dd_flags st) {
    vec3 diff = sub3(end, begin);
    float len = len3(diff), stick_len = len * 2 / 3;

    (dd_line)(begin, end, st);
    dd_cone_lowres(add3(begin, scale3(norm3(diff), stick_len)), end, len / 6);
}

void (dd_circle__with_orientation)(vec3 center, vec3 dir, float radius, dd_flags st) {
    // we'll skip 3 segments out of 32. 1.5 per half circle.
    int segments = 32, skip = 3, drawn_segments = segments-skip;

//  dir = norm3(dir);
    vec3 right = cross3(dir, vec3(0,1,0));
    vec3 up = cross3(dir, right);
    right = cross3(dir, up);

    vec3 point, lastPoint;
    dir = scale3(dir, radius);
    right = scale3(right, radius);
    //lastPoint = add3(center, dir);

    {
        const float radians = (DD_PI * 2) * (0+skip/2.f) / segments;
        vec3 vs = scale3(right, sinf(radians));
        vec3 vc = scale3(dir,   cosf(radians));
        lastPoint = add3(center, vs);
        lastPoint = add3(lastPoint, vc);
    }

    //.color = V3_GREEN
    (dd_line)(lastPoint, add3(center, scale3(dir, radius * (radius <= 1 ? 1.25 : 0.65) )), st);

    for (int i = 0; i <= drawn_segments; ++i) {
        const float radians = (DD_PI * 2) * (i+skip/2.f) / segments;

        vec3 vs = scale3(right, sinf(radians));
        vec3 vc = scale3(dir,   cosf(radians));

        point = add3(center, vs);
        point = add3(point,  vc);

        //.color = V3_YELLOW
        (dd_line)(lastPoint, point, st);
        lastPoint = point;
    }

    //.color = V3_PINK
    (dd_line)(lastPoint, add3(center, scale3(dir, radius * (radius <= 1 ? 1.25 : 0.65) )), st);
}

void dd_position_dir( vec3 position, vec3 direction, float radius ) {
    // idea from http://www.cs.caltech.edu/~keenan/m3drv.pdf and flotilla game UI
    unsigned col = position.y < 0 ? V3_PINK/*ORANGE*/ : V3_CYAN;

    vec3 ground = vec3(position.x, 0, position.z);
    dd_point( ground, .color = col );
    dd_point( position, .color = col );
    dd_line( ground, position, .color = col, .dashed = position.y < 0 );

    vec3 n = norm3(direction), up = vec3(0,1,0);
    for( int i = 0; i < 10 && i <= fabs(position.y); ++i ) {
        if( i < 2 && len3(direction) )
        dd_circle__with_orientation(ground, n, radius, .color = col);
        else
        dd_circle(ground, up, radius, .color = col);
        radius *= 0.9f;
    }
}

void dd_position( vec3 position, float radius ) {
    dd_position_dir(position, vec3(0,0,0), radius);
}


void dd_diamond(vec3 from, vec3 dir, float len) {
    vec3 up = norm3(dir);
    
    vec3 fwd, right;
    basis3(&fwd, &right, up);

    up = scale3(up, len);
    fwd = scale3(fwd, len);
    right = scale3(right, len);

    vec3 top = add3(from, up);
    vec3 bottom = sub3(from, up);

    vec3 a = add3(from, fwd);
    vec3 b = sub3(from, fwd);
    vec3 c = add3(from, right);
    vec3 d = sub3(from, right);

    /* draw vertices */
    dd_line(a, c);
    dd_line(a, d);
    dd_line(b, c);
    dd_line(b, d);

    dd_line(a, top);
    dd_line(b, top);
    dd_line(c, top);
    dd_line(d, top);

    dd_line(a, bottom);
    dd_line(b, bottom);
    dd_line(c, bottom);
    dd_line(d, bottom);
}



#if 0
void dd_cube33(vec3 center, vec3 radius, mat33 M) {
    vec3 half = scale3(radius, 0.5f);
    vec3 l = vec3(-half.x,+half.y,-half.z); // left-top-far
    vec3 r = vec3(+half.x,-half.y,+half.z); // right-bottom-near

    vec3 points[8] = {
        vec3(l.x, r.y, r.z),
        vec3(l.x, r.y, l.z),
        vec3(r.x, r.y, l.z),
        vec3(r.x, r.y, r.z),
        vec3(l.x, l.y, r.z),
        vec3(l.x, l.y, l.z),
        vec3(r.x, l.y, l.z),
        vec3(r.x, l.y, r.z),
    };

    for( int i = 0; i < 8; ++i ) {
        points[i] = add3(center, transform33(M, points[i]));
    }

    dd_bounds(points);
}
void dd_box(vec3 c, vec3 extents) {
    vec3 points[8], whd = scale3(extents, 0.5f);
    #define DD_BOX_V(v, op1, op2, op3) (v).x = c.x op1 whd.x; (v).y = c.y op2 whd.y; (v).z = c.z op3 whd.z
    DD_BOX_V(points[0], -, +, +);
    DD_BOX_V(points[1], -, +, -);
    DD_BOX_V(points[2], +, +, -);
    DD_BOX_V(points[3], +, +, +);
    DD_BOX_V(points[4], -, -, +);
    DD_BOX_V(points[5], -, -, -);
    DD_BOX_V(points[6], +, -, -);
    DD_BOX_V(points[7], +, -, +);
    #undef DD_BOX_V
    dd_bounds(points);
}

#endif

void dd_demo() {
#if 0
    // freeze current frame for (frustum) camera forensics
    static mat44 copy_proj, copy_view;
    ONCE copy_proj = last_proj, copy_view = last_view;
    dd_frustum(copy_proj, copy_view);
#endif

    dd_grid();

    vec3 origin = {0,0,0};
    dd_arrow(origin, vec3(-1,1,1), .color = V3_ORANGE);
    dd_text(vec3(-1,1,1), 0.008f, va("hello 1%s2!", "world"), .color = V3_YELLOW);

    const char abc[] = " !\"#$%&'()*+,-./\n"
        "0123456789:;<=>?@\n"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`\n"
        "abcdefghijklmnopqrstuvwxyz{|}~";
    dd_text(vec3(2,2,2), 0.008f, abc);

#if 1
    for( int i = -5; i <= 5; ++i ) {
        dd_pyramid(vec3(i*2,0,3),  0, i+5+2); dd_text(vec3(i*2,0,3), 0.008f, va("%d/1", i));
        dd_pyramid(vec3(i*2,0,6), -2, i+5+2); dd_text(vec3(i*2,0,6), 0.008f, va("%d/2", i));
        dd_pyramid(vec3(i*2,0,9), +2, i+5+2); dd_text(vec3(i*2,0,9), 0.008f, va("%d/3", i));
    }
#endif

#if 1 // @fixme: add positions to these
    // dd_triangle(origin, 1);
    dd_square(origin, 1);
    dd_pentagon(origin, 1);
    dd_hexagon(origin, 1);
    dd_cube(origin, 1);
    dd_pyramid(origin, 2, 3);
    dd_pyramid(origin, 2, 16);
    dd_cone(origin, add3(origin, vec3(0,1,0)), 0.5f);
    dd_arrow(origin, vec3(0,1,0));
    dd_bone(vec3(0,0,0), vec3(3,3,3));
    dd_aabb(vec3(0,0,0), vec3(1,1,1));
#endif

    dd_plane(vec3(0,10,0), vec3(0,1,0), .scale = 10);

#if 1
    dd_boid(vec3(15,0,15), vec3(-15,0,-15) );
    dd_position(vec3(10,10,10), 1);
    dd_position(vec3(-10,-10,10), 1);
#endif

    dd_point(vec3(-2,0,-2));
    dd_sphere(vec3(-3,0,-3),1, .color=V3_PURPLE);

    dd_diamond(vec3(10,10,10), vec3(0,1,0), 1);
}

#if 0
void dd_text2d(vec2 pos, const char *text) {
    struct text2d_cmd t = {0};
    t.sca = 0.5f; // 0.5 is like vertical 12units each
    t.pos = vec3(pos.x, 0 - pos.y - 12, 0);
    t.str = text;
    t.col = dd_color;
    array_push(dd_text2d, t);
}
#endif

#endif
