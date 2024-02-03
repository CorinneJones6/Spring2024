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

    DNSHeader() {

    }

    private DNSHeader(int id, byte flag1, byte flag2, int qr, int opCode, int aa, int tc, int rd, int ra, int z, int rCode, int qdCount, int anCount, int nsCount, int arCount) {
        id_ = id;
        flag1_ = flag1;
        flag2_ = flag2;
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

    /**
     * Decodes the DNS header from a ByteArrayInputStream and constructs a DNSHeader object.
     *
     * @param byteInputStream The ByteArrayInputStream containing the DNS header data.
     * @return A DNSHeader object representing the decoded header.
     * @throws IOException If there is an error reading from the input stream.
     */
    static DNSHeader decodeHeader(ByteArrayInputStream byteInputStream) throws IOException {
        DataInputStream dataInputStream = new DataInputStream(byteInputStream);
        int id = dataInputStream.readShort();

        byte flag1 = dataInputStream.readByte(); //store flag1 and flag2 to make write out easier
        byte flag2 = dataInputStream.readByte();

        //bit manipulation with flag1
        int qr = (flag1 & 0b10000000) >>> 7;
        int opCode = (flag1 & 0b01111000) >>> 3;
        int aa = (flag1 & 0b00000100) >>> 2;
        int tc = (flag1 & 0b00000010) >>> 1;
        int rd = flag1 & 0b00000001;

        //bit manipulation with flag2
        int ra = (flag2 & 0b10000000) >>> 7;
        int z = (flag2 & 0b01110000) >>> 4;
        int rCode = flag2 & 0b00001111;

        int qdCount = dataInputStream.readShort();
        int anCount = dataInputStream.readShort();
        int nsCount = dataInputStream.readShort();
        int arCount = dataInputStream.readShort();

        // Create a DNSHeader object with the decoded information
        return new DNSHeader(id, flag1, flag2, qr, opCode, aa, tc, rd, ra, z, rCode, qdCount, anCount, nsCount, arCount);
    }

    /**
     * Builds a DNS header suitable for a DNS response based on the provided DNSMessage object.
     *
     * @param request The DNSMessage object representing the original DNS request.
     * @return A DNSHeader object with fields adjusted for a response.
     */
    static DNSHeader buildHeaderForResponse(DNSMessage request) {
        DNSHeader dnsHeader = request.getHeader_();
        dnsHeader.qr_ = 1; //set to 1 for response
        dnsHeader.anCount_ = 1; //set to 1 for response
        return dnsHeader;
    }

    /**
     * Writes the DNS header data to the specified OutputStream.
     *
     * @param outputStream The OutputStream where the DNS header data will be written.
     * @throws IOException If there is an error while writing to the output stream.
     */
    void writeBytes(OutputStream outputStream) throws IOException {
        DataOutputStream dataOutputStream = new DataOutputStream(outputStream);
        dataOutputStream.writeShort(id_);
        dataOutputStream.writeByte(flag1_);
        dataOutputStream.writeByte(flag2_);
        dataOutputStream.writeShort(qdCount_);
        dataOutputStream.writeShort(anCount_);
        dataOutputStream.writeShort(nsCount_);
        dataOutputStream.writeShort(arCount_);
    }

    /**
     * Returns a string representation of the DNS header object, including its fields and values.
     *
     * @return A string representation of the DNS header object.
     */
    public String toString() {
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

    /**
     * Getter methods for retrieving DNS header member variables.
     */
    public int getId_() {
        return id_;
    }

    public void setID_(int id) {
        id_ = id;
    }

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
