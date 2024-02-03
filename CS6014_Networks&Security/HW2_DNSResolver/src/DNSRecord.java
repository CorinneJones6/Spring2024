import java.io.*;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;

/*    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
public class DNSRecord {

    String[] name_;
    int type_, class_, ttl_, rdLength_;
    private byte[] rdata_;
    Date createdDate_;

    DNSRecord() {

    }

    DNSRecord(String[] name, int type, int c_, int ttl, int rdLength, byte[] rdata, Date date) {
        name_ = name;
        type_ = type;
        class_ = c_;
        ttl_ = ttl;
        rdLength_ = rdLength;
        rdata_ = rdata;
        createdDate_ = date;
    }

    /**
     * Decodes a DNS record from the provided InputStream and constructs a DNSRecord object.
     *
     * @param inputStream The InputStream containing the DNS record data.
     * @param dnsMessage  The DNSMessage object to which the record belongs, used for domain name decoding.
     * @return A DNSRecord object representing the decoded DNS record.
     * @throws IOException If there inputStream an error during the decoding process.
     */
    static DNSRecord decodeRecord(InputStream inputStream, DNSMessage dnsMessage) throws IOException {
        DataInputStream dataInputStream = new DataInputStream(inputStream);
        String[] name;

        // Mark the current position in the input stream for potential reset
        dataInputStream.mark(2);

        // Read the first two bytes to determine if it's a compressed or full domain name
        short firstTwoBytes = dataInputStream.readByte();

        if ((firstTwoBytes & 0xC0) == 0xC0) {
            // Handle compressed domain name with offset
            int offset = dataInputStream.readByte();
//            int offset = firstTwoBytes & 0x3FFF;
            name = dnsMessage.readDomainName(offset);

        } else {
            // Reset the input stream to the marked position and read the full domain name
            dataInputStream.reset();
            name = dnsMessage.readDomainName(dataInputStream);
        }

        // Read and parse the record type, class, time-to-live (TTL), and record data length
        if(name.length!=0) {
            int type = dataInputStream.readShort();
            int c_ = dataInputStream.readShort();
            int ttl = dataInputStream.readInt();
            int rdLength = dataInputStream.readShort();

            // Read the raw record data
            byte[] rdata = new byte[rdLength];
            dataInputStream.readFully(rdata);
            Date date = new Date();

            // Create a DNSRecord object with the decoded information and the current date
            return new DNSRecord(name, type, c_, ttl, rdLength, rdata, date);
        }

        return new DNSRecord(name, 0, 0, 0, 0, new byte[0], new Date());
    }

    /**
     * Writes the DNSRecord object as bytes to the provided ByteArrayOutputStream.
     *
     * @param byteArrayOutputStream The ByteArrayOutputStream where the DNSRecord will be written.
     * @param domainNameLocations   A HashMap mapping domain names to their locations for possible compression.
     * @throws IOException If there is an error during the writing process.
     */
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        // Write the domain name to the output stream with possible compression
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocations, name_);
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);

        dataOutputStream.writeShort(type_);
        dataOutputStream.writeShort(class_);
        dataOutputStream.writeInt(ttl_);
        dataOutputStream.writeShort(rdLength_);
        dataOutputStream.write(rdata_);
    }

    /**
     * Returns a human-readable string representation of the DNSRecord object.
     *
     * @return A string containing the values of the DNSRecord's fields.
     */
    public String toString() {
        return "DNS Record={" +
                "name: " + name_ +
                "type: " + type_ +
                "class: " + class_ +
                "ttl: " + ttl_ +
                "rdLength: " + rdLength_ +
                "rdata: " + Arrays.toString(rdata_) +
                "Date: " + createdDate_;
    }

    /**
     * Checks whether the DNSRecord has expired based on its time-to-live (TTL) value.
     *
     * @return True if the DNSRecord has expired, False otherwise.
     */
    boolean isExpired() {
        Date date = new Date();

        long currentTime = date.getTime();

        // Calculate the expiration time based on the creation date and TTL
        long expirationTime = createdDate_.getTime() + (ttl_ * 1000); //multiply by 1000 to convert from seconds to milliseconds

        return currentTime > expirationTime;
    }
}
