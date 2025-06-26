#pragma once

#include <stdint.h>
/**
 * sample_rate: velocidad de reproduccion (ej: 44100, 32000, etc)
 * 
 * sample_count: cantidad de muestras (en formato float) de la pista de audio
 * 
 * audio: vector que contiene las muestras de audio
 */
int play_audio(uint32_t sample_rate,uint32_t sample_count,float *audio);
