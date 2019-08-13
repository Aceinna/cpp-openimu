/** ***************************************************************************
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 *  main is the center of the system universe, it all starts here. And never ends.
 * entry point for system (pins, clocks, interrupts), data and task initialization.
 * contains the main processing loop. - this is a standard implementation
 * which has mainly os functionality in the main loop
 ******************************************************************************/
/*******************************************************************************
Copyright 2018 ACEINNA, INC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "cksum.h"
#include "StreamBuffer.hpp"
#include "xassert.h"

error_t XbowPacket::send(IDataStream *stream)
{
    XASSERT(stream);

    // packet format: 0x5555 <pktType U2> <payloadLen U1> <payload> <crc U2>
    uint8_t msg[XBOWPKT_MAX_LEN];

    _dPtr           = msg;
    int length      = 0;
    uint16_t cksum  = 0;

    switch (_protocol) {

      case XBOW_PROTOCOL:
        htonb_16(_protocol);
        htonb_16(_pktType);
        htonb_8(_payloadLen);
        serialize(NULL);
        cksum = crc16(msg+2, _payloadLen+3);
        htonb_16(cksum);
        length = _payloadLen + 7;
        break;
      default:
          XASSERT(0);
          break;
    }
    
    // Write out the complete message, which is the length of the payload, plus the length of
    // the header, the packet type, (XBOW: the payload length), and the crc
    return stream->write(msg, length);

}
