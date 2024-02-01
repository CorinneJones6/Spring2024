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
    private short id_;
    private short flags_;
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
    private DNSHeader(short id, short flags, boolean qr, int opCode, boolean aa, boolean tc, boolean rd, boolean ra, int z, int rCode, short qdCount, short anCount, short nsCount, short arCount) {
        id_ = id;
        flags_=flags;
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
        short id = inputStream.readShort();


        //========== Pull info from flags ==========//
        byte flag1 = inputStream.readByte();
        boolean qr = ((flag1 & 0b10000000) >>> 7) == 1;
        int opCode = (flag1 & 0b01111000) >>> 3;
        boolean aa = ((flag1 & 0b00000100) >>> 2) == 1;
        boolean tc = ((flag1 & 0b00000010) >>> 1) == 1;
        boolean rd = (flag1 & 0b00000001) == 1;

        //========== Pull info from flags ==========//
        byte flag2 = inputStream.readByte();
        boolean ra = ((flag2 & 0b10000000) >>> 7) == 1;
        int z = (flag2 & 0b01110000) >>> 4;
        int rCode = (flag2 & 0b00001111);

        //========== Combine flag 1 and flag 2 into one short flags ==========//
        short flags = (short)((flag1 << 8) | (flag2 & (0b11111111)));

        //========== Read in qdCount, anCount, nsCount, and arCount ==========//
        short qdCount = inputStream.readShort();
        short anCount = inputStream.readShort();
        short nsCount = inputStream.readShort();
        short arCount = inputStream.readShort();

        return new DNSHeader(id, flags, qr, opCode, aa, tc, rd, ra, z, rCode, qdCount, anCount, nsCount, arCount);
    }

    static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response) {
        DNSHeader dnsHeader = request.getHeader_();

        //set qr and anCount to 1 for a response
        dnsHeader.qr_ = true;
        dnsHeader.anCount_ = 1;

        return dnsHeader;
    }

    void writeBytes(OutputStream os) throws IOException {
        DataOutputStream dos = new DataOutputStream(os);
        dos.writeShort(id_);
        dos.writeShort(flags_);
        dos.writeShort(qdCount_);
        dos.writeShort(anCount_);
        dos.writeShort(nsCount_);
        dos.writeShort(arCount_);
    }

    public String toString(){
        //Print out everything in a nice way.
        return "DNSHeader {" +
                "\n  transactionID=" + id_ +
                ",\n  flags=" + flags_ +
                " (qr=" + qr_ +
                ", opCode=" + opCode_ +
                ", aa=" + aa_ +
                ", tc=" + tc_ +
                ", rd=" + rd_ +
                ", ra=" + ra_ +
                ", z=" + z_ +
                ", rCode=" + rCode_ + ")" +
                ",\n  questions=" + qdCount_ +
                ",\n  answerRRs=" + anCount_ +
                ",\n  authorityRRs=" + nsCount_ +
                ",\n  additionalRRs=" + arCount_ +
                "\n}";
    }

    public short getQdCount_() {
        return qdCount_;
    }
    public short getAnCount_() {
        return anCount_;
    }
    public short getNsCount_() {
        return nsCount_;
    }
    public short getArCount_() {
        return arCount_;
    }
}
