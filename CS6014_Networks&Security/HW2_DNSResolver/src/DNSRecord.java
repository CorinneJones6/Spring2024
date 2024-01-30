import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
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

    String name_;
    short type_;
    short class_;
    int ttl_;
    int rdLength_;
    byte[] rdata_;
    Date date_;

    DNSRecord(){

    }
    DNSRecord(String name, short type, short c_, int ttl, int rdLength, byte[] rdata, Date date){
        name_ = name;
        type_ = type;
        class_ = c_;
        ttl_ = ttl;
        rdLength_ = rdLength;
        rdata_ = rdata;
        date_=date;
    }

    static DNSRecord decodeRecord(InputStream is, DNSMessage dnsMessage) throws IOException {
        DataInputStream inputStream = new DataInputStream(is);

        String name = "";
        //todo: use dnsMessage to read in the name????

        short type = inputStream.readShort();
        short class_ = inputStream.readShort();
        int ttl = inputStream.readInt();
        int rdLength = inputStream.readShort();

        byte[] rdata = new byte[rdLength];
        inputStream.readFully(rdata);

        Date date = new Date();

        return new DNSRecord(name, type, class_, ttl, rdLength, rdata, date);
    }
    public void writeBytes (ByteArrayOutputStream ob, HashMap<String, Integer> hm) {
        //todo: implement this function
    }
    public String toString(){
        //todo: implement this function
        return "";
    }

    boolean isExpired(){
        //todo: implement this function
        return false;
    }


}
