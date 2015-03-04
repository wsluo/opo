/**
* This file is autogenerated by nRFgo Studio 1.17.0.3211
*/

#ifndef SETUP_MESSAGES_H__
#define SETUP_MESSAGES_H__

#define SETUP_ID 0
#define SETUP_FORMAT 3 /** nRF8001 D */
#define ACI_DYNAMIC_DATA_SIZE 257

/* Service: Gap - Characteristic: Device name - Pipe: SET */
#define PIPE_GAP_DEVICE_NAME_SET          1
#define PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE 9

/* Service: CloudComm - Characteristic: CloudCommData - Pipe: TX */
#define PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX          2
#define PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommData - Pipe: TX_ACK */
#define PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK          3
#define PIPE_CLOUDCOMM_CLOUDCOMMDATA_TX_ACK_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommMetaData - Pipe: TX */
#define PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX          4
#define PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommMetaData - Pipe: TX_ACK */
#define PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX_ACK          5
#define PIPE_CLOUDCOMM_CLOUDCOMMMETADATA_TX_ACK_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommRequests - Pipe: TX */
#define PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX          6
#define PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommRequests - Pipe: TX_ACK */
#define PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK          7
#define PIPE_CLOUDCOMM_CLOUDCOMMREQUESTS_TX_ACK_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommIncomingData - Pipe: RX_ACK_AUTO */
#define PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO          8
#define PIPE_CLOUDCOMM_CLOUDCOMMINCOMINGDATA_RX_ACK_AUTO_MAX_SIZE 20

/* Service: CloudComm - Characteristic: CloudCommReady - Pipe: RX_ACK_AUTO */
#define PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO          9
#define PIPE_CLOUDCOMM_CLOUDCOMMREADY_RX_ACK_AUTO_MAX_SIZE 1

/* Service: CloudComm - Characteristic: CloudCommAcks - Pipe: RX_ACK_AUTO */
#define PIPE_CLOUDCOMM_CLOUDCOMMACKS_RX_ACK_AUTO          10
#define PIPE_CLOUDCOMM_CLOUDCOMMACKS_RX_ACK_AUTO_MAX_SIZE 20


#define NUMBER_OF_PIPES 10

#define SERVICES_PIPE_TYPE_MAPPING_CONTENT {\
  {ACI_STORE_LOCAL, ACI_SET},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX_ACK},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX_ACK},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX_ACK},   \
  {ACI_STORE_LOCAL, ACI_RX_ACK_AUTO},   \
  {ACI_STORE_LOCAL, ACI_RX_ACK_AUTO},   \
  {ACI_STORE_LOCAL, ACI_RX_ACK_AUTO},   \
}

#define GAP_PPCP_MAX_CONN_INT 0xc80 /**< Maximum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_MIN_CONN_INT  0x320 /**< Minimum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_SLAVE_LATENCY 60
#define GAP_PPCP_CONN_TIMEOUT 0xffff /** Connection Supervision timeout multiplier as a multiple of 10msec, 0xFFFF means no specific value requested */

#define NB_SETUP_MESSAGES 25
#define SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x42,0x07,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x0a,0x04,0x06,0x00,0x00,0x07,0x00,0x01,\
            0xc1,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x03,0x90,0x00,0xff,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0xff,0xff,0x02,0x58,0x0a,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x0e,0x03,0x00,0x00,0x2a,0x04,0x14,0x09,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x09,0x00,0x03,0x2a,0x00,0x01,0x43,0x6c,0x6f,0x75,0x64,0x63,0x6f,0x6d,0x6d,0x04,\
            0x04,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,0x05,0x00,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x2a,0x06,0x04,0x03,0x02,0x00,0x05,0x2a,0x01,0x01,0x00,0x00,0x04,0x04,0x05,0x05,\
            0x00,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,0x2a,0x06,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x09,0x08,0x00,0x07,0x2a,0x04,0x01,0x20,0x03,0x80,0x0c,0x3c,0x00,0xff,0xff,0x04,\
            0x04,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,0x18,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x02,0x02,0x00,0x09,0x28,0x00,0x01,0x00,0x20,0x04,0x04,0x05,0x05,0x00,0x0a,0x28,\
            0x03,0x01,0x32,0x0b,0x00,0x00,0x2b,0x34,0x04,0x14,0x01,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x0b,0x2b,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x14,0x03,0x02,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x00,0x0c,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x0d,0x28,0x03,0x01,\
            0x32,0x0e,0x00,0x01,0x2b,0x34,0x04,0x14,0x01,0x00,0x0e,0x2b,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xc4,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x0f,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xe0,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x10,0x28,0x03,0x01,0x32,0x11,\
            0x00,0x02,0x2b,0x34,0x04,0x14,0x01,0x00,0x11,0x2b,0x02,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xfc,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x12,0x29,0x02,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x18,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x13,0x28,0x03,0x01,0x0a,0x14,0x00,0x03,\
            0x2b,0x44,0x14,0x14,0x01,0x00,0x14,0x2b,0x03,0x01,0x01,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x15,0x28,0x03,0x01,0x0a,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x50,0x16,0x00,0x04,0x2b,0x46,0x14,0x02,0x01,0x00,0x16,0x2b,0x04,0x01,0x00,0x06,0x04,\
            0x08,0x07,0x00,0x17,0x29,0x04,0x01,0x04,0x00,0x00,0x00,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x6c,0x00,0x00,0x04,0x04,0x05,0x05,0x00,0x18,0x28,0x03,0x01,0x08,0x19,0x00,0x05,0x2b,\
            0x46,0x10,0x15,0x14,0x00,0x19,0x2b,0x05,0x01,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x15,0x06,0x21,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x00,0x2a,0x00,0x01,0x00,0x80,0x04,0x00,0x03,0x00,0x00,0x2b,0x00,0x01,0x00,0x06,0x04,\
            0x00,0x0b,0x00,0x0c,0x2b,0x01,0x01,0x00,0x06,0x04,0x00,0x0e,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x1c,0x00,0x0f,0x2b,0x02,0x01,0x00,0x06,0x04,0x00,0x11,0x00,0x12,0x2b,0x03,0x01,0x04,\
            0x00,0x04,0x00,0x14,0x00,0x00,0x2b,0x04,0x01,0x04,0x00,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x11,0x06,0x40,0x38,0x00,0x16,0x00,0x00,0x2b,0x05,0x01,0x04,0x00,0x04,0x00,0x19,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x18,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x03,0x3d,0x56,\
        },\
    },\
}

#endif