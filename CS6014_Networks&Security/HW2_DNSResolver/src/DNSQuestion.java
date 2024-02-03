import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Objects;

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

    private String[] qName_;
    private short qType_;
    private short qClass_;

    DNSQuestion() {

    }
    DNSQuestion(String[] qName, short qType, short qClass){
        qName_ = qName;
        qType_ = qType;
        qClass_ = qClass;
    }

    static DNSQuestion decodeQuestion(InputStream is, DNSMessage dnsMessage) throws IOException {

        DataInputStream inputStream = new DataInputStream(is);
        String[] qName = DNSMessage.readDomainName(is);
        short qType = inputStream.readShort();
        short qClass = inputStream.readShort();

        return new DNSQuestion(qName, qType, qClass);
    }

    void writeBytes(ByteArrayOutputStream outputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(outputStream, domainNameLocations, qName_);
        DataOutputStream dataOutputStream = new DataOutputStream(outputStream);
        // No more parts
//        dataOutputStream.writeByte(0);

        dataOutputStream.writeShort(qType_);
        dataOutputStream.writeShort(qClass_);
    }

    @Override
    public String toString() {
        return "DNSQuestion{" +
                "qName_='" + qName_ + '\'' +
                ", qType_=" + qType_ +
                ", qClass_=" + qClass_ +
                '}';
    }
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSQuestion that = (DNSQuestion) o;
        return qType_ == that.qType_ && qClass_ == that.qClass_ && Objects.equals(qName_, that.qName_);
    }

    @Override
    public int hashCode() {
        return Objects.hash(qName_, qType_, qClass_);
    }

}
