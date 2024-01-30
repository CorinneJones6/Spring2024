import java.io.*;

/*
                                   1  1  1  1  1  1
     0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */

public class DNSHeader {
    private int id_;
    private boolean qr_;
    private int opCode_;
    private boolean aa_;
    private boolean tc_;
    private boolean rd_;
    private boolean ra_;
    private int z_;
    private int rCode_;
    private short qdCount_;
    private short anCount_;
    private short nsCount_;
    private short arCount_;

    DNSHeader(){

    }
    private DNSHeader(int id, boolean qr, int opCode, boolean aa, boolean tc, boolean rd, boolean ra, int z, int rCode, short qdCount, short anCount, short nsCount, short arCount) {
        id_ = id;
        qr_ = qr;
        opCode_ = opCode;
        aa_ = aa;
        tc_ = tc;
        rd_ = rd;
        ra_ = ra;
        z_ = z;
        rCode_ = rCode;
        qdCount_ = qdCount;
        anCount_ = anCount;
        nsCount_ = nsCount;
        arCount_ = arCount;
    }
    static DNSHeader decodeHeader(ByteArrayInputStream is) throws IOException {
        DataInputStream inputStream = new DataInputStream(is);
        int id = inputStream.readShort();

        //========== Pull info from flags ==========//
        short flags = inputStream.readByte();
        boolean qr = ((flags & 0b10000000) >>> 7) == 1;
        int opCode = (flags & 0b01111000) >>> 3;
        boolean aa = ((flags & 0b00000100) >>> 2) == 1;
        boolean tc = ((flags & 0b00000010) >>> 1) == 1;
        boolean rd = (flags & 0b00000001)==1;

        //========== Pull info from flags ==========//
        flags = inputStream.readByte();
        boolean ra = ((flags & 0b10000000) >>> 7) == 1;
        int z = (flags & 0b01110000) >>> 4;
        int rCode = (flags & 0b00001111);

        //========== Read in qdCount, anCount, nsCount, and arCount ==========//
        short qdCount = inputStream.readShort();
        short anCount = inputStream.readShort();
        short nsCount = inputStream.readShort();
        short arCount = inputStream.readShort();

        return new DNSHeader(id, qr, opCode, aa, tc, rd, ra, z, rCode, qdCount, anCount, nsCount, arCount);
    }

    static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response) {
        //todo: implement this function
        return new DNSHeader();
    }
    void writeBytes(OutputStream os){
        //todo: implement this function
    }

    public String toString(){
        return "DNS Header={ " +
                "id: " + id_ +
                "qr: " + qr_ +
                "opCode: " + opCode_ +
                "aa: " + aa_ +
                "tc: " + tc_ +
                "rd: " + rd_ +
                "ra: " + ra_ +
                "z: " + z_ +
                "rCode: " + rCode_ +
                "qdCount: " + qdCount_ +
                "anCount: " + anCount_ +
                "nsCount: " + nsCount_ +
                "arCount_: " + arCount_;
    }

}
