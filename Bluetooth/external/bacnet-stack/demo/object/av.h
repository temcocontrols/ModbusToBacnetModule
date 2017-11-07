/**************************************************************************
*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
* Copyright (C) 2011 Krzysztof Malorny <malornykrzysztof@gmail.com>
* Copyright (C) 2013 Patrick Grimm <patrick@lunatiki.de>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#ifndef AV_H
#define AV_H

#include <stdbool.h>
#include <stdint.h>
#include "bacdef.h"
#include "cov.h"
#include "bacerror.h"
#include "wp.h"
#include "rp.h"
#if defined(INTRINSIC_REPORTING)
#include "nc.h"
#include "alarm_ack.h"
#include "getevent.h"
#include "get_alarm_sum.h"
#endif /* INTRINSIC_REPORTING */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    typedef struct analog_value_descr {
        uint32_t Instance;
        char Object_Name[64];
        char Object_Description[64];
        unsigned Event_State:3;
        //float Present_Value;
        BACNET_RELIABILITY Reliability;
        bool Out_Of_Service;
        bool Change_Of_Value;
        uint8_t Units;
        float Prior_Value;
        float COV_Increment;
        bool Changed;
        bool Disable;
        /* Here is our Priority Array.  They are supposed to be Real, but */
        /* we don't have that kind of memory, so we will use a single byte */
        /* and load a Real for returning the value when asked. */
        float Priority_Array[BACNET_MAX_PRIORITY];
        float Relinquish_Default;
#if defined(INTRINSIC_REPORTING)
        uint32_t Time_Delay;
        uint32_t Notification_Class;
        float High_Limit;
        float Low_Limit;
        float Deadband;
        unsigned Limit_Enable:2;
        unsigned Event_Enable:3;
        unsigned Notify_Type:1;
        ACKED_INFO Acked_Transitions[MAX_BACNET_EVENT_TRANSITION];
        BACNET_DATE_TIME Event_Time_Stamps[MAX_BACNET_EVENT_TRANSITION];
        /* time to generate event notification */
        uint32_t Remaining_Time_Delay;
        /* AckNotification informations */
        ACK_NOTIFICATION Ack_notify_data;
#endif /* INTRINSIC_REPORTING */
    } ANALOG_VALUE_DESCR;

/* value/name tuples */
struct av_inst_tuple {
	char idx[18];
	struct av_inst_tuple *next;
};

typedef struct av_inst_tuple av_inst_tuple_t;

/* structure to hold tuple-list and uci context during iteration */
struct av_inst_itr_ctx {
	struct av_inst_tuple *list;
	struct uci_context *ctx;
	char *section;
};


	void Analog_Value_Load_UCI_List(
		const char *sec_idx,
		struct av_inst_itr_ctx *itr);

    void Analog_Value_Property_Lists(
        const int **pRequired,
        const int **pOptional,
        const int **pProprietary);

    bool Analog_Value_Valid_Instance(
        uint32_t object_instance);

    unsigned Analog_Value_Count(
        void);

    uint32_t Analog_Value_Index_To_Instance(
        unsigned index);

    unsigned Analog_Value_Instance_To_Index(
        uint32_t object_instance);

    int Analog_Value_Read_Property(
        BACNET_READ_PROPERTY_DATA * rpdata);

    bool Analog_Value_Write_Property(
        BACNET_WRITE_PROPERTY_DATA * wp_data);

    /* optional API */
    bool Analog_Value_Object_Instance_Add(
        uint32_t instance);

    bool Analog_Value_Object_Name(
        uint32_t object_instance,
        BACNET_CHARACTER_STRING * object_name);

    bool Analog_Value_Name_Set(
        uint32_t object_instance,
        char *new_name);

    bool Analog_Value_Present_Value_Set(
        uint32_t object_instance,
        float value,
        uint8_t priority);

    float Analog_Value_Present_Value(
        uint32_t object_instance);

    unsigned Analog_Value_Present_Value_Priority(
        uint32_t object_instance);

    bool Analog_Value_Out_Of_Service(
        uint32_t object_instance);

    void Analog_Value_Out_Of_Service_Set(
        uint32_t object_instance,
        bool value);

    uint8_t Analog_Value_Reliability(
        uint32_t object_instance);

    void Analog_Value_Reliability_Set(
        uint32_t object_instance,
        uint8_t value);

    bool Analog_Value_Encode_Value_List(
        uint32_t object_instance,
        BACNET_PROPERTY_VALUE * value_list);

    bool Analog_Value_Change_Of_Value(
        uint32_t instance);
    void Analog_Value_Change_Of_Value_Clear(
        uint32_t instance);

    char *Analog_Value_Description(
        uint32_t instance);
    bool Analog_Value_Description_Set(
        uint32_t object_instance,
        char *text_string);

    /* note: header of Intrinsic_Reporting function is required
       even when INTRINSIC_REPORTING is not defined */
    void Analog_Value_Intrinsic_Reporting(
        uint32_t object_instance);

#if defined(INTRINSIC_REPORTING)
    int Analog_Value_Event_Information(
        unsigned index,
        BACNET_GET_EVENT_INFORMATION_DATA * getevent_data);

    int Analog_Value_Alarm_Ack(
        BACNET_ALARM_ACK_DATA * alarmack_data,
        BACNET_ERROR_CODE * error_code);

    int Analog_Value_Alarm_Summary(
        unsigned index,
        BACNET_GET_ALARM_SUMMARY_DATA * getalarm_data);
#endif

    bool Analog_Value_Create(
        uint32_t object_instance);
    bool Analog_Value_Delete(
        uint32_t object_instance);
    void Analog_Value_Cleanup(
        void);
    void Analog_Value_Init(
        void);

#ifdef TEST
#include "ctest.h"
    void testAnalog_Value(
        Test * pTest);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
