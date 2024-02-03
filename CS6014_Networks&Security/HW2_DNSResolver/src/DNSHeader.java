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
    private int id_, qr_, opCode_, aa_, tc_, rd_, ra_, z_, rCode_, qdCount_, anCount_, nsCount_, arCount_;
    byte flag1_, flag2_;

    DNSHeader(){

    }
    private DNSHeader(int id, byte flag1, byte flag2, int qr, int opCode, int aa, int tc, int rd, int ra, int z, int rCode, int qdCount, int anCount, int nsCount, int arCount) {
        id_ = id;
        flag1_=flag1;
        flag2_=flag2;
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

        byte flag1 = inputStream.readByte();
        byte flag2 = inputStream.readByte();

        int qr = (flag1 & 0b10000000) >>> 7;
        int opCode = (flag1 & 0b01111000) >>> 3;
        int aa = (flag1 & 0b00000100) >>> 2;
        int tc = (flag1 & 0b00000010) >>> 1;
        int rd = flag1 & 0b00000001;

        int ra = (flag2 & 0b10000000) >>> 7;
        int z = (flag2 & 0b01110000) >>> 4;
        int rCode = flag2 & 0b00001111;

        int qdCount = inputStream.readShort();
        int anCount = inputStream.readShort();
        int nsCount = inputStream.readShort();
        int arCount = inputStream.readShort();

        return new DNSHeader(id, flag1, flag2, qr, opCode, aa, tc, rd, ra, z, rCode, qdCount, anCount, nsCount, arCount);
    }

    static DNSHeader buildHeaderForResponse(DNSMessage request) {
        DNSHeader dnsHeader = request.getHeader_();

        //set qr and anCount to 1 for a response
        dnsHeader.qr_ = 1;
        dnsHeader.anCount_ = 1;

        return dnsHeader;
    }

    void writeBytes(OutputStream os) throws IOException {
        DataOutputStream dos = new DataOutputStream(os);
        dos.writeShort(id_);
        dos.writeByte(flag1_);
        dos.writeByte(flag2_);
        dos.writeShort(qdCount_);
        dos.writeShort(anCount_);
        dos.writeShort(nsCount_);
        dos.writeShort(arCount_);
    }

    public String toString(){
        //Print out everything in a nice way.
        return "DNSHeader {" +
                "\n  transactionID=" + id_ +
                ",\n  flags=" +
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

    public int getId_(){return id_;}
    public void setID_(int id){id_=id;}
    public int getQdCount_() {
        return qdCount_;
    }
    public int getAnCount_() {
        return anCount_;
    }
    public int getNsCount_() {
        return nsCount_;
    }
    public int getArCount_() {
        return arCount_;
    }
}
