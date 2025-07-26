#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct audio_type {
    int channels;        /* 0 invalid, 1 mono, 2 stereo, etc. */
    int bits;            /* 16 for short, 32 for int, -32 for float */
    int frequency;       /* in Hz, typically 11025, 22050, 44100 */
    int count;           /* num_samples (optional) */
    void *data;          /* samples (optional) */
} audio_type;

/* Functor type for resampling */
typedef void (*resampler_t)(const float *in_samples, int in_num_samples, int in_channels,
                           float *out_samples, int out_num_samples, double ratio);

/* Default linear interpolation resampler */
static void default_resampler(const float *in_samples, int in_num_samples, int in_channels,
                             float *out_samples, int out_num_samples, double ratio) {
    for (int i = 0; i < out_num_samples; i++) {
        double input_pos = (double)i / ratio;
        int pos1 = (int)floor(input_pos);
        double frac = input_pos - pos1;
        int pos2 = pos1 + 1;

        if (pos1 >= in_num_samples - 1) {
            pos1 = in_num_samples - 1;
            pos2 = pos1;
            frac = 0.0;
        } else if (pos2 >= in_num_samples) {
            pos2 = in_num_samples - 1;
        }

        for (int ch = 0; ch < in_channels; ch++) {
            float sample1 = in_samples[pos1 * in_channels + ch];
            float sample2 = in_samples[pos2 * in_channels + ch];
            out_samples[i * in_channels + ch] = sample1 * (1.0f - (float)frac) + sample2 * (float)frac;
        }
    }
}

static void read_frame_to_float(void *in_base, int bits, int channels, float *samples) {
    if (bits == 16) {
        short *ptr = (short *)in_base;
        for (int ch = 0; ch < channels; ch++) {
            samples[ch] = ptr[ch] / 32768.0f;
        }
    } else if (bits == 32) {
        int *ptr = (int *)in_base;
        for (int ch = 0; ch < channels; ch++) {
            samples[ch] = ptr[ch] / 2147483648.0f;
        }
    } else { /* -32 */
        float *ptr = (float *)in_base;
        for (int ch = 0; ch < channels; ch++) {
            samples[ch] = ptr[ch];
        }
    }
}

static float get_mixed_sample(float *samples, int in_channels, int out_channels, int ch) {
    if (in_channels == out_channels) {
        return samples[ch];
    } else if (out_channels == 1) { /* Stereo to mono: average */
        return (samples[0] + samples[1]) / 2.0f;
    } else { /* Mono to stereo: duplicate */
        return samples[0];
    }
}

bool audio_convert(void **data, int num_samples, audio_type in, audio_type out, resampler_t resampler) {
    if (!data || !*data || num_samples <= 0 || in.channels < 1 || out.channels < 1 ||
        in.channels > 2 || out.channels > 2 ||
        (in.bits != 16 && in.bits != 32 && in.bits != -32) ||
        (out.bits != 16 && out.bits != 32 && out.bits != -32) ||
        (in.frequency != 11025 && in.frequency != 22050 && in.frequency != 44100) ||
        (out.frequency != 11025 && out.frequency != 22050 && out.frequency != 44100) ||
        in.frequency <= 0 || out.frequency <= 0) {
        return false;
    }

    if (!resampler) resampler = default_resampler;

    double ratio = (double)out.frequency / in.frequency;
    long long new_num_samples_ll = (long long)round((double)num_samples * ratio);
    if (new_num_samples_ll <= 0 || new_num_samples_ll > INT_MAX) return false;
    int new_num_samples = (int)new_num_samples_ll;

    size_t out_sample_size = (out.bits == 16 ? sizeof(short) : sizeof(int));
    size_t out_frame_size = out.channels * out_sample_size;
    size_t new_size = (size_t)new_num_samples * out_frame_size;
    if (new_size / out_frame_size != (size_t)new_num_samples) return false; /* Overflow check */

    void *new_data = malloc(new_size);
    if (!new_data) return false;

    void *input = *data;
    size_t in_sample_size = (in.bits == 16 ? sizeof(short) : sizeof(int));
    size_t in_frame_size = in.channels * in_sample_size;

    /* Convert input to float array for resampling */
    float *float_samples = malloc((size_t)num_samples * in.channels * sizeof(float));
    if (!float_samples) {
        free(new_data);
        return false;
    }

    for (int i = 0; i < num_samples; i++) {
        char *in_base = (char *)input + (size_t)i * in_frame_size;
        read_frame_to_float(in_base, in.bits, in.channels, float_samples + i * in.channels);
    }

    /* Resample */
    float *resampled_samples = malloc((size_t)new_num_samples * in.channels * sizeof(float));
    if (!resampled_samples) {
        free(float_samples);
        free(new_data);
        return false;
    }
    resampler(float_samples, num_samples, in.channels, resampled_samples, new_num_samples, ratio);
    free(float_samples);

    /* Convert to output format and channels */
    for (int i = 0; i < new_num_samples; i++) {
        char *out_base = (char *)new_data + (size_t)i * out_frame_size;
        if (out.bits == 16) {
            short *out_ptr = (short *)out_base;
            for (int ch = 0; ch < out.channels; ch++) {
                float sample = get_mixed_sample(resampled_samples + i * in.channels, in.channels, out.channels, ch);
                out_ptr[ch] = (short)(sample * 32767.0f + (sample >= 0 ? 0.5f : -0.5f));
            }
        } else if (out.bits == 32) {
            int *out_ptr = (int *)out_base;
            for (int ch = 0; ch < out.channels; ch++) {
                float sample = get_mixed_sample(resampled_samples + i * in.channels, in.channels, out.channels, ch);
                out_ptr[ch] = (int)(sample * 2147483647.0f + (sample >= 0 ? 0.5f : -0.5f));
            }
        } else { /* -32 */
            float *out_ptr = (float *)out_base;
            for (int ch = 0; ch < out.channels; ch++) {
                out_ptr[ch] = get_mixed_sample(resampled_samples + i * in.channels, in.channels, out.channels, ch);
            }
        }
    }

    free(resampled_samples);
    free(input);
    *data = new_data;
    return true;
}
