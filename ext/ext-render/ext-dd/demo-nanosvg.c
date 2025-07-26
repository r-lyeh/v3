#ifdef NANOSVG_H

//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <stdio.h>
#include <string.h>
#include <float.h>

#define V3_IMPLEMENTATION
#include "v3.h"

#define dd_strip(num,arr,...) dd_strip(num,arr,dd_flags(__VA_ARGS__))
void (dd_strip)(int num, vec3* arr, dd_flags opt) {
    if( num > 1)
    for( int i = 1; i < num; ++i ) {
        (dd_line)(arr[i-1], arr[i], opt);
    }
    // if closed dd_line(arr[0], arr[num-1], opt);
}


NSVGimage* g_image = NULL;

static unsigned char bgColor[4] = {205,202,200,255};
static unsigned char lineColor[4] = {0,160,192,255};

static float distPtSeg(float x, float y, float px, float py, float qx, float qy) {
    float dx = qx - px, dy = qy - py;
    float t = ((x - px) * dx + (y - py) * dy) / (dx * dx + dy * dy);
    if (t < 0) t = 0; else if (t > 1) t = 1;
    dx = px + t * dx - x;
    dy = py + t * dy - y;
    return dx * dx + dy * dy;
}

array_(vec3) strip;

static void cubicBez(float x1, float y1, float x2, float y2,
                     float x3, float y3, float x4, float y4,
                     float tol, int level) {
    if (level > 12) return;
    float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
    float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
    float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
    float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
    float d = distPtSeg(x1234, y1234, x1, y1, x4, y4);
    if (d > tol * tol) {
        cubicBez(x1, y1, x12, y12, x123, y123, x1234, y1234, tol, level + 1);
        cubicBez(x1234, y1234, x234, y234, x34, y34, x4, y4, tol, level + 1);
    } else {
        array_push(strip, vec3(x1, -y1)); // Add start
        array_push(strip, vec3(x4, -y4)); // Add end
    }
}

void drawPath(float* pts, int npts, char closed, float tol)
{
    unsigned color = *(unsigned *)lineColor;
    array_clear(strip);
    array_push(strip, vec3(pts[0], -pts[1]));
    for ( int i = 0; i < npts-1; i += 3) {
        float* p = &pts[i*2];
        cubicBez(p[0],p[1], p[2],p[3], p[4],p[5], p[6],p[7], tol, 0);
    }
    if (closed) {
        array_push(strip, vec3(pts[0], -pts[1]));
    }
    dd_strip(array_count(strip), strip, .color = color);
}

void drawControlPts(float* pts, int npts)
{
    unsigned color = *(unsigned *)lineColor;
    unsigned bgcolor = *(unsigned *)bgColor;

    // Control lines
    for (int i = 0; i < npts-1; i += 3) {
        float* p = &pts[i*2];
        dd_line(vec3(p[0],-p[1]), vec3(p[2],-p[3]), .color = color);
        dd_line(vec3(p[4],-p[5]), vec3(p[6],-p[7]), .color = color);
    }

    // Points
    dd_point(vec3(pts[0],-pts[1]), .color = color, .thin = 0);
    dd_point(vec3(pts[0],-pts[1]), .color = bgcolor, .thin = 1);
    for (int i = 0; i < npts-1; i += 3) {
        float* p = &pts[i*2];
        dd_point(vec3(p[2],-p[3]), .color = color, .thin = 1);
        dd_point(vec3(p[4],-p[5]), .color = color, .thin = 1);
        dd_point(vec3(p[6],-p[7]), .color = color, .thin = 0);
        dd_point(vec3(p[6],-p[7]), .color = bgcolor, .thin = 1);
    }
}

void svg_draw(int drawControlPoints)
{
    int width = app_width(), height = app_height();
    float view[4], cx, cy, hw, hh, aspect, px;

    // Fit view to bounds
    cx = g_image->width*0.5f;
    cy = g_image->height*0.5f;
    hw = g_image->width*0.5f;
    hh = g_image->height*0.5f;

    if (width/hw < height/hh) {
        aspect = (float)height / (float)width;
        view[0] = cx - hw * 1.2f;
        view[2] = cx + hw * 1.2f;
        view[1] = cy - hw * 1.2f * aspect;
        view[3] = cy + hw * 1.2f * aspect;
    } else {
        aspect = (float)width / (float)height;
        view[0] = cx - hh * 1.2f * aspect;
        view[2] = cx + hh * 1.2f * aspect;
        view[1] = cy - hh * 1.2f;
        view[3] = cy + hh * 1.2f;
    }
    // Size of one pixel.
    px = (view[2] - view[1]) / (float)width;

    // Set tolerance.
    float tolerance = px * 1.5;

    // Draw bounds
    dd_line(vec3(), vec3(hw*2, 0), .color = 0);
    dd_line(vec3(hw*2, 0), vec3(hw*2, -hh*2), .color = 0);
    dd_line(vec3(hw*2, -hh*2), vec3(0, -hh*2), .color = 0);
    dd_line(vec3(0, -hh*2), vec3(), .color = 0);

    for (NSVGshape* shape = g_image->shapes; shape; shape = shape->next) {
        for (NSVGpath* path = shape->paths; path; path = path->next) {
            drawPath(path->pts, path->npts, path->closed, tolerance);
            if( drawControlPoints )
            drawControlPts(path->pts, path->npts);
        }
    }
}

int main() {
    const char *svg = argc() > 1 && strcmpi_(argv0(), "v3") ? argv(1) : "nanosvg/nano.svg";

    printf("Loading `%s`...\n", svg);
    char *bin = file(svg).str;
    if(!bin) die("cannot load .svg file");

    g_image = nsvgParse(bin, "px", 96.0f);
    if(!g_image) die("Could not open SVG image.");

    camera_t cam = { .pos=vec3(12,0,30), .y=-90,.p=-20 };

    app_create(.title = "v3(nanosvg)", .scale = 0.75);

    while (app_swap(.color = 0xFFDCDCDC)) {
        // ui
        ui_label(va("%5.2f/%5.2f fps, %5.2f ms/f", fps(), hz(), spf()*1e3));

        static int drawControlPoints = 1;
        ui_bool("Draw Control Points", &drawControlPoints);

        if( ui_tree("Camera") ) {
            ui_vec3("Yaw,Pitch,Roll", &cam.ypr.x, -180, 180);
            ui_vec3("Position", &cam.pos.x, -1000, 1000);
            ui_tree_end();
        }

        // fps camera
        vec3 offset = vec3(key('d')-key('a'),key('e')-key('q')-key('c'),key('w')-key('s'));
        camera_offset(&cam, offset, key(' ') ? 1.5 : 0.5);

        vec3 turn = vec3(mouse('u'), -mouse('v'), 0);
        camera_turn(&cam, turn, mouse('l') * !ui_hovered() * (key(' ') ? 1.5 : 0.5));

        // draw shape
        svg_draw(drawControlPoints);
        dd_flush(proj44(45), view44(cam));
    }

    nsvgDelete(g_image);

    return 0;
}

#endif
