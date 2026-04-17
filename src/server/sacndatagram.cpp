#include "sacndatagram.h"

SacnDatagram::SacnDatagram(QNetworkDatagram datagram) {
    data = datagram.data();
    sourceAddress = datagram.senderAddress();
}

bool SacnDatagram::isValid() {
    return ((data.size() >= 126)
        && (data.size() <= 638)
        // ROOT LAYER
        // Preamble Size
        && (data[0] == (char)0x00)
        && (data[1] == (char)0x10)
        // Post-amble Size
        && (data[2] == (char)0x00)
        && (data[3] == (char)0x00)
        // ACN Packet Identifier
        && (data[4] == (char)0x41)
        && (data[5] == (char)0x53)
        && (data[6] == (char)0x43)
        && (data[7] == (char)0x2d)
        && (data[8] == (char)0x45)
        && (data[9] == (char)0x31)
        && (data[10] == (char)0x2e)
        && (data[11] == (char)0x31)
        && (data[12] == (char)0x37)
        && (data[13] == (char)0x00)
        && (data[14] == (char)0x00)
        && (data[15] == (char)0x00)
        // Flags and Length
        && checkFlagsAndLength(16)
        // Vector
        && (data[18] == (char)0x00)
        && (data[19] == (char)0x00)
        && (data[20] == (char)0x00)
        && (data[21] == (char)0x04)
        // FRAMING LAYER
        // Flags and Length
        && checkFlagsAndLength(38)
        // Vector
        && (data[40] == (char)0x00)
        && (data[41] == (char)0x00)
        && (data[42] == (char)0x00)
        && (data[43] == (char)0x02)
        // DMP LAYER
        // Flags and Length
        && checkFlagsAndLength(115)
        // Vector
        && (data[117] == (char)0x02)
        // Address Type & Data Type
        && (data[118] == (char)0xa1)
        // First Property Address
        && (data[119] == (char)0x00)
        && (data[120] == (char)0x00)
        // Address Increment
        && (data[121] == (char)0x00)
        && (data[122] == (char)0x01)
        // Property value count
        && (((256 * data[123]) + data[124]) == (data.length() - 125))
        // START Code
        && (data[125] == (char)0x00)
    );
}

int SacnDatagram::getPriority() {
    if (!isValid()) {
        return 0;
    }
    return data[108];
}

int SacnDatagram::getUniverse() {
    if (!isValid()) {
        return 0;
    }
    return (256 * data[113]) + data[114];
}

QString SacnDatagram::getSource() {
    if (!isValid()) {
        return QString();
    }

    QByteArray sourceName = data.mid(44, 64);
    int nullIndex = sourceName.indexOf(0x00);
    if (nullIndex >= 0) {
        sourceName = sourceName.first(nullIndex);
    }

    return QString("%1 (%2)").arg(QString::fromUtf8(sourceName)).arg(sourceAddress.toString());
}

bool SacnDatagram::checkFlagsAndLength(int index) {
    Q_ASSERT((index + 1) < data.length());

    int receivedFlagsAndLength = 256 * data[index] + data[index + 1];
    int expectedFlagsAndLength = 0x7000 + data.length() - index;

    return receivedFlagsAndLength == expectedFlagsAndLength;
}

uint8_t SacnDatagram::getChannel(int channel) {
    if (!isValid() || (channel < 1) || ((125 + channel) >= data.length())) {
        return 0;
    }

    return data[125 + channel];
}