#include "asn1.h"
#include <stdlib.h>

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *len)
{
    RRCConnectionRequest_t rrcc_request;
    int ue_id = rand();

    // set RRC connection request
    memset(&rrcc_request, 0, sizeof(rrcc_request));
    rrcc_request.criticalExtensions.present = criticalExtensions_PR_rrcConnectionRequest_r8;
    rrcc_request.criticalExtensions.choice.rrcConnectionRequest_r8.establishmentCause = EstablishmentCause_mt_Access;
    rrcc_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present = InitialUE_Identity_PR_randomValue;
    rrcc_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.buf = (uint8_t*)&ue_id;
    rrcc_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.size = sizeof(ue_id);

    xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, &rrcc_request);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionRequest, &rrcc_request);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        perror("Enconing RRC Connect Request");
        exit(1);
    } else {
        printf("Encoded RRC Connect Request\n");
    }

}

bool RRCConnectionRequestDecoder(RRCConnectionRequest_t *rrc_request, uint8_t *buffer, size_t len)
{
    asn_dec_rval_t rval;
   
    rval = asn_decode(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionRequest, (void **)&rrc_request, (void *)buffer, len);
    
    if (rval.code != RC_OK) 
    {
        return false;
    }

    xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, rrc_request);

    return true; // success
}

void RRCConnectionCompleteCoder(uint8_t **buffer, ssize_t *len)
{
    RRCConnectionSetup_t rrcc_response;
    long srb_Identity = GetRandomNumber(1, 2);
    long rrc_TransactionIdentifier = GetRandomNumber(0, 3);
    long eps_BearerIdentity = GetRandomNumber(0, 15);


    // set RRC connection setup
    memset(&rrcc_response, 0, sizeof(rrcc_response));
    rrcc_response.radioResourceConfigDedicated.eps_BearerIdentity = &eps_BearerIdentity;
    rrcc_response.radioResourceConfigDedicated.srb_Identity = srb_Identity;
    rrcc_response.rrc_TransactionIdentifier = rrc_TransactionIdentifier;

    xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, &rrcc_response);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionSetup, &rrcc_response);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        perror("Enconing RRC Connect Setup");
        exit(1);
    } else {
        printf("RRC Connect Setup\n");
    }
}

bool RRCConnectionCompleteDecoder(RRCConnectionSetup_t *rrc_respons, uint8_t *buffer, size_t len)
{
    asn_dec_rval_t rval;
   
    rval = asn_decode(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionSetup, (void **)&rrc_respons, (void *)buffer, len);
    
    if (rval.code != RC_OK) 
    {
        return false;
    }

    xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, rrc_respons);

    return true; // success
}

int GetRandomNumber(int min, int max)
{
    int randomNumber = rand() % (max - min + 1);
    return randomNumber + min;
}