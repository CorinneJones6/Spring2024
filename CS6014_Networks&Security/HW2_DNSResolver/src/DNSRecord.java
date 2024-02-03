import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
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

    DNSRecord(){

    }
    DNSRecord(String[] name, int type, int c_, int ttl, int rdLength, byte[] rdata, Date date){
        name_ = name;
        type_ = type;
        class_ = c_;
        ttl_ = ttl;
        rdLength_ = rdLength;
        rdata_ = rdata;
        createdDate_ = date;
    }

    static DNSRecord decodeRecord(InputStream is, DNSMessage dnsMessage) throws IOException {
        DataInputStream inputStream = new DataInputStream(is);
        String[] name;

        inputStream.mark(2);

        short firstTwoBytes = inputStream.readShort();

        if ((firstTwoBytes & 0xC000) == 0xC000) {
           int offset = firstTwoBytes & 0x3FFF;
           name = dnsMessage.readDomainName(offset);

        } else {
            inputStream.reset();
            name = dnsMessage.readDomainName(inputStream);
        }


        int type = inputStream.readShort();
        int c_ = inputStream.readShort();
        int ttl = inputStream.readInt();
        int rdLength = inputStream.readShort();

        byte[] rdata = new byte[rdLength];
        inputStream.readFully(rdata);

        Date date = new Date();

        return new DNSRecord(name, type, c_, ttl, rdLength, rdata, date);
    }
    public void writeBytes (ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocations, name_);
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);

        dataOutputStream.writeShort(type_);
        dataOutputStream.writeShort(class_);
        dataOutputStream.writeLong(ttl_);
        dataOutputStream.writeShort(rdLength_);
        dataOutputStream.write(rdata_);
    }

    public String toString(){
        return "DNS Record={" +
                "name: " + name_ +
                "type: " + type_ +
                "class: " + class_ +
                "ttl: " + ttl_ +
                "rdLength: " + rdLength_ +
                "rdata: " + Arrays.toString(rdata_) +
                "Date: " + createdDate_;
    }

    boolean isExpired(){
        Date date = new Date();

        long currentTime = date.getTime();

        long expirationTime = createdDate_.getTime()+ (ttl_*1000);

        return currentTime>expirationTime;
    }

}
