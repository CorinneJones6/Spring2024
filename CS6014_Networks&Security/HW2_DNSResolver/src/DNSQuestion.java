import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;

/*                                  1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

public class DNSQuestion {

    private String qName_;
    private short qType_;
    private short qClass_;

    DNSQuestion() {

    }
    DNSQuestion(String qName, short qType, short qClass){
        qName_ = qName;
        qType_ = qType;
        qClass_ = qClass;
    }

    static DNSQuestion decodeQuestion(InputStream is, DNSMessage dnsMessage) throws IOException {
        DataInputStream inputStream = new DataInputStream(is);
        String qName = "";
        int len;

        //todo: maybe??? use dnsMessage to extract the qName???
        while( (len = inputStream.readByte()) > 0) {
            byte[] record = new byte[len];
            for(int i=0; i<len; i++){
                record[i] = inputStream.readByte();
            }
            qName = new String (record, StandardCharsets.UTF_8);
        }
        short qType = inputStream.readShort();
        short qClass = inputStream.readShort();

        return new DNSQuestion(qName, qType, qClass);
    }

    void writeBytes(ByteArrayOutputStream bo, HashMap<String,Integer> domainNameLocations){
        //todo: implement this function
    }

//    toString(), equals(), and hashCode()
}
