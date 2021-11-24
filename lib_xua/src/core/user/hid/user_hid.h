// Copyright 2013-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

/**
 * @brief Human Interface Device (HID) API
 *
 * This file defines the Application Programming Interface (API) used to record HID
 *   events and retrieve a HID Report for sending to a host.
 * The using application has the responsibility to fulfill this API.
 * Document section numbers refer to the HID Device Class Definition, version 1.11.
 */

#ifndef __USER_HID_H__
#define __USER_HID_H__

/**
 *  \brief  HID event
 *
 *  This struct identifies:
 *  - the HID Report that reports an event,
 *  - the location within the HID Report for the event, and
 *  - the value to report for that location (typically interpreted as a Boolean).
 *  It assumes only single bit flags within the HID Report.
 */
typedef struct hidEvent_t {
  unsigned bit;
  unsigned byte;
  unsigned id;
  unsigned value;
} hidEvent_t;

#define HID_MAX_DATA_BYTES 4

#if( 0 < HID_CONTROLS )

/**
 *  \brief  Get the data for the next HID Report
 *
 *  \note This function returns the HID data as a list of unsigned char because the
 *        \c XUD_SetReady_In() accepts data for transmission to the USB Host using
 *        this type.
 *
 *  \param[in]  id       The HID Report ID (see 5.6, 6.2.2.7, 8.1 and 8.2)
 *  \param[out] hidData  The HID data
 */
void UserHIDGetData( const unsigned id, unsigned char hidData[ HID_MAX_DATA_BYTES ]);

/**
 *  \brief  Get the upper limit of HID Report identifiers
 *
 *  This function returns the upper limit of the HID Report identifiers.
 *  The upper limit has a value one greater than the maximum HID Report identifier.
 *  In the case that HID Report identifiers are not in use, this function returns the value 1.
 *
 *  \returns  The upper limit of HID Report identifiers
 */
unsigned UserHIDGetReportIdLimit ( void );

/**
 *	\brief  Initialize HID processing
 */
void UserHIDInit( void );

/**
 *  \brief  Record that a HID event has occurred
 *
 *  \param[in]  hidEvent      A list of events which have occurred.
 *                            Each element specifies a HID Report ID, a bit and byte
 *                              within the HID Report and the value for it.
 *  \param[in]  hidEventCnt   The length of the \a hidEvent list.
 *
 *  \returns  A Boolean flag indicating the status of the operation
 *  \retval   False No recording of the event(s) occurred
 *  \retval   True  Recording of the event(s) occurred
 */
unsigned UserHIDRecordEvent( const hidEvent_t hidEvent[], const unsigned hidEventCnt );

/**
 *  \brief  Indicate if a HID Report ID has new data to report
 *
 *  \param[in]  id  A HID Report ID (see 5.6, 6.2.2.7, 8.1 and 8.2)
 *
 *  \returns  A Boolean flag indicating the status of the operation
 *  \retval   False The given \a id either does not have new data to report or lies
 *              outside the range of supported HID Report identifiers
 *  \retval   True  The given \a id has new data to report to the USB Host
 */
unsigned UserHIDReportChanged( const unsigned id );

#endif /* ( 0 < HID_CONTROLS ) */
#endif /* __USER_HID_H__ */
