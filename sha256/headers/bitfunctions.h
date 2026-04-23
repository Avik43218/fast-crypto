#pragma once

#ifndef BITFUNCTIONS_H
#define BITFUNCTIONS_H

#include <cstdint>

uint32_t rotr(uint32_t x, uint32_t n);
uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t maj(uint32_t x, uint32_t y, uint32_t z);

uint32_t Σ0(uint32_t x);
uint32_t Σ1(uint32_t x);
uint32_t σ0(uint32_t x);
uint32_t σ1(uint32_t x);

#endif
