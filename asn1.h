#ifndef _ASN1_H
#define _ASN1_H

#include <stdio.h>
#include <sys/types.h>
#include <RRCConnectionRequest.h>
#include <RRCConnectionSetup.h>
#include <stdbool.h>

static const size_t buf_max_size = 1024;

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *len);
bool RRCConnectionRequestDecoder(RRCConnectionRequest_t* rrc_request, uint8_t *buffer, size_t len); // returns true if successful
void RRCConnectionCompleteCoder(uint8_t **buffer, ssize_t *len);
bool RRCConnectionCompleteDecoder(RRCConnectionSetup_t* rrc_respons, uint8_t *buffer, size_t len); // returns true if successful

int GetRandomNumber(int min, int max);

#endif