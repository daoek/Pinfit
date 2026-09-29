/*@Pinfit(file:status-codes:pinfit_status.status-codes.yaml)*/
/*@Pinfit(skeleton-hash:57a2afeb034b9c67)*/
/**
 * @file pinfit_status.h
 * @brief Shared status codes
 */

#ifndef PINFIT_STATUS_H_
#define PINFIT_STATUS_H_

/*@Pinfit usercode+ status-codes.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(enum:pinfit_status)*/
/** @brief Shared status codes */
typedef enum
{
    PINFIT_STATUS_OK = 0,
    PINFIT_STATUS_INVALID_PARAM = -1,
    PINFIT_STATUS_NOT_READY = -2
} pinfit_status_t;

/*@Pinfit(macro:PINFIT_STATUS_SUCCEEDED)*/
#define PINFIT_STATUS_SUCCEEDED(status) ((status) == PINFIT_STATUS_OK)

/*@Pinfit(macro:PINFIT_STATUS_FAILED)*/
#define PINFIT_STATUS_FAILED(status) (!PINFIT_STATUS_SUCCEEDED(status))

/*@Pinfit(macro:PINFIT_STATUS_CHECK)*/
#define PINFIT_STATUS_CHECK(status_expression) \
    do { pinfit_status_t pinfit_status = (status_expression); \
        if (PINFIT_STATUS_FAILED(pinfit_status)) { return pinfit_status; } \
    } while (0)

/*@Pinfit usercode+ status-codes.footer*/
/*@Pinfit usercode-*/

#endif /* PINFIT_STATUS_H_ */
