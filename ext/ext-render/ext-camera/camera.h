#ifndef CAMERA_H
#define CAMERA_H

typedef struct camera_t {
    vec3 pos;
    union {
        vec3 ypr;
        struct {
            union { float y, yaw; };
            union { float p, pitch; };
            union { float r, roll; };
        };
    };
    vec3 fwd, right, up, *lookat;
} camera_t;

void camera_offset(camera_t *cam, vec3 offset, float linear_speed);
void camera_turn(camera_t *cam, vec3 turn_ypr, float angular_speed);

extern camera_t last_cam;

#elif CODE
#pragma once

camera_t last_cam;

void camera_offset(camera_t *cam, vec3 offset, float linear_speed) {
    offset = scale3(offset, linear_speed);

    cam->pos = add3(cam->pos, scale3(cam->right, offset.x));
    cam->pos = add3(cam->pos, scale3(cam->up,    offset.y));
    cam->pos = add3(cam->pos, scale3(cam->fwd,   offset.z));

    last_cam = *cam;
}
void camera_turn(camera_t *cam, vec3 turn_ypr, float angular_speed) {
    turn_ypr = scale3(turn_ypr, angular_speed);

    cam->y += turn_ypr.x;
    cam->p += turn_ypr.y;
    cam->r += turn_ypr.z;
    cam->y = fmod(cam->y, 360);
    cam->p = cam->p > 89 ? 89 : cam->p < -89 ? -89 : cam->p;
    cam->r = fmod(cam->r, 360);

    const float deg2rad = 0.0174532f, y = cam->y * deg2rad, p = cam->p * deg2rad, r = cam->r * deg2rad;
    cam->fwd = norm3(vec3(cos(y) * cos(p), sin(p), sin(y) * cos(p)));
    vec3 up = vec3(0,1,0);
    // calculate right and up dirs
    {
        float cosfa = cosf(r);
        float sinfa = sinf(r);
        vec3 right = cross3(cam->fwd, up);
        cam->right = right;
        float th = dot3(cam->fwd, up);

        cam->up.x = up.x * cosfa + right.x * sinfa + cam->fwd.x * th * (1.0f - cosfa);
        cam->up.y = up.y * cosfa + right.y * sinfa + cam->fwd.y * th * (1.0f - cosfa);
        cam->up.z = up.z * cosfa + right.z * sinfa + cam->fwd.z * th * (1.0f - cosfa);
    }

    last_cam = *cam;
}

#endif
